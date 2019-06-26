#pragma once

#define ServerOnly

#include <Ethernet.h>

#include "IArduinoTarget.h"
#include "GlobalSerial.h"
#include "Unique.h"
#include "EthernetMessage.h"
#include "CommandHandler.h"
#include "ServerDevices.h"

struct ArduinoServer : public IArduinoTarget<ArduinoServer> {
    const static byte MAC_ADDRESS[6];   // Any address will work, as long as it's not used by a different device in the network.
    const static byte IP_ADDRESS[4];    // Fallback address if DHCP doesn't work.
    const static short PORT;

    static EthernetServer server;


    static void setup(void) {
        ServerDevices::RegisterAll();

        // Attempt to use DHCP but use a static address if that fails.
        if (Ethernet.begin((byte*) MAC_ADDRESS)) {
            GSerial.println(F("DHCP is supported."));
        } else {
            GSerial.println(F("DHCP is not supported. Using fallback address..."));
            Ethernet.begin((byte*) MAC_ADDRESS, IPAddress(IP_ADDRESS));
        }

        server.begin();

        GSerial.print(F("Initialized socket server at "));
        GSerial.print(Ethernet.localIP());
        GSerial.print(":");
        GSerial.println(PORT);
    }


    static void loop(void) {
        EthernetClient client = server.available();

        if (client) {
            GSerial.print(F("Client connected from "));
            GSerial.println(client.remoteIP());

            // Scope is here so message buffer will be deallocated earlier.
            Unique<EthernetMessage> msg = nullptr;
            {
                // Read message data until we have the entire header (6 bytes) and a null-terminator character is received.
                byte MsgBuffer[255];
                byte size = 0;

                do {
                    MsgBuffer[size] = client.read();
                    ++size;
                } while (client.connected() && !(size > 6 && MsgBuffer[size - 1] == '\0'));

                if (size < 7) {
                    GSerial.println(F("<Received corrupt or invalid message>"));
                    return;
                }

                // Construct message object from data.
                msg = new EthernetMessage(
                    *((int32_t*) &MsgBuffer[0]),
                    MsgBuffer[4],
                    MsgBuffer[5],
                    SafeCString::FromCopy((char*) MsgBuffer + 6, strlen((char*) MsgBuffer + 6))
                );
            }

            GSerial.printf("Received message from client: dest=%i:%i", msg->device, msg->index);
            GSerial.println(msg->data.raw_ptr());

            // Message has been received, now run the appropriate command or dispatch to an attachable.
            SafeCString response = nullptr;
            if (msg->device == 0) {
                GSerial.printf("Running local command %i\n", msg->index);
                response = CmdHandler.HandleCommandMessage(*msg);
            } else {
                GSerial.printf("Running attachable request on device %i, port %i\n", msg->device, msg->index);
                Attachable* a = AttRegistry.FindAttachable(msg->device, msg->index);
                response = a->CallCallback(a, move(msg->data));
            }

            // Send response back to app.
            UniqueArray<byte> binresponse = new byte[strlen(response.raw_ptr()) + 5];  // +4 for the Message ID, +1 for the null-terminator.
            memcpy(binresponse.raw_ptr() + 0, &msg->MessageID, 4);
            memcpy(binresponse.raw_ptr() + 4, response.raw_ptr(), strlen(response.raw_ptr()) + 1);

            GSerial.println(response.raw_ptr());

            GSerial.printf("Sending response to app... (%i bytes)\n", strlen(response.raw_ptr()) + 5);
            client.write(binresponse.raw_ptr(), strlen(response.raw_ptr()) + 5);
        }
    }
};