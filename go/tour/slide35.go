package main

import "fmt"

//
// Nil Slices:
//
// The zero value of a slice is nil.
//
// A nil slice has a length and capacity of 0.
//
func main() {
        var slice []int
        fmt.Println(slice, len(slice), cap(slice))

        if slice == nil {
                fmt.Println("Slice is nil!")
        }
}
