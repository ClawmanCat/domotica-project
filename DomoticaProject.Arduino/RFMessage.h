#pragma once

#include "Unique.h"
#include "Move.h"

struct RFMessage {
    byte src, dest, index;
    SafeCString data;


    RFMessage(byte src, byte dest, byte index, SafeCString&& data)
        : src(src), dest(dest), index(index), data(move(data)) {}


    UniqueArray<byte> Serialize(void) {
        UniqueArray<byte> result = new byte[strlen(data.raw_ptr()) + 4];
        memcpy(result.raw_ptr() + 0, &src,            1);
        memcpy(result.raw_ptr() + 1, &dest,           1);
        memcpy(result.raw_ptr() + 2, &index,          1);
        memcpy(result.raw_ptr() + 3, data.raw_ptr(),  strlen(data.raw_ptr()) + 1);

        return result;
    }

    static RFMessage Deserialize(UniqueArray<byte>& bytes) {
        return RFMessage(
            bytes[0],
            bytes[1],
            bytes[2],
            SafeCString::FromCopy((char*) bytes.raw_ptr() + 3, strlen((char*) bytes.raw_ptr() + 3) + 1)
        );
    }

    static RFMessage Deserialize(byte* bytes) {
        return RFMessage(
            bytes[0],
            bytes[1],
            bytes[2],
            SafeCString::FromCopy((char*) bytes + 3, strlen((char*) bytes + 3) + 1)
        );
    }

    byte size(void) {
        return strlen(data.raw_ptr()) + 4;
    }
};