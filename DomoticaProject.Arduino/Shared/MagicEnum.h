#pragma once

#include <Dependencies/boost/preprocessor.hpp>

#define _MAGIC_ENUM_IMPL_FE_MACRO(Index, Data, Elem) BOOST_PP_STRINGIZE(Elem),

#define _MAGIC_ENUM_IMPL(Name, EType, UType, ...)                       \
enum EType Name : UType { __VA_ARGS__ };                                \
                                                                        \
const static UType Num##Name##s = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__);  \
                                                                        \
const char* Name##ToString(Name val) {                                  \
    static const char* StringData[] = {                                 \
        BOOST_PP_SEQ_FOR_EACH(                                          \
            _MAGIC_ENUM_IMPL_FE_MACRO,                                  \
            _,                                                          \
            BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)                       \
        )                                                               \
        ""                                                              \
    };                                                                  \
                                                                        \
    return StringData[(UType) val];                                     \
}

#define MAGIC_ENUM(Name, UnderlyingType, ...)       _MAGIC_ENUM_IMPL(Name,      , UnderlyingType, __VA_ARGS__)
#define MAGIC_ENUM_CLASS(Name, UnderlyingType, ...) _MAGIC_ENUM_IMPL(Name, class, UnderlyingType, __VA_ARGS__)
