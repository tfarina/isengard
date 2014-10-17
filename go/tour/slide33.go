package main

import "fmt"

func main() {
	primes := []int{2, 3, 5, 7, 11}
	fmt.Println(primes)

	// Slice
	// Remember that it is [lo:hi], but it evaluates to [lo:hi-1].
	fmt.Println("[1:4] is: ", primes[1:4])

	// If you omit the 'low index', it infers that and assumes 0 in the
	// place. Since it does the hi-1 for the second part, then [:3] will go
	// from 0 to 2.
	fmt.Println(primes[:3])

	// [low:low] evaluates to an empty slide. No matter index you use. :(
	fmt.Println(primes[2:2])
}
