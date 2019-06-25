#pragma once

// RAII-wrapped unique pointer.
// Can be moved, but not copied.
template <typename T> class Unique {
public:
    template <typename... Args> Unique(Args&... args) {
        ptr = new T(args...);
    }

    Unique(void) : ptr(nullptr) {}

    Unique(T* ptr) : ptr(ptr) {}

    ~Unique(void) {
        if (ptr != nullptr) delete ptr;
    }

    Unique(const Unique<T>&) = delete;

    Unique(const Unique<T>&& other) {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    void operator=(const Unique<T>&& other) {
        if (ptr != nullptr) delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    T* operator->(void) {
        return ptr;
    }

    T& operator*(void) {
        return *ptr;
    }

    const T& operator->(void) const {
        return *ptr;
    }

    T*& raw_ptr(void) {
        return ptr;
    }

    bool is_assigned(void) const {
        return ptr != nullptr;
    }

    static Unique<T> FromCopy(const T* ptr) {
        Unique<T> obj = (T*) operator new(sizeof(T));
        memcpy(obj.raw_ptr(), ptr, sizeof(T));

        return obj;
    }
private:
    mutable T* ptr;
};


// RAII-wrapped unique c-array.
// Can be moved, but not copied.
template <typename T> class UniqueArray {
public:
    UniqueArray(T* ptr) : ptr(ptr) {}

    ~UniqueArray(void) {
        if (ptr != nullptr) delete[] ptr;
    }

    UniqueArray(const UniqueArray<T>&) = delete;

    UniqueArray(const UniqueArray<T>&& other) {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    void operator=(const UniqueArray<T>&& other) {
        if (ptr != nullptr) delete[] ptr;
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

    static UniqueArray<T> FromCopy(const T* ptr, byte size) {
        UniqueArray<T> arr = new T[size];
        memcpy(arr.raw_ptr(), ptr, size * sizeof(T));

        return arr;
    }
private:
    mutable T* ptr;
};


using SafeCString = UniqueArray<char>;


inline bool SCStringEquals(SafeCString& a, SafeCString& b,  byte offA = 0, byte offB = 0) {
    return strcmp((char*) a.raw_ptr() + offA, (char*) b.raw_ptr() + offB);
}

inline bool SCStringEquals(SafeCString& a, SafeCString& b, byte offA, byte offB, byte cnt) {
    return memcmp(a.raw_ptr() + offA, b.raw_ptr() + offB, cnt * sizeof(char));
}