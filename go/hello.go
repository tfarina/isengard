// $ go build -gcflags=-S hello.go

package main

import (
	"fmt"
	"log"
	"net/http"
	"os"
)

func handleRoot(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(os.Stdout, "%s details: %+v\n", r.URL.Path, r)
	fmt.Fprintf(w, "Hello, world! at %s\n", r.URL.Path)
}

func main() {
	log.Printf("Server is running at http://127.0.0.1:8080")
	http.HandleFunc("/", handleRoot)
	log.Fatal(http.ListenAndServe("127.0.0.1:8080", nil))
}
