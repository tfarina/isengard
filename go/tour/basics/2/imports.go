// http://tour.golang.org/basics/2

package main

import (
	"fmt"
	"math"
)

func main() {
	fmt.Printf("Now you have %g problems.", math.Nextafter(2, 3))
}
