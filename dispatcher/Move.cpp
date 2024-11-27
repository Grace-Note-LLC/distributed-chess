#include <tuple>
#include <bitset>

#include "Move.h"
#include "Board.h"
#include "Utils.h"

#include <iostream>
#include <algorithm>

#include <mutex>

using namespace std;

const int WHITE_PAWN_DOUBLE_RANK = 1;
const int BLACK_PAWN_DOUBLE_RANK = 6;

mutex printMutex;

Move::Move(Board::PieceIndex pieceType, uint64_t newPosition, uint64_t oldPosition, bool isCapture) {
    
    this->newPosition = newPosition;
    this->oldPosition = oldPosition;
    this->pieceType = pieceType;
    this->capture = isCapture;
}

std::vector<Move> MoveGenerator::generateAllMoves(Board board, tileState color) {
    std::vector<Move> moves;
    const Board::PieceIndex* pieceIndices = board.getPieceIndices();
    for (int i = color; i < 12; i += 2) {
        auto pieceType = pieceIndices[i];
        auto pieceMoves = generatePieceMoves(&board, pieceType);
        moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
    }
    return moves;
}

vector<Move> MoveGenerator::generateAndFilterMoves(Board* board, tileState player) {
    std::vector<Move> possibleMoves = generateAllMoves(*board, player);
    possibleMoves = removeKingTargetingMoves(possibleMoves, 
        board->getPiece((player == WHITE) ? Board::BLACK_KING : Board::WHITE_KING)
    );
    possibleMoves = removeMovesLeavingKingInCheck(board, possibleMoves, player);
    
    // Sort moves by captures first
    std::sort(possibleMoves.begin(), possibleMoves.end(), [](const Move& a, const Move& b) {
        return a.isCapture() && !b.isCapture();
    });
    return possibleMoves;
}

void MoveGenerator::printMoves(Board board, vector<Move> moves, tileState player) {
    lock_guard<mutex> lock(printMutex);
    cout << "CURRENT BOARD: " << endl;
    board.prettyPrint();
    cout << "Moves for " << (player ? "BLACK" : "WHITE") << " on the above board: " << endl;
    cout << "-----------" << endl;
    for (Move move : moves) {
        tuple<int, int> fromPos = binIdxToGrid(move.getOldPosition());
        tuple<int, int> toPos = binIdxToGrid(move.getNewPosition());

        cout << "\t(" << get<0>(fromPos) << ", " << get<1>(fromPos) << ") -> (" << get<0>(toPos) << ", " << get<1>(toPos) << ")" << endl; 
    }

    cout << "-----------" << endl;
}

std::vector<Move> MoveGenerator::generatePieceMoves(Board* board, Board::PieceIndex pieceType){
    std::vector<Move> moves;
    switch (pieceType) {
        case Board::WHITE_PAWNS:
        case Board::BLACK_PAWNS:
            moves = generatePawnMoves(board, pieceType);
            break;
        case Board::WHITE_ROOKS:
        case Board::BLACK_ROOKS:
            moves = generateRookMoves(board, pieceType);
            break;
        case Board::WHITE_KNIGHTS:
        case Board::BLACK_KNIGHTS:
            moves = generateKnightMoves(board, pieceType);
            break;
        case Board::WHITE_BISHOPS:
        case Board::BLACK_BISHOPS:
            moves = generateBishopMoves(board, pieceType);
            break;
        case Board::WHITE_QUEEN:
        case Board::BLACK_QUEEN:
            moves = generateQueenMoves(board, pieceType);
            break;
        case Board::WHITE_KING:
        case Board::BLACK_KING:
            moves = generateKingMoves(board, pieceType);
            break;
        default:
            break;
    }
    return moves;
}

/*
If the moves were generated on WHITE's turn, remove any moves that target 
the BLACK king and vice versa.
*/
vector<Move> MoveGenerator::removeKingTargetingMoves(const vector<Move>& moves, uint64_t opponentKingPosition) {
    std::vector<Move> filteredMoves;
    for (const auto& move : moves) {
        if (move.getNewPosition() != opponentKingPosition) {
            filteredMoves.push_back(move);
        }
    }
    return filteredMoves;
}

