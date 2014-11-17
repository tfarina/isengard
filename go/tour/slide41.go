package main

import "fmt"

type Location struct {
	Lat  float64
	Long float64
}

var m = map[string]Location{
	"Bell Labs": {40.68433, -74.39967},
	"Google":    {37.42202, -122.08408},
}

//
// Map literals continued
//
// If the top-level type is just a type name, you can omit it from the
// elements of the literal.
//
func main() {
	fmt.Println(m)
}
