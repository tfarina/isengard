package main

import (
	"flag"
	"fmt"
	"html/template"
	"log"
	"net/http"
	"strconv"
)

var tmpl = template.Must(template.New("tmpl").Parse(`
<!doctype html><html><body><h2>Hello, www!</h2><a href="https://duckduckgo.com">DuckDuckGo</a></body></html>`))

func renderTemplate(w http.ResponseWriter) {
	err := tmpl.Execute(w, nil)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func rootHandler(w http.ResponseWriter, r *http.Request) {
	renderTemplate(w)
}

func main() {
	http_port_int := flag.Int("http-port", 8001, "The HTTP port to listen on.")

	flag.Parse()

	if *http_port_int < 1 || *http_port_int > 65535 {
		fmt.Println("Port numbers must be between 1 and 65535, includesive.")
		return
	}

	http_port := strconv.Itoa(*http_port_int)

	fmt.Println("Listening on port " + http_port)

	http.HandleFunc("/", rootHandler)
	log.Fatal(http.ListenAndServe("0.0.0.0:"+http_port, nil))
}
