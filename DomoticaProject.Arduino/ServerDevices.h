#pragma once

#ifdef ServerOnly
#include "AttachableRegistry.h"
#include "ProgmemUtils.h"
#include "RFManager.h"
#include "IsNumber.h"

MAKE_PROGMEM_STRING_ARRAY(
    ServerDeviceStrings,
    "Server LED Array",
    "Server RF Transmitter"
);

class ServerDevices {
public:
    // Can't have the static definition in a .cpp file, since having an extra
    // translation unit here will break the ServerOnly definition.
    // Just wrap the static variable in a function instead.
    static byte* GetLEDPins(void) {
        const static byte LED_PINS[] = { 42, 44, 46, 48, 50, 52 };
        return (byte*) LED_PINS;
    }

    static void RegisterAll(void) {
        byte NextIndex = 0;


        // Writing to Server LED.
        AttRegistry.RegisterAttachable(new Attachable(
            [](Attachable* thisptr) {
                for (byte i = 0; i < 6; ++i) pinMode(ServerDevices::GetLEDPins()[i], OUTPUT);
            },
            [](Attachable* thisptr, SafeCString&& msg) {
                if (!IsNumber(msg)) return SafeCString::FromCopy("NO", 3);

                char* end = nullptr;
                byte state = (byte) strtol(msg.raw_ptr(), &end, 10);

                for (byte i = 0; i < 6; ++i) digitalWrite(ServerDevices::GetLEDPins()[i], state & (1 << i) ? HIGH : LOW);
                return SafeCString::FromCopy("OK", 3);
            },
            ProgmemUtils::ReadProgmemString(ServerDeviceStrings, 0),
            1,
            NextIndex++
        ));


        // RF Transmitter
        /*AttRegistry.RegisterAttachable(new Attachable(
            [](Attachable* thisptr) {},
            [](Attachable* thisptr, SafeCString&& msg) {
                RF.GetDriver().send((uint8_t*) msg.raw_ptr(), strlen(msg.raw_ptr() + 1));
                return SafeCString::FromCopy("OK", 3);
            },
            ProgmemUtils::ReadProgmemString(ServerDeviceStrings, 1),
            1,
            NextIndex++
        ));*/
    }
};
#endif