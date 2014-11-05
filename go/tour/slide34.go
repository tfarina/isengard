package main

import "fmt"

func printSlice(slice []int) {
        fmt.Printf("len=%d cap=%d %v\n", len(slice), cap(slice), slice)
}

//
// Making slices:
//
// Slices are created with the make function. It works by allocating a zeroed
// array and returning a slice that refers to that array:
//
func main() {
        s := make([]int, 5)
        printSlice(s)
}
