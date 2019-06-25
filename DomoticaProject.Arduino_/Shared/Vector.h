#pragma once

template <typename T, byte REA_SIZE = 4> class Vector {
public:
    const static byte REALLOC_SIZE = REA_SIZE;

    T* begin(void) { return data; }
    T* end(void) { return data + isize; }
    const T* begin(void) const { return data; }
    const T* end(void) const { return data + isize; }


    Vector(byte initial_capacity = REALLOC_SIZE) {
        capacity = initial_capacity;
        data = (T*) operator new[](capacity * sizeof(T));
    }

    Vector(T* itbegin, T* itend) : Vector(itend - itbegin) {
        for (byte i = 0; i < itend - itbegin; ++i) data[i] = *(itbegin + i);
        isize = (itend - itbegin);
    }

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

    void pop_back() {
        data[--isize].~T();
    }

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