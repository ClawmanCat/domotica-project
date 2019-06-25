#pragma once

template <typename T> struct remove_reference {
    typedef T type;
};

template <typename T> struct remove_reference<T&> {
    typedef T type;
};

template <typename T> struct remove_reference<T&&> {
    typedef T type;
};


template<typename T> inline typename remove_reference<T>::type&&
move(T&& arg) {
    return ((typename remove_reference<T>::type&&) arg);
}

