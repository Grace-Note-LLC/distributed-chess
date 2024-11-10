#include <tuple>
#include <bitset>

#include "Move.h"
#include "Board.h"
#include "Utils.h"

#include <iostream>
using namespace std;

const int WHITE_PAWN_DOUBLE_RANK = 1;
const int BLACK_PAWN_DOUBLE_RANK = 6;

Move::Move(Board::PieceIndex pieceType, uint64_t newPosition, uint64_t oldPosition, bool isCapture, Board *prevBoard) {
    
    this->newPosition = newPosition;
    this->oldPosition = oldPosition;
    this->pieceType = pieceType;
    this->capture = isCapture;
    this->prevBoard = prevBoard;
}

std::vector<Move> MoveGenerator::generateAllMoves(tileState color) {
    std::vector<Move> moves;
    const Board::PieceIndex* pieceIndices = this->board->getPieceIndices();
    for (int i = color; i < 12; i += 2) {
        auto pieceType = pieceIndices[i];
        auto pieceMoves = generatePieceMoves(pieceType);
        moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
    }
    return moves;
}

std::vector<Move> MoveGenerator::generatePieceMoves(Board::PieceIndex pieceType){
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

void MoveGenerator::undoMove(const Move& move) {
    // Implement logic to revert the board state to before the given move
}

void MoveGenerator::generatePawnDiagonalCaptures(
    std::vector<Move>& moves, 
    Board::PieceIndex pieceType, 
    uint64_t oldPosition,
    int newRank, 
    int newFile) {
    
    if (isOnBoard(newRank, newFile)) {
        uint64_t proposedMove = gridToBinIdx(newRank, newFile);
        auto color = static_cast<tileState>(pieceType % 2);
        auto occupant = getOccupant(pieceType, proposedMove);
        if (occupant != color && occupant != EMPTY) {
            moves.push_back(Move(pieceType, proposedMove, oldPosition, true, this->board));
        }
    }
}

std::vector<Move> MoveGenerator::generatePawnMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = this->board->getPiece(pieceType);
    auto color = static_cast<tileState>(pieceType % 2);    
    int singleOffset           = (color == WHITE) ? whitePawnSingleOffset   : blackPawnSingleOffset;
    int doubleOffset           = (color == WHITE) ? whitePawnDoubleOffset   : blackPawnDoubleOffset;
    const auto& captureOffsets = (color == WHITE) ? whitePawnCaptureOffsets : blackPawnCaptureOffsets;
    int startRank              = (color == WHITE) ? WHITE_PAWN_DOUBLE_RANK  : BLACK_PAWN_DOUBLE_RANK;

    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        auto oldPosition = gridToBinIdx(rank, file);
        // Check for pawn 2x move
        tileState front = getOccupant(pieceType, gridToBinIdx(rank + singleOffset, file));
        if (front == EMPTY) {
            if (rank == startRank)
                addMoveIfValid(moves, pieceType, oldPosition, rank + doubleOffset, file);
            addMoveIfValid(moves, pieceType, oldPosition, rank + singleOffset, file);
        }

        // Check for diagonal captures
        for (const auto& os : captureOffsets)
            generatePawnDiagonalCaptures(moves, pieceType, oldPosition, rank + os.first, file + os.second);
        piece ^= gridToBinIdx(rank, file);
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
        auto oldPosition = gridToBinIdx(rank, file);

        for (const auto& cardinal : rookOffsets) {
            for (const auto& os : cardinal) {
                if (!isOnBoard(rank + os.first, file + os.second)) { break; }

                tileState next = getOccupant(pieceType, gridToBinIdx(rank + os.first, file + os.second));
                if (color == next) { break; }
                if (color != next && next != EMPTY) {
                    addMoveIfValid(moves, pieceType, oldPosition, rank + os.first, file + os.second);
                    break;
                }
                addMoveIfValid(moves, pieceType, oldPosition, rank + os.first, file + os.second);
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
        auto oldPosition = gridToBinIdx(rank, file);

        for (const auto& os : knightOffsets)
            addMoveIfValid(moves, pieceType, oldPosition, rank + os.first, file + os.second);
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
        auto oldPosition = gridToBinIdx(rank, file);

        for (const auto& diagonal : bishopOffsets) {
            for (const auto& os : diagonal) {
                if (!isOnBoard(rank + os.first, file + os.second)) { break; }

                tileState next = getOccupant(pieceType, gridToBinIdx(rank + os.first, file + os.second));
                // cout << "color: " << color << "|" << "next: " << next << endl;
                if (color == next) { break; }
                if (color != next && next != EMPTY) {
                    addMoveIfValid(moves, pieceType, oldPosition, rank + os.first, file + os.second);
                    break;
                }
                addMoveIfValid(moves, pieceType, oldPosition, rank + os.first, file + os.second);
            }
        }
        piece ^= gridToBinIdx(rank, file);
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = this->board->getPiece(pieceType);
    auto color = static_cast<tileState>(pieceType % 2);
    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        auto oldPosition = gridToBinIdx(rank, file);
        for (const auto& lineOfSight : queenOffsets) {
            for (const auto& os : lineOfSight) {
                if (!isOnBoard(rank + os.first, file + os.second)) { break; }

                tileState next = getOccupant(pieceType, gridToBinIdx(rank + os.first, file + os.second));
                if (color == next) { break; }
                if (color != next && next != EMPTY) {
                    addMoveIfValid(moves, pieceType, oldPosition, rank + os.first, file + os.second);
                    break;
                }
                addMoveIfValid(moves, pieceType, oldPosition, rank + os.first, file + os.second);
            }
        }
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
        addMoveIfValid(moves, pieceType, piece, rank + os.first, file + os.second);
    }
    return moves;
}

tileState MoveGenerator::getOccupant(Board::PieceIndex pieceType, uint64_t proposedMove) {
    auto pieces = this->board->getPieces();
    for (int i = 0; i < 12; i++) {
        // if (i == pieceType) { continue; }
        uint64_t piece = pieces[i];
        if (piece == 0ULL) { continue; }
        if ((proposedMove & piece) > 0) return i % 2 == 0 ? WHITE : BLACK;
    }
    return EMPTY;
}

void MoveGenerator::addMoveIfValid(
    std::vector<Move>& moves, 
    Board::PieceIndex pieceType, 
    uint64_t oldPosition,
    int newRank, 
    int newFile) {
    
    tileState pieceColor = static_cast<tileState>(pieceType % 2);
    if (isOnBoard(newRank, newFile)) {
        uint64_t proposedMove = gridToBinIdx(newRank, newFile);        
        auto occupant = getOccupant(pieceType, proposedMove);
        if (occupant == pieceColor) { return; }
        moves.push_back(
            Move(pieceType, proposedMove, oldPosition, occupant != EMPTY, this->board));
    }
}

bool MoveGenerator::isInCheck(tileState color) {
    // Find the position of the king of the given color
    uint64_t kingPosition = board->getPiece((color == WHITE) ? Board::WHITE_KING : Board::BLACK_KING);

    // Generate all possible moves for the opponent
    tileState opponentColor = (color == WHITE) ? BLACK : WHITE;
    std::vector<Move> opponentMoves = this->generateAllMoves(opponentColor);

    // Check if any of the opponent's moves can capture the king
    for (const auto& move : opponentMoves) {
        if (move.getNewPosition() == kingPosition) {
            return true;
        }
    }
    return false;
}

bool MoveGenerator::isCheckmate(tileState color) {
    // Generate all possible moves for the given color
    std::vector<Move> allMoves = this->generateAllMoves(color);

    // Check if any of the moves result in a non-check position
    for (const auto& move : allMoves) {
        // Apply the move to a copy of the board
        Board copy = *this->board;
        copy.applyMove(move);

        // Check if the king is still in check
        if (isInCheck(color)) {
            return false;
        }
    }
    return true;
}

/*
Check if the game is over.
*/
bool MoveGenerator::isGameOver() {
    return false;
}