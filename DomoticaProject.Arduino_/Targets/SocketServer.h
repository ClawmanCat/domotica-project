#pragma once

#include "IArduinoTarget.h"
#include "../Shared/AttachableRegistry.h"
#include "../Shared/GlobalSerial.h"
#include "../Shared/Move.h"
#include "../Shared/RFTransmission.h"
#include "../Server/ServerCommandHandler.h"
#include "../Server/Message.h"

#include <Ethernet.h>

struct SocketServer : public IArduinoTarget<SocketServer> {
    const static byte MAC_ADDRESS[6];   // Any address will work, as long as it's not used by a different device in the network.
    const static byte IP_ADDRESS[4];    // Currently unused: using DHCP instead.
    const static short PORT;

    static EthernetServer server;

    static void setup(void) {
        Ethernet.begin((byte*) MAC_ADDRESS);
        server.begin();

        GSerial.print(F("Initialized socket server at "));
        GSerial.println(Ethernet.localIP());
    }

    static void loop(void) {
        EthernetClient client = server.available();

        if (client) {
            GSerial.print(F("Client connected from "));
            GSerial.println(client.remoteIP());

            // Message is made up as follows:
            // 4 bytes:   Message ID, so the original message can be identified from the response.
            // 1 byte:    Device ID, so the message can be routed elsewhere. (0 = local command, 1 = local device)
            // 1 byte:    Port ID, so the appropriate action can be taken at the message destination.
            // Remainder: Null-terminated string containing optional data. If there is no data, just the null-terminator is sent.
            // Max message length is 256 bytes.
            
            Unique<Message> msg = Unique<Message>();
            {
                // Read message data until we have the entire header (6 bytes) and a null-terminator character is received.
                byte MsgBuffer[256];
                byte size = 0;
                do {
                    MsgBuffer[size] = client.read();
                    ++size;
                } while (client.connected() && !(size > 6 && MsgBuffer[size - 1] == '\0'));

                if (size < 7) {
                    GSerial.println(F("<Received corrupt or invalid message>"));
                    return;
                } else {
                    GSerial.print(F("Received message from "));
                    GSerial.print(client.remoteIP());
                    GSerial.println(": ");
                    
                    byte at = 0;
                    while (MsgBuffer[at] != '\0') GSerial.printf("0x%0.02X ", (int) MsgBuffer[at++]);
                    GSerial.println();
                }

                // Construct message object.
                // Assignment here is safe, since the unique is guaranteed to not have been assigned yet.
                // (Scope here only serves to reduce lifetime of other resources, otherwise it could have
                // been diretly assigned.)
                SafeCString data = new char[strlen((char*) MsgBuffer + 6) + 1];

                msg.raw_ptr() = new Message(
                    *((int32_t*) &MsgBuffer[0]),
                    MsgBuffer[4],
                    MsgBuffer[5],
                    move(data)
                );
            }

            // Execute & send response.
            if (msg->device == 0) {
                GSerial.printf("Running local command %i\n", msg->port);

                // Handle as local command.
                SafeCString response = CommandHandler.HandleCommand(*msg);

                // Add message ID to response.
                // Length = string length + null terminator + 4 bytes of message ID.
                byte len = strlen(response.raw_ptr());

                UniqueArray<byte> binresponse = new byte[len + 5];
                memcpy(binresponse.raw_ptr(), &((*msg).MessageID), 4);
                memcpy(binresponse.raw_ptr() + 4, response.raw_ptr(), len + 1);

                // Send response.
                client.write(binresponse.raw_ptr(), len + 5);
            } else {
                // Handle as attachable request.
                if (msg->port >= AttRegistry.size()) return;
                Attachable* a = AttRegistry.GetAttachable(msg->port);
                a->CallCallback(*msg);
            }
        }
    }
};