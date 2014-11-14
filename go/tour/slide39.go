package main

import "fmt"

type Location struct {
	Lat  float64
	Long float64
}

//
// Maps
//
// A map maps keys to values.
//
// Maps must be created with make (not new) before use; the nil map is empty and
// cannot be assigned to.
//
func main() {
	m := make(map[string]Location)
	m["Bell Labs"] = Location{
		40.68433, -74.39967,
	}
	fmt.Println(m["Bell Labs"])
}
