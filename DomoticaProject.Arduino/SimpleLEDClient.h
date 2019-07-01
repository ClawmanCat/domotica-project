#pragma once

// Client device ID must be unique within network.
// Make sure to change it when adding a client.
// IDs should be continuous.
#define ClientOnly
#define CLIENT_DEVICE_ID 2

#include "IArduinoTarget.h"
#include "RFManager.h"
#include "AttachableRegistry.h"
#include "IsNumber.h"

struct SimpleLEDClient : public IArduinoTarget<SimpleLEDClient> {
    static void setup(void) {
        byte NextIndex = 1;

        AttRegistry.RegisterAttachable(new Attachable(
            [](Attachable* thisptr) {
                DDRB = 0xFF;
            },
            [](Attachable* thisptr, SafeCString&& data) {
                if (!IsNumber(data)) return SafeCString::FromCopy("NO", 3);
                
                char* endptr = nullptr;
                byte state = (byte) strtol(data.raw_ptr(), &endptr, 10);

                PORTB = state;
                return SafeCString::FromCopy("OK", 3);
            },
            SafeCString::FromCopy("Client LED Array", 17),
            NextIndex++
        ));
    }

    static void loop(void) {
        RF.init();
    }
};