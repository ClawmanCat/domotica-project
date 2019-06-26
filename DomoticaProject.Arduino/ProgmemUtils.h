#pragma once

#include "boost/preprocessor.hpp"
#include "Unique.h"

#define _MAKE_STRARR_IMPL_PR_MACRO(Index, Data, Elem) static const char S##Index[] PROGMEM = Elem;
#define _MAKE_STRARR_IMPL_AR_MACRO(Index, Data, Elem) ( Data##Detail::S##Index )

#define MAKE_PROGMEM_STRING_ARRAY(Name, ...)                            \
namespace Name##Detail {                                                \
    BOOST_PP_SEQ_FOR_EACH(                                              \
        _MAKE_STRARR_IMPL_PR_MACRO,                                     \
        ,                                                               \
        BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)                           \
    )                                                                   \
}                                                                       \
                                                                        \
static const char* const Name[] PROGMEM = {                             \
    BOOST_PP_SEQ_ENUM(                                                  \
        BOOST_PP_SEQ_FOR_EACH(                                          \
            _MAKE_STRARR_IMPL_AR_MACRO,                                 \
            Name,                                                       \
            BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)                       \
        )                                                               \
    )                                                                   \
};                                                                      \
                                                                        \
static const byte Num##Name = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__);



namespace ProgmemUtils {
    inline SafeCString ReadProgmemString(const char* const* csstorage, byte index) {
        char* ptr = (char*) pgm_read_word(&(csstorage[index]));
        byte len = strlen_P(ptr);

        UniqueArray<char> storage = new char[len + 1];
        strcpy_P(storage.raw_ptr(), ptr);

        return storage;
    }
}