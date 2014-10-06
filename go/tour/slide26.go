package main

import "fmt"

type Vertex struct {
	X int
	Y int
}

//
// Structs
//
// A struct is a collection of fields.
//
// (And a type declaration does what you'd expect.)
//
func main() {
	fmt.Println(Vertex{1, 2})
}
