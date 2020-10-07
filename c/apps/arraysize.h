#ifndef ARRAYSIZE_H_
#define ARRAYSIZE_H_

/**
 * @brief Computes the number of elements in a statically-allocated array.
 *
 * @note Ensure that 'array' is actually an array, not a pointer. Otherwise,
 *       ARRAY_SIZE will produce erroneous results if 'array' is a pointer.
 *       Pay attention to the fact that an array passed as a parameter to a
 *       function decays to a pointer, which means that within the function
 *       you can't use ARRAY_SIZE.
 *
 * @param[in] array The name of the array.
 *
 * @return    The number of elements in the array, expressed as a size_t.
 */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(*(array)))

#endif /* ARRAYSIZE_H_ */