/*
Removes any moves that would leave the king in check after the move is applied.

If the king is already in check, next move needs to make the king no longer in check.
Else if the king is not in check, it should not move to a position that would put the king in check.
*/
std::vector<Move> MoveGenerator::removeMovesLeavingKingInCheck(
    Board* board, 
    const std::vector<Move>& moves, 
    tileState color) {

    std::vector<Move> validMoves;
    for (const auto& move : moves) {
        Board copy = *board;
        copy.applyMove(move);
        if (!isInCheck(copy, color)) {
            validMoves.push_back(move);
        }
    }
    return validMoves;
}

bool MoveGenerator::isValidMove(const Move& move) {
    // Implement validation logic based on board state and piece rules
    return true; // Placeholder
}

void MoveGenerator::generatePawnDiagonalCaptures(
    std::vector<Move>& moves, 
    Board* board,
    Board::PieceIndex pieceType, 
    uint64_t oldPosition,
    int newRank, 
    int newFile) {
    
    if (isOnBoard(newRank, newFile)) {
        uint64_t proposedMove = gridToBinIdx(newRank, newFile);
        auto color = static_cast<tileState>(pieceType % 2);
        auto occupant = getOccupant(board, pieceType, proposedMove);
        if (occupant != color && occupant != EMPTY) {
            moves.push_back(Move(pieceType, proposedMove, oldPosition, true));
        }
    }
}

