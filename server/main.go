package main

import (
	"html/template"
	"log"
	"net/http"
	"path/filepath"
)

func main() {

	// creates the file server
	fs := http.FileServer(http.Dir("./static"))
	http.Handle("/static/", http.StripPrefix("/static/", fs))


	// root URL
	http.HandleFunc("/", homePage)

	log.Println("Starting server on http://127.0.0.1:8080")
    err := http.ListenAndServe(":8080", nil)
    if err != nil {
        log.Fatal("Server failed:", err)
    }
}

func homePage(w http.ResponseWriter, r *http.Request) {
    tmpl, err := template.ParseFiles(filepath.Join("templates", "index.html"))
    if err != nil {
        http.Error(w, "Could not load template", http.StatusInternalServerError)
        return
    }

    // Render the template
    tmpl.Execute(w, nil)
}