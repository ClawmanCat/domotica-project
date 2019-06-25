#pragma once

#include <Arduino.h>
#include "../Shared/Unique.h"
#include "../Shared/Move.h"

struct Message {
    int32_t MessageID;
    byte device, port;
    SafeCString data;

    Message(int32_t MessageID, byte device, byte port, SafeCString&& data)
        : MessageID(MessageID), device(device), port(port), data(move(data)) {}
};