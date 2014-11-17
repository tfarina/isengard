package main

import "fmt"

type Location struct {
	Lat  float64
	Long float64
}

var m = map[string]Location{
	"Bell Labs": Location{
		40.68433, -74.39967,
	},
	"Google": Location{
		37.42202, -122.08408,
	},
}

func main() {
	fmt.Println(m)
}
