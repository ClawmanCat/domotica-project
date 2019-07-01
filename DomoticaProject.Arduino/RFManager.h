#pragma once

#include <Wire.h>

#include "RFMessage.h"
#include "Unique.h"
#include "AttachableRegistry.h"

#define RF RFManager::instance()

// RF communication is currently not working.
// We're currently using the wire library instead to communicate via I2C.
// Unfortunately, this currently isn't working either.
class RFManager {
public:
    const static byte TRNS_PIN = 2;
    const static byte RECV_PIN = 3;

    #ifdef ServerOnly
    const static byte ADDRESS = 1;
    #else
    const static byte ADDRESS = CLIENT_DEVICE_ID;
    #endif

    static RFManager& instance(void) {
        static RFManager i = RFManager();
        return i;
    }
    
    #ifdef ServerOnly
    Unique<RFMessage> RequestResponse(RFMessage& msg, short timeout) {
        init();

        Wire.beginTransmission(msg.dest);
        Wire.write(msg.Serialize().raw_ptr(), msg.size());
        Wire.endTransmission();

        delay(1000);

        Wire.requestFrom(msg.dest, (byte) 255);

        UniqueArray<byte> response = new byte[255];
        byte count = 0;

        auto start = millis();

        pinMode(42, HIGH);
        do {
            if (Wire.available()) response[count++] = Wire.read();
        } while ((timeout == 0 || start + timeout < millis()) && (count < 5 || response[count - 1] != '\0'));
        pinMode(42, LOW);

        if (count < 5 || response[count - 1] != '\0') return Unique<RFMessage>(nullptr);
        return Unique<RFMessage>::FromCopy(&RFMessage::Deserialize(response));
    }

    SafeCString RequestAttachable(byte device, byte index, SafeCString&& data) {
        RFMessage msg = RFMessage(ADDRESS, device, index, move(data));
        auto m = RequestResponse(msg, 2500);

        if (m.raw_ptr() == nullptr) return SafeCString::FromCopy("ERROR", 6);
        return move(m->data);
    }

    void FetchAttachables(void) {
        byte next = 2;
        bool done = false;

        while (!done) {
            RFMessage msg = RFMessage(ADDRESS, next, 0, SafeCString::FromCopy("", 1));
            auto m = RequestResponse(msg, 2500);

            if (m.raw_ptr() == nullptr) {
                done = true;
            } else {
                AttRegistry.RegisterAttachable(new Attachable(
                    [](Attachable* thisptr) {},
                    [](Attachable* thisptr, SafeCString&& data) {
                        return RF.RequestAttachable(thisptr->device, thisptr->index, move(data));
                    },
                    move(m->data),
                    m->src,
                    m->index
                ));

                ++next;
            }
        }
    }
    #endif

    #ifdef ClientOnly
    void Respond(RFMessage& msg) {
        init();
        Wire.beginTransmission(0);
        Wire.write(msg.Serialize().raw_ptr(), msg.size());
        Wire.endTransmission();
    }

    void HandleClientRequest(Unique<RFMessage>& msg) {
        Unique<RFMessage>& m = msg;

        if (m->index == 0) {
            // Request attachables.
            RFMessage msg = RFMessage(ADDRESS, 1, 0, AttRegistry.ListAttachables());
            Respond(msg);
        } else {
            // Run attachable.
            Attachable* a = AttRegistry.FindAttachable(m->index);

            RFMessage msg = RFMessage(ADDRESS, 1, 0, a->CallCallback(a, move(m->data)));
            Respond(msg);
        }
    }
    #endif

    RFManager(void) {}

    #ifdef ClientOnly
    byte reqbuf[127];
    #endif

    void init(void) {
        static bool inited = false;
        if (inited) return;

        ADDRESS == 1 ? Wire.begin() : Wire.begin(ADDRESS);

        #ifdef ClientOnly
        Wire.onReceive([](int) {
            byte i = 0;
            while (Wire.available()) RF.reqbuf[i++] = Wire.read();
        });

        Wire.onRequest([]() {
            Unique<RFMessage> msg = Unique<RFMessage>::FromCopy(&RFMessage::Deserialize(RF.reqbuf));
            RF.HandleClientRequest(msg);
        });
        #endif

        inited = true;
    }
};