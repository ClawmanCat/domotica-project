#pragma once

#include <Arduino.h>

struct Message {
    int32_t MessageID;
    byte device, port;
    char* data;

    Message(int32_t MessageID, byte device, byte port, char* data)
        : MessageID(MessageID), device(device), port(port), data(data) {}
};