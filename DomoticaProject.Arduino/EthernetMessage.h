#pragma once

#include "Unique.h"
#include "Move.h"

struct EthernetMessage {
    int32_t MessageID;
    byte device, index;
    SafeCString data;

    EthernetMessage(int32_t id, byte device, byte index, SafeCString&& data)
        : MessageID(id), device(device), index(index), data(move(data)) {}
};