#pragma once

#include "IArduinoTarget.h"
#include "../Shared/AttachableRegistry.h"
#include "../Shared/GlobalSerial.h"
#include "../Server/ServerCommandHandler.h"
#include "../Server/Message.h"

#include <Ethernet.h>
#include <Vector.h>

struct SocketServer : public IArduinoTarget<SocketServer> {
    const static byte MAC_ADDRESS[6];   // Any address will work, as long as it's not used by a different device in the network.
    const static byte IP_ADDRESS[4];    // Currently unused: using DHCP instead.
    const static short PORT;

    static EthernetServer server;

    static void setup(void) {
        Ethernet.begin((byte*) MAC_ADDRESS);
        server.begin();

        GSerial.print("Initialized socket server at ");
        GSerial.println(Ethernet.localIP());
    }

    static void loop(void) {
        EthernetClient client = server.available();

        if (client) {
            GSerial.print("Client connected from ");
            GSerial.println(client.remoteIP());

            // Message is made up as follows:
            // 4 bytes:   Message ID, so the original message can be identified from the response.
            // 1 byte:    Device ID, so the message can be routed elsewhere. (0 = local command, 1 = local device)
            // 1 byte:    Port ID, so the appropriate action can be taken at the message destination.
            // Remainder: Null-terminated string containing optional data. If there is no data, just the null-terminator is sent.
            // Max message length is 256 bytes.
            
            Message* msg;
            {
                // Read message data.
                byte MsgBuffer[256];
                byte size = 0;
                do {
                    MsgBuffer[size] = client.read();
                    ++size;
                } while (client.connected() && !(size > 6 && MsgBuffer[size - 1] == '\0'));

                if (size < 7) {
                    GSerial.println("<Received corrupt or invalid message>");
                    return;
                }

                // Construct message object.
                msg = new Message(
                    *((int32_t*)&MsgBuffer[0]),
                    MsgBuffer[4],
                    MsgBuffer[5],
                    (char*)(MsgBuffer + 6)
                );
            }

            // Execute & send response.
            if (msg->device == 0) {
                GSerial.printf("Running local command %i\n", msg->port);

                // Handle as local command.
                String response = CommandHandler.HandleCommand(*msg);

                // Add message ID to response.
                // Length = string length + null terminator + 4 bytes of message ID.
                byte* binresponse = new byte[response.length() + 5];
                memcpy(binresponse, &((*msg).MessageID), 4);
                memcpy(binresponse + 4, response.c_str(), response.length() + 1);

                // Send response.
                client.write(binresponse, response.length() + 5);

                delete[] binresponse;
            } else {
                // Handle as attachable request.
                // TODO
            }
        }
    }
};