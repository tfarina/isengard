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
// Test that a key is present with a two-value assignment:
//
// elem, ok = m[key]
//
func main() {
	m := make(map[string]int)

	// Insert item.
	m["books"] = 17
	fmt.Println("How many books do you have?", m["books"])

	// Update its value.
	m["books"] = 2
	fmt.Println("Now, how many books do you have?", m["books"])

	// Check if a key is present in the map.
	value, is_present := m["books"]
	fmt.Println(value, is_present)

	// Delete an item from the map.
	delete(m, "books")
	fmt.Println("books element was deleted, hence it returns", m["books"])
}
