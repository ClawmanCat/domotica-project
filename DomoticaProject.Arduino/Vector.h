#pragma once

#include "Move.h"

// Dynamically-resizing array.
// Note: be careful with large vectors, resizing the vector may temporarily consume up to
// 2 * old size + realloc size bytes of memory.
template <typename T, byte REA_SIZE = 4> class Vector {
public:
    const static byte REALLOC_SIZE = REA_SIZE;

    // Iterators.
    T* begin(void) { return data; }
    T* end(void) { return data + isize; }
    const T* begin(void) const { return data; }
    const T* end(void) const { return data + isize; }

    // Construct empty with capacity.
    Vector(byte initial_capacity = REALLOC_SIZE) {
        capacity = initial_capacity;
        data = (T*) operator new[](capacity * sizeof(T));
    }

    // Construct from existing container.
    Vector(T* itbegin, T* itend) : Vector(itend - itbegin) {
        for (byte i = 0; i < itend - itbegin; ++i) data[i] = *(itbegin + i);
        isize = (itend - itbegin);
    }

    ~Vector(void) {
        for (byte i = 0; i < isize; ++i) pop_back();
        operator delete[](data);
    }

    // Add element to back of vector.
    void push_back(T& elem) {
        if (isize < capacity) {
            data[isize++] = elem;
        } else {
            T* newdata = operator new[]((capacity + REALLOC_SIZE) * sizeof(T));
            memcpy(newdata, data, isize);

            operator delete[](data);
            data = newdata;

            capacity += REALLOC_SIZE;
            push_back(elem);
        }
    }

    void push_back(T&& elem) {
        if (isize < capacity) {
            data[isize++] = move(elem);
        } else {
            T* newdata = operator new[]((capacity + REALLOC_SIZE) * sizeof(T));
            memcpy(newdata, data, isize);

            operator delete[](data);
            data = newdata;

            capacity += REALLOC_SIZE;
            push_back(elem);
        }
    }

    // Remove element from back of vector.
    void pop_back() {
        data[--isize].~T();
    }

    // Erase element within vector.
    void erase(byte index) {
        data[index].~T();
        memmove(data + index, data + index + 1, (isize - index - 1) * sizeof(T));
        --isize;
    }

    T& operator[](byte index) {
        return data[index];
    }

    const T& operator[](byte index) const {
        return data[index];
    }

    byte size(void) const {
        return isize;
    }

    T* raw_ptr(void) {
        return data;
    }
private:
    T* data;
    byte isize, capacity;
};