std::vector<Move> MoveGenerator::generatePawnMoves(Board* board, Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = board->getPiece(pieceType);
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

        // Check if square immediately in front is occupied
        tileState tileInPath1Ahead = getOccupant(board, pieceType, gridToBinIdx(rank + singleOffset, file));
        if (tileInPath1Ahead == EMPTY) {
            addMoveIfValid(moves, board, pieceType, oldPosition, rank + singleOffset, file);
        }

        // If on starting square, check if there are no pieces in the path 2 squares ahead
        if (rank == startRank) {
            tileState tileInPath2Ahead = getOccupant(board, pieceType, gridToBinIdx(rank + doubleOffset, file));

            if (tileInPath1Ahead == EMPTY && tileInPath2Ahead == EMPTY) {
                addMoveIfValid(moves, board, pieceType, oldPosition, rank + doubleOffset, file);
            }
        }

        // Check for diagonal captures
        for (const auto& os : captureOffsets) {
            generatePawnDiagonalCaptures(moves, board, pieceType, oldPosition, rank + os.first, file + os.second);
        }
        
        // Remove piece from reference bitboard; equivalent to marking as done
        piece &= piece - 1;
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(Board* board, Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = board->getPiece(pieceType);
    auto color = static_cast<tileState>(pieceType % 2);
    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        auto oldPosition = gridToBinIdx(rank, file);

        for (const auto& cardinal : rookOffsets) {
            for (const auto& os : cardinal) {
                if (!isOnBoard(rank + os.first, file + os.second)) { break; }
                uint64_t proposedPosition = gridToBinIdx(rank + os.first, file + os.second);
                tileState next = getOccupant(board, pieceType, proposedPosition);

                if (color == next) { break; }
                if (color != next && next != EMPTY) {
                    addMoveIfValid(moves, board, pieceType, oldPosition, rank + os.first, file + os.second);
                    break;
                }
                addMoveIfValid(moves, board, pieceType, oldPosition, rank + os.first, file + os.second);
            }
        }
        piece &= piece - 1;
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateKnightMoves(Board* board, Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = board->getPiece(pieceType);

    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        auto oldPosition = gridToBinIdx(rank, file);

        for (const auto& os : knightOffsets)
            addMoveIfValid(moves, board, pieceType, oldPosition, rank + os.first, file + os.second);
        piece &= piece - 1;
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateBishopMoves(Board* board, Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = board->getPiece(pieceType);
    auto color = static_cast<tileState>(pieceType % 2);
    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        auto oldPosition = gridToBinIdx(rank, file);

        for (const auto& diagonal : bishopOffsets) {
            for (const auto& os : diagonal) {
                if (!isOnBoard(rank + os.first, file + os.second)) { break; }
                uint64_t proposedPosition = gridToBinIdx(rank + os.first, file + os.second);
                tileState next = getOccupant(board, pieceType, proposedPosition);
                // cout << "color: " << color << "|" << "next: " << next << endl;
                if (color == next) { break; }
                if (color != next && next != EMPTY) {
                    addMoveIfValid(moves, board, pieceType, oldPosition, rank + os.first, file + os.second);
                    break;
                }
                addMoveIfValid(moves, board, pieceType, oldPosition, rank + os.first, file + os.second);
            }
        }
        piece &= piece - 1;
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(Board* board, Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = board->getPiece(pieceType);
    auto color = static_cast<tileState>(pieceType % 2);
    while (piece > 0) {
        auto rankfile = binIdxToGrid(piece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        auto oldPosition = gridToBinIdx(rank, file);
        for (const auto& lineOfSight : queenOffsets) {
            for (const auto& os : lineOfSight) {
                if (!isOnBoard(rank + os.first, file + os.second)) { break; }
                uint64_t proposedPosition = gridToBinIdx(rank + os.first, file + os.second);
                tileState next = getOccupant(board, pieceType, proposedPosition);
                if (color == next) { break; }
                if (color != next && next != EMPTY) {
                    addMoveIfValid(moves, board, pieceType, oldPosition, rank + os.first, file + os.second);
                    break;
                }
                addMoveIfValid(moves, board, pieceType, oldPosition, rank + os.first, file + os.second);
            }
        }
        piece &= piece - 1;
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateKingMoves(Board* board, Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    uint64_t piece = board->getPiece(pieceType);
    auto rankfile = binIdxToGrid(piece);
    int rank = std::get<0>(rankfile);
    int file = std::get<1>(rankfile);
    for (const auto& os : kingOffsets) {
        addMoveIfValid(moves, board, pieceType, piece, rank + os.first, file + os.second);
    }
    return moves;
}

tileState MoveGenerator::getOccupant(Board* board, Board::PieceIndex pieceType, uint64_t proposedMove) {
    auto pieces = board->getPieces();
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
    Board* board,
    Board::PieceIndex pieceType, 
    uint64_t oldPosition,
    int newRank, 
    int newFile) {
    
    tileState pieceColor = static_cast<tileState>(pieceType % 2);
    if (isOnBoard(newRank, newFile)) {
        uint64_t proposedMove = gridToBinIdx(newRank, newFile);        
        auto occupant = getOccupant(board, pieceType, proposedMove);
        if (occupant == pieceColor) { return; }
        moves.push_back(
            Move(pieceType, proposedMove, oldPosition, occupant != EMPTY));
    }
}

bool MoveGenerator::isInCheck(Board board, tileState color) {
    // Find the position of the king of the given color
    uint64_t kingPosition = board.getPiece((color == WHITE) ? Board::WHITE_KING : Board::BLACK_KING);

    // Generate all possible moves for the opponent
    tileState opponentColor = (color == WHITE) ? BLACK : WHITE;
    std::vector<Move> opponentMoves = this->generateAllMoves(board, opponentColor);

    // Check if any of the opponent's moves can capture the king
    for (const auto& move : opponentMoves) {
        if (move.getNewPosition() == kingPosition) {
            return true;
        }
    }
    return false;
}

/*
Asks the game:
    Is it checkmate for COLOR?

*/
bool MoveGenerator::isCheckmate(Board* board, tileState color) {
    if (!isInCheck(*board, color)) { return false; }
    std::vector<Move> allMoves = this->generateAllMoves(*board, color);

    // Check if any of the moves result in a non-check position
    for (const auto& move : allMoves) {
        Board copy = *board;
        copy.applyMove(move);
        if (!isInCheck(copy, color)) {
            return false;
        }
    }
    return true;
}

/*
Check if the game is over.
*/
bool MoveGenerator::isGameOver(Board* board) {
    if (isCheckmate(board, WHITE)) {
        cout << "Checkmate for WHITE" << endl;
        return true;
    }
    if (isCheckmate(board, BLACK)) {
        cout << "Checkmate for BLACK" << endl;
        return true;
    }

    // Check for stalemate
    if (generateAndFilterMoves(board, WHITE).size() == 0) {
        cout << "Stalemate for WHITE" << endl;
        return true;
    }
    if (generateAndFilterMoves(board, BLACK).size() == 0) {
        cout << "Stalemate for BLACK" << endl;
        return true;
    }
    return false;
}