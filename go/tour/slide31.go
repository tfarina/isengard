package main

import "fmt"

//
// Arrays:
//
// The type [n]T is an array of n values of type T.
//
// The expression
//
// var a [10]int
//
// declares a variable a as an array of ten integers.
//
// An array's length is part of its type, so arrays cannot be resized. This
// seems limiting, but don't worry; Go provides a convenient way of working
// with arrays.
//
func main() {
        var array[4]string

        array[0] = "Verizon"
        array[1] = "AT&T"
        array[2] = "Bell Labs"
        array[3] = "Vodafone"

        fmt.Println(array[0], array[2])
        fmt.Println()
        fmt.Println(array)
        fmt.Println()
        fmt.Println(array[1])
        fmt.Println(array[2])
        fmt.Println(array[0])
        fmt.Println(array[3])
}
