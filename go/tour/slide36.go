package main

import "fmt"

//
// Range
//
// The range form of the for loop iterates over a slice or map.
//
func main() {
	slice := []int{1, 2, 3}

	fmt.Println("Index Value")
	for i, value := range slice {
		fmt.Println(i, value)
	}

	fmt.Println("Index")
	for i, _ := range slice {
		fmt.Println(i)
	}

	fmt.Println("Value")
	for _, value := range slice {
		fmt.Println(value)
	}
}
