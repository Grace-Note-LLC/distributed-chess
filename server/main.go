package main

import (
	"encoding/json"
	"fmt"
	"net/http"
)

type Piece struct {
	Image string `json:"image"`
	X     int    `json:"x"`
	Y     int    `json:"y"`
}

func enableCORS(w http.ResponseWriter) {
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS")
	w.Header().Set("Access-Control-Allow-Headers", "Content-Type")
}

func boardStateHandler(w http.ResponseWriter, r *http.Request) {
	enableCORS(w)
	if r.Method == http.MethodOptions {
		return
	}

	if r.Method == http.MethodPost {
		var pieces []Piece
		err := json.NewDecoder(r.Body).Decode(&pieces)
		if err != nil {
			http.Error(w, err.Error(), http.StatusBadRequest)
			return
		}

		// Print out the board state
		fmt.Println("Received board state:")
		for _, piece := range pieces {
			fmt.Printf("Piece: %s at (%d, %d)\n", piece.Image, piece.X, piece.Y)
		}

		// Respond to the client
		w.Header().Set("Content-Type", "application/json")
		json.NewEncoder(w).Encode(map[string]string{"status": "success"})
	} else {
		http.Error(w, "Invalid request method", http.StatusMethodNotAllowed)
	}
}

func main() {
	http.HandleFunc("/api/board-state", boardStateHandler)
	fmt.Println("Server is running on port 8080...")
	http.ListenAndServe(":8080", nil)
}
