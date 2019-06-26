#pragma once

#include "Unique.h"

inline bool IsNumber(SafeCString& str) {
    const static char DIGITS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    if (strlen(str.raw_ptr()) == 0) return false;

    for (byte i = 0; i < strlen(str.raw_ptr()); ++i) {
        bool found = false;
        for (byte j = 0; j < 10; ++j) {
            if (str[i] == DIGITS[j]) found = true;
        }

        if (!found) return false;
    }

    return true;
}