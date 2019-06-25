#pragma once

#include "../Dependencies/boost/preprocessor.hpp"
#include "../Shared/Unique.h"
#include "../Shared/ProgmemUtils.h"

#define _MAGIC_ENUM_IMPL_PR_MACRO(Index, Data, Elem) static const char S##Index[] PROGMEM = BOOST_PP_STRINGIZE(Elem);
#define _MAGIC_ENUM_IMPL_AR_MACRO(Index, Data, Elem) S##Index,

#define _MAGIC_ENUM_IMPL(Name, EType, UType, ...)                       \
enum EType Name : UType { __VA_ARGS__ };                                \
                                                                        \
SafeCString Name##ToString(Name val) {                                  \
    BOOST_PP_SEQ_FOR_EACH(                                              \
        _MAGIC_ENUM_IMPL_PR_MACRO,                                      \
        ,                                                               \
        BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)                           \
    );                                                                  \
                                                                        \
    static const char* const StringData[] PROGMEM = {                   \
        BOOST_PP_SEQ_FOR_EACH(                                          \
            _MAGIC_ENUM_IMPL_AR_MACRO,                                  \
            ,                                                           \
            BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)                       \
        )                                                               \
        nullptr                                                         \
    };                                                                  \
                                                                        \
    return ProgmemUtils::ReadProgmemString(StringData, (byte) val);     \
}                                                                       \
                                                                        \
const static UType Num##Name##s = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__);

#define MAGIC_ENUM(Name, UnderlyingType, ...)       _MAGIC_ENUM_IMPL(Name,      , UnderlyingType, __VA_ARGS__)
#define MAGIC_ENUM_CLASS(Name, UnderlyingType, ...) _MAGIC_ENUM_IMPL(Name, class, UnderlyingType, __VA_ARGS__)
