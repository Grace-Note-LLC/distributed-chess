#include <tuple>

#include "Move.h"
#include "Board.h"

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

std::vector<Move> MoveGenerator::generatePawnMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement pawn move generation logic
    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement rook move generation logic
    return moves;
}

std::vector<Move> MoveGenerator::generateKnightMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement knight move generation logic
    return moves;
}

std::vector<Move> MoveGenerator::generateBishopMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement bishop move generation logic
    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement queen move generation logic
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
    for (int i = 0; i < 12; i++) {
        if (i == pieceType) { continue; }
        uint64_t piece = this->board->getPieces()->at(i);
        if ((proposedMove & piece)) {
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
        uint64_t proposedMove = A1 >> (newRank * 8 + newFile);        
        auto occupant = getOccupant(pieceType, proposedMove);
        if (occupant == pieceColor) { return; }
        // cout << "here!" << endl;
        moves.push_back(Move(pieceType, proposedMove, occupant != EMPTY, this->board));
    }
}

/*
Converts a binary index to a grid index (rank, file).

Function `__builtin_ctzll` counts the number of trailing 
zeros in a 64-bit integer. This is used to determine the
index of the piece in the board.
*/
std::tuple<int, int> MoveGenerator::binIdxToGrid(uint64_t bin) {
    int index = __builtin_ctzll(bin); 
    return std::make_tuple(index / 8ULL, index % 8ULL);
}

uint64_t MoveGenerator::gridToBinIdx(std::tuple<int, int> twoDIndex) {
    return gridToBinIdx(std::get<0>(twoDIndex), std::get<1>(twoDIndex));
}

uint64_t MoveGenerator::gridToBinIdx(int rank, int file) {
    return rank * 8 + file;
}