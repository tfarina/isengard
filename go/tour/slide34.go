package main

import "fmt"

//
// Making slices:
//
// Slices are created with the make function. It works by allocating a zeroed
// array and returning a slice that refers to that array:
//
func main() {
        a := make([]int, 5)
        fmt.Println(a)
        fmt.Println(len(a))
}
