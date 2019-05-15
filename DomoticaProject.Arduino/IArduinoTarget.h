#pragma once

#include <Arduino.h>

template <typename Der> struct IArduinoTarget {
    static void setup(void) {
        Der::setup();
    }

    static void loop(void) {
        Der::loop();
    }
};