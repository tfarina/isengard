// go run handle_func.go
//
// 1) Open Chrome
// 2) Navigate to localhost:8080
// 3) See the result

package main

import "net/http"
import "fmt"

func rootHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, "<html><body><h2>Hello, www!</h2><a href=\"http://google.com\">Google</a></body></html>")
}

func main() {
	http.HandleFunc("/", rootHandler)
	http.ListenAndServe(":8080", nil)
}
