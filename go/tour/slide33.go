package main

import "fmt"

func main() {
	// Slice
	primes := []int{2, 3, 5, 7, 11}

	fmt.Println(primes)

	// Remember that it is [lo:hi], but it evaluates to [lo:hi-1].
	fmt.Println("[1:4] is: ", primes[1:4])

	// If you omit the 'low index', it infers that and assumes 0 in the
	// place. Since it does the hi-1 for the second part, then [:3] will go
	// from 0 to 2.
	fmt.Println(primes[:3])

	// [low:low] evaluates to an empty slide. No matter index you use. :(
	fmt.Println(primes[2:2])

	// If you omit the 'high index', it will assume len(primes).
	// So in this case, len(primes) is equal to 5, since there are 5
	// elements in this slice.
	// With that [3:] will evaluate to [3:5-1], which is [3:4], and will
	// print 7, 11.
	fmt.Println(primes[3:])

        // The slice [low:low+1] will evalute to ONE element.
	fmt.Println(primes[2:3])
}
