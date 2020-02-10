#ifndef ARRAYSIZE_H_
#define ARRAYSIZE_H_

/**
 * \brief Computes the number of elements in a statically-allocated array.
 *
 * \note Don't use this macro on pointers.
 *
 * \param[in] array The name of an array.
 *
 * \return The number of elements in the array, expressed as a size_t.
 */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(*(array)))

#endif /* ARRAYSIZE_H_ */
