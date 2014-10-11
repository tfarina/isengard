package main

import "fmt"

//
// Slices:
//
// A slice points to an array of values and also includes a length.
//
// []T is a slice with elements of type T.
//
func main() {
        primes := []int{2, 3, 5, 7, 11}

        fmt.Println(primes)

        for i := 0; i < len(primes); i++ {
                fmt.Printf("%d\n", primes[i])
        }
}
