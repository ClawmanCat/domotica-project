#pragma once

#include "IArduinoTarget.h"

struct VMTest : public IArduinoTarget<VMTest> {
    static void setup(void) {
        DDRB = B00111111;
    }

    static void loop(void) {
        PORTB = 0x00;

        for (byte i = 0; i < (1 << 6); i++) {
            PORTB = i;
            delay(250);
        }
    }
};