/*
Methods for piece movement.
*/
#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include "Board.h"  // Include Board for accessing board state

class Move {
public:
    Move(int start, int end, Board::PieceIndex pieceType, bool isCapture = false);

    int getStart() const { return startSquare; }
    int getEnd() const { return endSquare; }
    Board::PieceIndex getPieceType() const { return pieceType; }
    bool isCapture() const { return capture; }

private:
    int startSquare;             // Starting square of the move (0-63)
    int endSquare;               // Ending square of the move (0-63)
    bool capture;                // Whether this move captures an opponent's piece
    Board::PieceIndex pieceType; // Type of piece being moved
    Board* prevBoard;            // Previous board state
};

class MoveGenerator {
public:
    MoveGenerator(Board* board) { this->board = board; }

    // Generate all legal moves for the current board state
    std::vector<Move> generateAllMoves();

    // Generate moves for a specific piece type
    std::vector<Move> generatePieceMoves(Board::PieceIndex pieceType);

    // Check if a move is valid based on board state and piece rules
    bool isValidMove(const Move& move);

    // Apply a move to the board (updates the board state)
    void applyMove(const Move& move);

    // Undo a move on the board
    void undoMove(const Move& move);

private:
    Board* board;  // Pointer to the board object
    enum tileState {
        WHITE,
        BLACK,
        EMPTY
    };

    // Helper functions to generate moves for each piece type
    std::vector<Move> generatePawnMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateRookMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateKnightMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateBishopMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateQueenMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateKingMoves(Board::PieceIndex pieceType);

    // Helper functions for move validation
    tileState getOccupant(Board::PieceIndex, uint64_t proposedMove);
    
    // (row, column)
    std::tuple<int, int> binIdxToGrid(uint64_t bin) { return std::make_tuple(bin / 8, bin % 8); }
    uint64_t GridToBinIdx(std::tuple<int> twoDIndex){
        return std::get<0>(twoDIndex) * 8 + std::get<1>(twoDIndex);
    };

};

#endif // MOVE_H
