package main

import "fmt"

type Vertex struct {
	X int
	Y int
}

//
// Struct Fields
//
// Struct fields are accessed using a dot.
//
func main() {
	v := Vertex{1, 2}
	v.X = 4
	fmt.Println(v.X)
}
