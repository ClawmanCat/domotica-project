#pragma once

#include <Arduino.h>

#define GSerial GlobalSerial::instance()

class GlobalSerial {
public:
    const static unsigned short FREQUENCY = 9600;

    static GlobalSerial& instance(void) {
        static GlobalSerial i = GlobalSerial();
        return i;
    }

    template <typename... Ts> void print(const Ts&... args) {
        init();
        Serial.print(args...);
    }

    template <typename... Ts> void println(const Ts&... args) {
        init();
        Serial.println(args...);
    }

    template <typename... Ts> void printf(const char* format, const Ts&... args) {
        init();

        const short bufsize = 2 * strlen(format) + 32;
        char* buf = new char[bufsize];

        int result = snprintf(buf, bufsize, format, args...);

        if (result == -1 || result > bufsize) {
            GSerial.println("<Error printing formatted data>");
        } else {
            GSerial.print(buf);
        }

        delete[] buf;
    }
private:
    bool initialized;

    GlobalSerial(void) : initialized(false) {}

    void init(void) {
        if (initialized) return;

        Serial.begin(FREQUENCY);
        while (!Serial);

        Serial.println("<Initialized Global Serial Writer>");
        initialized = true;
    }
};