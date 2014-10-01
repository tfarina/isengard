package main

import "fmt"

// Short variable declarations:
//
// Inside a function, the := short assignment statement can be used in place of
// a var declaration with implicit type.
//
// Outside a function, every construct begins with a keyword (var, func, and so
// on) and the := construct is not available.
func main() {
	var i, j int = 1, 2
	k := 3
	c, python, java := true, false, "no!"

	fmt.Println(i, j, k, c, python, java)
}
