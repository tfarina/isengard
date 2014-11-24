// tour.golang.org/#44

package main

import (
	"fmt"
	"math"
)

//
// Function values
//
// Functions are values too.
//
func main() {
	hypotenusa := func(x, y float64) float64 {
		return math.Sqrt(x*x + y*y)
	}

	fmt.Println(hypotenusa(3, 4))
}
