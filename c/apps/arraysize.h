#ifndef ARRAYSIZE_H_
#define ARRAYSIZE_H_

/**
 * Helper macro for counting the number of elements in the array.
 *
 * \note Don't use this macro on pointers.
 *
 * \param[in] arr A C array.
 * \return The number of elements in the array.
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#endif /* ARRAYSIZE_H_ */
