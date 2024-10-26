package handlers

import (
	"encoding/json"
	"log"
	"net/http"
)

// helloAPI handles requests to the /api/hello endpoint
func HelloAPI(w http.ResponseWriter, r *http.Request) {
    // Set response header to application/json
    w.Header().Set("Content-Type", "application/json")

    // Create a response with a message field
    response := map[string]string{"message": "Hello from the Go backend!"}

    // Encode the response as JSON and send it
    json.NewEncoder(w).Encode(response)

	log.Printf("helloAPI returned message")
}
