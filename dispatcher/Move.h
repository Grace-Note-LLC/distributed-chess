/*
Methods for piece movement.
*/
#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <iostream>
#include <vector>

#include "Board.h"  // Include Board for accessing board state
#include "Utils.h"  

using namespace std;

class Board;  // Forward declaration of Board class

class Move {
public:
    Move(
        Board::PieceIndex pieceType, uint64_t newPosition, uint64_t oldPosition,bool isCapture);
    Move() {}

    Board::PieceIndex getPieceType() const { return pieceType; }
    bool isCapture()                 const { return capture; }
    uint64_t getNewPosition()        const { return newPosition; }
    uint64_t getOldPosition()        const { return oldPosition; }
    void print() {
        auto move = binIdxToGrid(newPosition);
        cout << "Piece type: " << pieceType << endl;
        cout << "Capture: " << capture << endl;
        cout << "New position: " << "(" << get<0>(move) << "," << get<1>(move) << ")" << endl;
    }

private:
    uint64_t newPosition;        // New position of the piece
    uint64_t oldPosition;        // Old position of the piece
    bool capture;                // Whether this move captures an opponent's piece
    Board::PieceIndex pieceType; // Type of piece being moved
};

class MoveGenerator {
public:
    MoveGenerator() {}

    // Generate all legal WHITE moves for the current board state
    std::vector<Move> generateAllMoves(Board board, tileState color);

    // Generate moves for a specific piece type
    std::vector<Move> generatePieceMoves(Board* board, Board::PieceIndex pieceType);

    // Check if a move is valid based on board state and piece rules
    bool isValidMove(const Move& move);

    tileState getOccupant(Board* board, Board::PieceIndex, uint64_t proposedMove);

    bool isInCheck(Board board, tileState color);
    bool isCheckmate(Board* board, tileState color);
    bool isGameOver(Board* board);

private:
    // Board* board;  // Pointer to the board object
   
    // Possible moves for a KING (relative to current position)
    const std::pair<int, int> kingOffsets[8] = {
        {1, -1}, {1,  0}, {1,  1},
        {0, -1},          {0,  1},
        {-1,-1}, {-1, 0}, {-1, 1}
    };

    // Possible moves for a BISHOP (relative to current position)
    const std::pair<int, int> bishopOffsets[4][7] = {
        // Up-Right (positive x, positive y)
        { {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7} },
        // Up-Left (positive x, negative y)
        { {1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7} },
        // Down-Right (negative x, positive y)
        { {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7} },
        // Down-Left (negative x, negative y)
        { {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7} }
    };

    const std::pair<int, int> rookOffsets[4][7] = {
        // Right (positive x)
        { {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0} },
        // Left (negative x)
        { {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0} },
        // Up (positive y)
        { {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7} },
        // Down (negative y)
        { {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7} }
    };

    const std::pair<int, int> knightOffsets[8] = {
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
    };
    
    const std::pair<int, int> queenOffsets[8][7] = {
        // Combine rook and bishop offsets for queen
        // Up-Right (positive x, positive y)
        { {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7} },
        // Up-Left (positive x, negative y)
        { {1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7} },
        // Down-Right (negative x, positive y)
        { {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7} },
        // Down-Left (negative x, negative y)
        { {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7} },
        // Right (positive x)
        { {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0} },
        // Left (negative x)
        { {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0} },
        // Up (positive y)
        { {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7} },
        // Down (negative y)
        { {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7} }
    };
    
    // Pawn offsets for white and black pawns
    const int whitePawnSingleOffset = 1;
    const int whitePawnDoubleOffset = 2;
    const std::pair<int, int> whitePawnCaptureOffsets[2] = {{1, 1}, {1, -1}};
    const int blackPawnSingleOffset = -1;
    const int blackPawnDoubleOffset = -2;
    const std::pair<int, int> blackPawnCaptureOffsets[2] = {{-1, 1}, {-1, -1}};

    // Helper functions to generate moves for each piece type
    std::vector<Move> generatePawnMoves(Board* board, Board::PieceIndex pieceType);
    std::vector<Move> generateRookMoves(Board* board, Board::PieceIndex pieceType);
    std::vector<Move> generateKnightMoves(Board* board, Board::PieceIndex pieceType);
    std::vector<Move> generateBishopMoves(Board* board, Board::PieceIndex pieceType);
    std::vector<Move> generateQueenMoves(Board* board, Board::PieceIndex pieceType);
    std::vector<Move> generateKingMoves(Board* board, Board::PieceIndex pieceType);

    // Helper functions for move validation
    void addMoveIfValid(
        std::vector<Move>& moves, 
        Board* board,
        Board::PieceIndex pieceType, 
        uint64_t oldPosition,
        int newRank, 
        int newFile
    );
    void generatePawnDiagonalCaptures(
        std::vector<Move>& moves, 
        Board* board,
        Board::PieceIndex pieceType,
        uint64_t oldPosition,
        int rank, 
        int file
    );

};

#endif // MOVE_H
