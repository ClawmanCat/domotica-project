#include "BinOpener.h"

Ultrasonic BinOpener::us = Ultrasonic(BinOpener::TRIG_PIN, BinOpener::ECHO_PIN);
Servo BinOpener::servo = Servo();

bool BinOpener::IsTriggered = false;