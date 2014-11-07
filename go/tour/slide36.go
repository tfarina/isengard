package main

import "fmt"

//
// Range
//
// The range form of the for loop iterates over a slice or map.
//
func main() {
	slice := []int{1, 2, 3}

	for i, value := range slice {
		fmt.Println(i, value)
	}
}
