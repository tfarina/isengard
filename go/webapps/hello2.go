// 1) go run hello2.go
// 2) Open Chrome
// 3) Navigate to localhost:8001
// 4) See the result

package main

import (
	"fmt"
	"log"
	"net/http"
)

func rootHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, "<html><body><h2>Hello, www!</h2><a href=\"http://google.com\">Google</a></body></html>")
}

func main() {
	http.HandleFunc("/", rootHandler)
	log.Fatal(http.ListenAndServe("0.0.0.0:8001", nil))
}
