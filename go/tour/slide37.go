package main

import "fmt"

//
// Range continued
//
// You can skip the index or value by assigning to _.
//
// If you only want the index, drop the ", value" entirely.
//
func main() {
	pow := make([]int, 10)

	for i := range pow {
		pow[i] = 1 << uint(i)
	}

	for _, value := range pow {
		fmt.Printf("%d\n", value)
	}
}
