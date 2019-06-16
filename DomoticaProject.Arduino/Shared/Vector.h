#pragma once

template <typename T> class Vector {
public:
    const static byte REALLOC_SIZE = 4;

    T* begin(void) { return data; }
    T* end(void)   { return data + isize; }
    const T* begin(void) const { return data; }
    const T* end(void) const   { return data + isize; }


    Vector(byte initial_capacity = REALLOC_SIZE) {
        capacity = initial_capacity;
        data = operator new[](capacity * sizeof(T));
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

    T& operator[](byte index) {
        return data[index];
    }

    const T& operator[](byte index) const {
        return data[index];
    }

    byte size(void) const {
        return isize;
    }
private:
    T* data;
    byte isize, capacity;
};