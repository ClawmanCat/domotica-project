#pragma once

// Helpers.
namespace Detail {
    template <typename T> struct remove_reference {
        typedef T type;
    };

    template <typename T> struct remove_reference<T&> {
        typedef T type;
    };

    template <typename T> struct remove_reference<T&&> {
        typedef T type;
    };
}

// Re-implementation of std::move
// Produces an xvalue expression that identifies its argument T,
// equivalent to a static_cast to an rvalue reference type.
template<typename T> inline typename Detail::remove_reference<T>::type&&
move(T&& arg) {
    return ((typename Detail::remove_reference<T>::type&&) arg);
}

