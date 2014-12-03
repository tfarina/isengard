// tour.golang.org/#47

package main

import (
	"fmt"
	"runtime"
)

//
// Switch
//
// You probably knew what switch was going to look like.
//
// A case body breaks automatically, unless it ends with a fallthrough
// statement.
//
func main() {
	fmt.Print("Go is running on ")
	switch os := runtime.GOOS; os {
	case "darwin":
		fmt.Println("OS X.")
	case "linux":
		fmt.Println("Linux.")
	default:
		// freebsd, openbsd,
		// plan9, windows...
		fmt.Printf("%s.", os)
	}
}
