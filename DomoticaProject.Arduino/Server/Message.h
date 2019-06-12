#pragma once

#include <Arduino.h>

struct Message {
    long long MessageID;
    byte device, port;
    const byte* data;

    Message(long long MessageID, byte device, byte port, const byte* data)
        : MessageID(MessageID), device(device), port(port), data(data) {}
};