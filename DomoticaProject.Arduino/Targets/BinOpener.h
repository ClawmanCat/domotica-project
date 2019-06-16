#pragma once

#include "IArduinoTarget.h"
#include "../Shared/GlobalSerial.h"

#include <Ultrasonic.h>
#include <Servo.h>

struct BinOpener : public IArduinoTarget<BinOpener> {
    const static unsigned short TRIGGER_DISTANCE = 100;  // cm.

    const static byte TRIG_PIN = 8;
    const static byte ECHO_PIN = 9;
    const static byte SRVO_PIN = 10;

    static Ultrasonic us;
    static bool IsTriggered;

    static Servo servo;

    static void setup(void) {
        servo.attach(SRVO_PIN);

        GSerial.println("Starting measurements...");
    }

    static void loop(void) {
        int distance = us.read();

        if (distance < TRIGGER_DISTANCE && !IsTriggered) {
            GSerial.printf("Opening! (%i cm)\n", distance);

            servo.write(180);

            IsTriggered = true;
            delay(2500);
        } else if (distance >= TRIGGER_DISTANCE && IsTriggered) {
            GSerial.printf("Closing! (%i cm)\n", distance);

            servo.write(0);

            IsTriggered = false;
            delay(2500);
        }

    }
};