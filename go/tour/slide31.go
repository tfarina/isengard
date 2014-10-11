package main

import "fmt"

func main() {
        var array[4]string

        array[0] = "Verizon"
        array[1] = "AT&T"
        array[2] = "Bell Labs"
        array[3] = "Vodafone"

        fmt.Println(array[0], array[2])
        fmt.Println()
        fmt.Println(array)
        fmt.Println()
        fmt.Println(array[1])
        fmt.Println(array[2])
        fmt.Println(array[0])
        fmt.Println(array[3])
}
