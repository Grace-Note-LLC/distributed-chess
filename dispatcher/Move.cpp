#include <tuple>
#include <bitset>

#include "Move.h"
#include "Board.h"
#include "Utils.h"

#include <iostream>
using namespace std;

const uint64_t A1 = 0x8000000000000000;

Move::Move(Board::PieceIndex pieceType, uint64_t newPosition, bool isCapture, Board *prevBoard) {
    this->newPosition = newPosition;
    this->pieceType = pieceType;
    this->capture = isCapture;
    this->prevBoard = prevBoard;
}

std::vector<Move> MoveGenerator::generatePieceMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    switch (pieceType) {
        case Board::WHITE_PAWNS:
        case Board::BLACK_PAWNS:
            moves = generatePawnMoves(pieceType);
            break;
        case Board::WHITE_ROOKS:
        case Board::BLACK_ROOKS:
            moves = generateRookMoves(pieceType);
            break;
        case Board::WHITE_KNIGHTS:
        case Board::BLACK_KNIGHTS:
            moves = generateKnightMoves(pieceType);
            break;
        case Board::WHITE_BISHOPS:
        case Board::BLACK_BISHOPS:
            moves = generateBishopMoves(pieceType);
            break;
        case Board::WHITE_QUEEN:
        case Board::BLACK_QUEEN:
            moves = generateQueenMoves(pieceType);
            break;
        case Board::WHITE_KING:
        case Board::BLACK_KING:
            moves = generateKingMoves(pieceType);
            break;
        default:
            break;
    }
    return moves;
}

bool MoveGenerator::isValidMove(const Move& move) {
    // Implement validation logic based on board state and piece rules
    return true; // Placeholder
}

void MoveGenerator::applyMove(const Move& move) {
    // Implement logic to update the board state with the given move
}

void MoveGenerator::undoMove(const Move& move) {
    // Implement logic to revert the board state to before the given move
}

void MoveGenerator::generatePawnDiagonalCaptures(
    std::vector<Move>& moves, 
    Board::PieceIndex pieceType, 
    int rank, 
    int file) {
    
    if (rank >= 0 && rank < 8 && file >= 0 && file < 8) {
        uint64_t proposedMove = gridToBinIdx(rank, file);
        auto color = static_cast<tileState>(pieceType % 2);
        auto occupant = getOccupant(pieceType, proposedMove);
        if (occupant != color && occupant != EMPTY) {
            moves.push_back(Move(pieceType, proposedMove, true, this->board));
        }
    }
}

