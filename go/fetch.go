package main

import (
	"io/ioutil"
	"log"
	"net/http"
)

func main() {
	log.Println("http://google.com")
	resp, err := http.Get("https://www.google.com/")
	if err != nil {
		// handle error
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	log.Println(body)
}
