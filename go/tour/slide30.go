package main

import "fmt"

type Vertex struct {
        X, Y int
}

//
// The new function:
//
// The expression new(T) allocates a zeroed T value and returns a pointer to it.
//
// var t *T = new(T)
//
// or
//
// t := new(T)
//
func main() {
        var u *Vertex = new(Vertex)
        fmt.Println(u.X)
        fmt.Println(u.Y)
        fmt.Println(u)

        v := new(Vertex)
        v.X = 4
        v.Y = 2
        fmt.Println(v.X)
        fmt.Println(v.Y)
        fmt.Println(v)
}
