#pragma once

template <typename T> class Unique {
public:
    template <typename... Args> Unique(Args&... args) {
        ptr = new T(args...);
    }

    Unique(T* ptr) : ptr(ptr) {}

    ~Unique(void) {
        if (ptr != nullptr) delete ptr;
    }

    Unique(const Unique&) = delete;

    Unique(const Unique&& other) {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    T& operator->(void) {
        return *ptr;
    }

    const T& operator->(void) const {
        return *ptr;
    }

    T* raw_ptr(void) {
        return ptr;
    }
private:
    mutable T* ptr;
};



template <typename T> class UniqueArray {
public:
    UniqueArray(T* ptr) : ptr(ptr) {}

    ~UniqueArray(void) {
        if (ptr != nullptr) delete[] ptr;
    }

    UniqueArray(const UniqueArray&) = delete;

    UniqueArray(const UniqueArray&& other) {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    T& operator[](byte index) {
        return *(ptr + index);
    }

    const T& operator[](byte index) const {
        return *(ptr + index);
    }

    T* raw_ptr(void) {
        return ptr;
    }
private:
    mutable T* ptr;
};


using SafeCString = UniqueArray<char>;