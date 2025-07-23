#ifndef ARRAYSIZE_H_
#define ARRAYSIZE_H_

/**
 * @brief Computes the number of elements in a statically allocated array.
 *
 * This macro returns the number of elements in an array at compile time.
 * It is intended for use with arrays that are declared locally or globally,
 * not with pointers or dynamically allocated arrays.

 * @code
 * int array[10];
 * size_t array_size = ARRAY_SIZE(array); // array_size will be 10
 * @endcode
 *
 * @warning This macro only works with statically allocated arrays. If used
 * with a pointer (including arrays passed to functions, they decay to a
 * pointer), the result will be incorrect because the `sizeof` operator
 * returns the size of the pointer not the array it may point to.

 * @param[in] array A statically allocated array.
 * @return The number of elements in the array, as a `size_t`.
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif

#endif /* ARRAYSIZE_H_ */
