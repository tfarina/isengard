package main

import "fmt"

func main() {
	primes := []int{2, 3, 5, 7, 11}
	fmt.Println(primes)

	// Slice
	// Remember that it is [lo:hi], but it evaluates to [lo:hi-1].
	fmt.Println("[1:4] is: ", primes[1:4])
}
