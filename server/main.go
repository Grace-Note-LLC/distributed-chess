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

// Define bitboards for each piece type
var (
    whitePawns   uint64
    blackPawns   uint64
    whiteRooks   uint64
    blackRooks   uint64
    whiteKnights uint64
    blackKnights uint64
    whiteBishops uint64
    blackBishops uint64
    whiteQueen   uint64
    blackQueen   uint64
    whiteKing    uint64
    blackKing    uint64
)

// Calculate bit index from board coordinates
func getBitIndex(x, y int) uint64 {
    return uint64(y*8 + x)
}

func updateBitboards(pieces []Piece) {
    // Reset all bitboards
    whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueen, whiteKing = 0, 0, 0, 0, 0, 0
    blackPawns, blackRooks, blackKnights, blackBishops, blackQueen, blackKing = 0, 0, 0, 0, 0, 0

    // Iterate over each piece and update the corresponding bitboard
    for _, piece := range pieces {
        bitIndex := getBitIndex(piece.X, piece.Y)
        bitMask := uint64(1) << bitIndex

        switch piece.Image {
        case "/images/pieces-basic-svg/pawn-b.svg":
            blackPawns |= bitMask
        case "/images/pieces-basic-svg/rook-b.svg":
            blackRooks |= bitMask
        case "/images/pieces-basic-svg/knight-b.svg":
            blackKnights |= bitMask
        case "/images/pieces-basic-svg/bishop-b.svg":
            blackBishops |= bitMask
        case "/images/pieces-basic-svg/queen-b.svg":
            blackQueen |= bitMask
        case "/images/pieces-basic-svg/king-b.svg":
            blackKing |= bitMask
        case "/images/pieces-basic-svg/pawn-w.svg":
            whitePawns |= bitMask
        case "/images/pieces-basic-svg/rook-w.svg":
            whiteRooks |= bitMask
        case "/images/pieces-basic-svg/knight-w.svg":
            whiteKnights |= bitMask
        case "/images/pieces-basic-svg/bishop-w.svg":
            whiteBishops |= bitMask
        case "/images/pieces-basic-svg/queen-w.svg":
            whiteQueen |= bitMask
        case "/images/pieces-basic-svg/king-w.svg":
            whiteKing |= bitMask
        }
    }
}

func printBitboards() {
    fmt.Printf("White Pawns:   0x%016X\n", whitePawns)
    fmt.Printf("Black Pawns:   0x%016X\n", blackPawns)
    fmt.Printf("White Rooks:   0x%016X\n", whiteRooks)
    fmt.Printf("Black Rooks:   0x%016X\n", blackRooks)
    fmt.Printf("White Knights: 0x%016X\n", whiteKnights)
    fmt.Printf("Black Knights: 0x%016X\n", blackKnights)
    fmt.Printf("White Bishops: 0x%016X\n", whiteBishops)
    fmt.Printf("Black Bishops: 0x%016X\n", blackBishops)
    fmt.Printf("White Queen:   0x%016X\n", whiteQueen)
    fmt.Printf("Black Queen:   0x%016X\n", blackQueen)
    fmt.Printf("White King:    0x%016X\n", whiteKing)
    fmt.Printf("Black King:    0x%016X\n", blackKing)
}

func boardStateHandler(w http.ResponseWriter, r *http.Request) {
    if r.Method == http.MethodOptions {
        enableCORS(w)
        return
    }
    if r.Method == http.MethodPost {
        enableCORS(w)

        var pieces []Piece
        err := json.NewDecoder(r.Body).Decode(&pieces)
        if err != nil {
            http.Error(w, err.Error(), http.StatusBadRequest)
            return
        }

        updateBitboards(pieces)
        printBitboards()
        
        w.Header().Set("Content-Type", "application/json")
        json.NewEncoder(w).Encode(map[string]string{"status": "success"})
    } else {
        http.Error(w, "Invalid request method", http.StatusMethodNotAllowed)
    }
}

func enableCORS(w http.ResponseWriter) {
    w.Header().Set("Access-Control-Allow-Origin", "*")
    w.Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS")
    w.Header().Set("Access-Control-Allow-Headers", "Content-Type")
}

func main() {
    http.HandleFunc("/api/board-state", boardStateHandler)
    fmt.Println("Server is running on port 8080...")
    http.ListenAndServe(":8080", nil)
}
