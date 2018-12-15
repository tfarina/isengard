package main

import (
	"flag"
	"fmt"
	"html/template"
	"log"
	"net/http"
	"path/filepath"
	"runtime"
	"strconv"
)

var tmpl *template.Template

func renderTemplate(w http.ResponseWriter) {
	err := tmpl.ExecuteTemplate(w, "index.html", nil)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func rootHandler(w http.ResponseWriter, r *http.Request) {
	renderTemplate(w)
}

func main() {
	http_port_int := flag.Int("http-port", 8001, "The HTTP port to listen on.")
	resources_dir := flag.String("resources-dir", "", "The directory to find templates, JS and CSS files")

	flag.Parse()

	if *http_port_int < 1 || *http_port_int > 65535 {
		fmt.Println("Port numbers must be between 1 and 65535, includesive.")
		return
	}

	http_port := strconv.Itoa(*http_port_int)

	if *resources_dir == "" {
		_, filename, _, _ := runtime.Caller(0)
		*resources_dir = filepath.Dir(filename)
	}
	fmt.Println(*resources_dir)
	tmpl = template.Must(template.ParseFiles(filepath.Join(*resources_dir, "index.html")))

	fmt.Println("Listening on port " + http_port)

	http.HandleFunc("/", rootHandler)
	log.Fatal(http.ListenAndServe("0.0.0.0:"+http_port, nil))
}
