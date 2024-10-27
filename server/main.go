package main

import (
	"html/template"
	"log"
	"net/http"
	"path/filepath"

	"server/handlers"
)

func main() {
	// handles all API calls
	registerRoutes()

	log.Println("Starting server on http://127.0.0.1:8080")
    err := http.ListenAndServe(":8080", nil)
    if err != nil {
        log.Fatal("Server failed:", err)
    }
}

func registerRoutes() {
    fs := http.FileServer(http.Dir("../frontend/static"))
    http.Handle("/static/", http.StripPrefix("/static/", fs))

	// index page
	http.HandleFunc("/", homePage)

	// sample api call 
	http.HandleFunc("/api/hello", handlers.HelloAPI)
}

func homePage(w http.ResponseWriter, r *http.Request) {
    tmpl, err := template.ParseFiles(filepath.Join("../frontend/templates", "index.html"))
    if err != nil {
        http.Error(w, "Could not load template", http.StatusInternalServerError)
        return
    }

    // Render the template
    tmpl.Execute(w, nil)
}