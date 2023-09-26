#ifndef BASE_ARRAYSIZE_H_
#define BASE_ARRAYSIZE_H_

#include <stddef.h>

#ifndef arraysize
template <typename T, size_t N>
    char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))
#endif

#endif  // BASE_ARRAYSIZE_H_
