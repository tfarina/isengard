package main

import "fmt"

//
// Mutating Maps
//
// Insert or update an element in map m:
//
// m[key] = elem
//
// Retrieve an element:
//
// elem = m[key]
//
// Delete an element:
//
// delete(m, key)
//
func main() {
	m := make(map[string]int)

	// Insert item.
	m["books"] = 17
	fmt.Println("How many books do you have?", m["books"])

	// Update its value.
	m["books"] = 2
	fmt.Println("Now, how many books do you have?", m["books"])

	// Delete an item from the map.
	delete(m, "books")
	fmt.Println("books element was deleted, hence it returns", m["books"])
}
