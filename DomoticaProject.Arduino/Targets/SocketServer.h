#pragma once

#include "IArduinoTarget.h"
#include "../Shared/AttachableRegistry.h"
#include "../Shared/GlobalSerial.h"

#include <Ethernet.h>
#include <Vector.h>

struct SocketServer : public IArduinoTarget<SocketServer> {
    const static byte MAC_ADDRESS[6];   // Any address will work, as long as it's not used by a different device in the network.
    const static byte IP_ADDRESS[4];    // TODO: Use DHCP / display address on screen or something.
    const static short PORT;

    static EthernetServer server;

    static void setup(void) {
        Ethernet.begin((byte*) MAC_ADDRESS, IPAddress(IP_ADDRESS));
        server.begin();

        GSerial.print("Initialized socket server at ");
        GSerial.println(IPAddress(IP_ADDRESS));
    }

    static void loop(void) {
        EthernetClient client = server.available();

        if (client) {
            GSerial.print("Client connected from ");
            GSerial.println(client.remoteIP());

            Vector<byte> request;
            while (client.connected()) request.push_back((byte) client.read());
            if (request.size() < 2) {
                GSerial.println("<Received corrupted message.>");
                return;
            }
            request.push_back((byte) '\0');

            GSerial.println("Received message from client: ");
            GSerial.printf("(Device: %i, Port: %i)\n", request[0], request[1]);
            GSerial.println((const char*) &request[2]);

            const Attachable* a = AttachableRegistry::instance().find(request[0], request[1]);
            if (a == nullptr) {
                GSerial.println("<Request made to non-existant attachable.>");
                return;
            }

            GSerial.printf("Found device %s\n", a->DisplayName);
            a->RequestCallback((const char*) &request[2], a);
        }
    }
};