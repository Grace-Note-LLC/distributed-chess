/*
Methods for piece movement.
*/
#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include "Board.h"  // Include Board for accessing board state

class Move {
public:
    Move(Board::PieceIndex pieceType, uint64_t newPosition, bool isCapture, Board* prevBoard);

    Board::PieceIndex getPieceType() const { return pieceType; }
    bool isCapture()                 const { return capture; }
    Board* getPrevBoard()            const { return prevBoard; }
    uint64_t getNewPosition()        const { return newPosition; }

private:
    uint64_t newPosition;        // New position of the piece
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

    // (row, column)
    std::tuple<int, int> binIdxToGrid(uint64_t bin);
    uint64_t gridToBinIdx(std::tuple<int, int> twoDIndex);
    uint64_t gridToBinIdx(int rank, int file);

private:
    Board* board;  // Pointer to the board object
    enum tileState {
        WHITE,
        BLACK,
        EMPTY
    };

    // Possible moves for a KING (relative to current position)
    std::pair<int, int> kingOffsets[8] = {
        {1, -1}, {1,  0}, {1,  1},
        {0, -1},          {0,  1},
        {-1,-1}, {-1, 0}, {-1, 1}
    };

    // Possible moves for a BISHOP (relative to current position)
    std::pair<int, int> bishopOffsets[28] = {
        // Up-Right (positive x, positive y)
        {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7},
        // Up-Left (positive x, negative y)
        {1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7},
        // Down-Right (negative x, positive y)
        {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7},
        // Down-Left (negative x, negative y)
        {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}
    };

    std::pair<int, int> rookOffsets[28] = {
        // Right (positive x)
        {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0},
        // Left (negative x)
        {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0},
        // Up (positive y)
        {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
        // Down (negative y)
        {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}
    };

    std::pair<int, int> knightOffsets[8] = {
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
    };

    std::pair<int, int> queenOffsets[56] = {
        // Combine rook and bishop offsets for queen
        // Up-Right (positive x, positive y)
        {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7},
        // Up-Left (positive x, negative y)
        {1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7},
        // Down-Right (negative x, positive y)
        {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7},
        // Down-Left (negative x, negative y)
        {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7},
        // Right (positive x)
        {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0},
        // Left (negative x)
        {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0},
        // Up (positive y)
        {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
        // Down (negative y)
        {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}
    };
    
    // Pawn offsets for white and black pawns
    std::pair<int, int> whitePawnNormalOffset = {1, 0};
    std::pair<int, int> whitePawnDoubleOffset = {2, 0};
    std::pair<int, int> whitePawnCaptureOffsets[2] = {{1, 1}, {1, -1}};
    std::pair<int, int> blackPawnNormalOffset = {-1, 0};
    std::pair<int, int> blackPawnDoubleOffset = {-2, 0};
    std::pair<int, int> blackPawnCaptureOffsets[2] = {{-1, 1}, {-1, -1}};

    // Helper functions to generate moves for each piece type
    std::vector<Move> generatePawnMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateRookMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateKnightMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateBishopMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateQueenMoves(Board::PieceIndex pieceType);
    std::vector<Move> generateKingMoves(Board::PieceIndex pieceType);

    // Helper functions for move validation
    tileState getOccupant(Board::PieceIndex, uint64_t proposedMove);
    void addMoveIfValid(
        std::vector<Move>& moves, 
        Board::PieceIndex pieceType, 
        int newRank, 
        int newFile
    );
    
};

#endif // MOVE_H