std::vector<Move> MoveGenerator::generatePawnMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = this->board->getPiece(pieceType);
    auto color = static_cast<tileState>(pieceType % 2);    
    if (color == WHITE) {
        while (piece > 0) {
            auto rankfile = binIdxToGrid(piece);
            int rank = std::get<0>(rankfile);
            int file = std::get<1>(rankfile);
            // Check for pawn 2x move
            tileState front = getOccupant(pieceType, gridToBinIdx(rank + 1, file));
            if (front == EMPTY){
                if (rank == 1)
                    addMoveIfValid(moves, pieceType, rank + whitePawnDoubleOffset, file);
                addMoveIfValid(moves, pieceType, rank + whitePawnSingleOffset, file);
            }

            // Check for diagonal captures
            for (const auto& os : whitePawnCaptureOffsets) {
                generatePawnDiagonalCaptures(moves, pieceType, rank + os.first, file + os.second);
            }
            piece ^= gridToBinIdx(rank, file);
        }
    } else if (color == BLACK) {
        while (piece > 0) {
            auto rankfile = binIdxToGrid(piece);
            int rank = std::get<0>(rankfile);
            int file = std::get<1>(rankfile);
            // Check for pawn 2x move
            if (rank == 6) 
                addMoveIfValid(moves, pieceType, rank + blackPawnDoubleOffset, file);

            // Pawn will always have the option to move forward
            addMoveIfValid(moves, pieceType, rank + blackPawnSingleOffset, file);

            // Check for diagonal captures
            for (const auto& os : blackPawnCaptureOffsets) {
                generatePawnDiagonalCaptures(moves, pieceType, rank + os.first, file + os.second);
            }
            piece ^= gridToBinIdx(rank, file);
        }
    } 
    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = this->board->getPiece(pieceType);
    auto color = static_cast<tileState>(pieceType % 2);
    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        for (const auto& cardinal : rookOffsets) {
            for (const auto& os : cardinal) {
                if (!isOnBoard(rank + os.first, file + os.second)) { break; }

                tileState next = getOccupant(pieceType, gridToBinIdx(rank + os.first, file + os.second));
                if (color == next) { break; }
                if (color != next && next != EMPTY) {
                    addMoveIfValid(moves, pieceType, rank + os.first, file + os.second);
                    break;
                }
                addMoveIfValid(moves, pieceType, rank + os.first, file + os.second);
            }
        }
        piece ^= gridToBinIdx(rank, file);
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateKnightMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = this->board->getPiece(pieceType);
    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        for (const auto& os : knightOffsets)
            addMoveIfValid(moves, pieceType, rank + os.first, file + os.second);
        piece ^= gridToBinIdx(rank, file);
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateBishopMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = this->board->getPiece(pieceType);
    auto color = static_cast<tileState>(pieceType % 2);
    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        for (const auto& diagonal : bishopOffsets) {
            for (const auto& os : diagonal) {
                if (!isOnBoard(rank + os.first, file + os.second)) { break; }

                tileState next = getOccupant(pieceType, gridToBinIdx(rank + os.first, file + os.second));
                // cout << "color: " << color << "|" << "next: " << next << endl;
                if (color == next) { break; }
                if (color != next && next != EMPTY) {
                    addMoveIfValid(moves, pieceType, rank + os.first, file + os.second);
                    break;
                }
                addMoveIfValid(moves, pieceType, rank + os.first, file + os.second);
            }
        }
        piece ^= gridToBinIdx(rank, file);
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = this->board->getPiece(pieceType);
    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        for (const auto& os : queenOffsets)
            addMoveIfValid(moves, pieceType, rank + os.first, file + os.second);
        piece ^= gridToBinIdx(rank, file);
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateKingMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = this->board->getPiece(pieceType);
    auto rankfile = binIdxToGrid(piece);
    int rank = std::get<0>(rankfile);
    int file = std::get<1>(rankfile);
    for (const auto& os : kingOffsets) {
        addMoveIfValid(moves, pieceType, rank + os.first, file + os.second);
    }
    return moves;
}

MoveGenerator::tileState MoveGenerator::getOccupant(Board::PieceIndex pieceType, uint64_t proposedMove) {
    auto pieces = this->board->getPieces();
    for (int i = 0; i < 12; i++) {
        // if (i == pieceType) { continue; }
        uint64_t piece = pieces[i];
        if (piece == 0ULL) { continue; }

        if ((proposedMove & piece) > 0) {
            // auto piec = binIdxToGrid(piece);
            // cout << std::get<0>(piec) << "," << std::get<1>(piec) << endl;
            // cout << "piece collision:" << i << endl;
            return i % 2 == 0 ? WHITE : BLACK;
        }
    }
    return MoveGenerator::EMPTY;
}

void MoveGenerator::addMoveIfValid(
    std::vector<Move>& moves, 
    Board::PieceIndex pieceType, 
    int newRank, 
    int newFile) {
    
    // cout << "Rank: " << rank << " File: " << file << endl;
    // cout << piece << endl;
    tileState pieceColor = static_cast<tileState>(pieceType % 2);
    if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
        uint64_t proposedMove = gridToBinIdx(newRank, newFile);        
        auto occupant = getOccupant(pieceType, proposedMove);
        if (occupant == pieceColor) { return; }
        // cout << "here!" << endl;
        moves.push_back(Move(pieceType, proposedMove, occupant != EMPTY, this->board));
    }
}