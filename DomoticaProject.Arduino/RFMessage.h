#pragma once

#include "Unique.h"
#include "Move.h"

struct RFMessage {
    byte index;
    SafeCString data;


    RFMessage(byte index, SafeCString&& data)
        : index(index), data(move(data)) {}


    UniqueArray<byte> Serialize(void) {
        UniqueArray<byte> result = new byte[strlen(data.raw_ptr()) + 2];
        memcpy(result.raw_ptr() + 0, &index,          1);
        memcpy(result.raw_ptr() + 1, data.raw_ptr(),  strlen(data.raw_ptr()) + 1);

        return result;
    }

    static RFMessage Deserialize(UniqueArray<byte>& bytes) {
        return RFMessage(
            bytes[0],
            SafeCString::FromCopy((char*) bytes.raw_ptr() + 1, strlen((char*) bytes.raw_ptr() + 1))
        );
    }
};