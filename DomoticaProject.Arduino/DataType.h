#pragma once

#include "MagicEnum.h"
#include "Unique.h"
#include "Vector.h"

MAGIC_ENUM_CLASS(DataType, byte, NONE, STRING, INT, FLOAT, BOOL);


inline SafeCString MakeDataTypeMessage(DataType ReturnType, Vector<DataType, 1>& ArgTypes) {
    byte len = (2 * ArgTypes.size() - 1)            // For each argument, the type ID and a comma, no comma for the last argument. (-1)
             + 2                                    // ID + comma of the return type.
             + 2                                    // Opening and closing braces.
             + 1;                                   // Null-terminator.

    SafeCString result = new char[len];
    char* const resptr = result.raw_ptr();

    // Return type.
    resptr[1] = (char) ReturnType;
    resptr[2] = ',';

    // Arguments.
    for (byte i = 0; i < ArgTypes.size(); ++i) {
        resptr[3 + (i * 2)] = (char) ArgTypes[i];
        resptr[4 + (i * 2)] = ',';
    }

    // Opening & closing braces, null-terminator.
    resptr[0] = '{';
    resptr[len - 2] = '}';
    resptr[len - 1] = '\0';

    return result;
}


inline Vector<DataType, 1> DecodeDataTypeMessage(const SafeCString& str) {
    Vector<DataType, 1> result;

    for (byte i = 1; str[i] != '\0'; i += 2) {
        result.push_back((DataType) str[i]);
    }

    return result;
}