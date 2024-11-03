#include <iostream>
#include <tuple>
#include <bitset>

#include "../Board.h"
#include "../Move.h"

using namespace std;

void testStandardBoard() {
    Board board;
    board.fillStandard();
    board.prettyPrint();
}

void testAllPositionsRF() {
    Board board;
    board.fillEmpty();
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            board.setPieceRF(Board::WHITE_KING, rank, file);
            board.prettyPrint();
            board.fillEmpty();
            cout << endl;
        }
    }
}

void testAllPositionsBin() {
    Board board;
    board.fillEmpty();
    for (int i = 0; i < 64; i++) {
        board.setPieceBin(Board::WHITE_KING, 1ULL << i);
        board.prettyPrint();
        board.fillEmpty();
        cout << endl;
    }
}

void testKingMoveGeneration_alldirections() {
    Board board;
    board.fillEmpty();
    board.setPieceRF(Board::WHITE_KING, 4, 4);
    board.prettyPrint();
    MoveGenerator moveGen(&board);
    std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KING);
    for (const auto& move : moves) {
        // cout << "New position: " << move.getNewPosition() << endl;
        auto rankfile = moveGen.binIdxToGrid(move.getNewPosition());
        cout << "Rank: " << std::get<0>(rankfile) << " File: " << std::get<1>(rankfile) << endl;
    }
}

/*
Tests move generation for KING placed in all possible 
non-blocked positions on the board.
Expected output: "Moves.size(): 8" for all positions.
*/
void testKingMoveGeneration_alldirections_everywhere() {
    Board board;
    board.fillEmpty();
    for (int rank = 6; rank > 0; rank--) {
        for (int file = 1; file < 7; file++) {
            board.setPieceRF(Board::WHITE_KING, rank, file);
            MoveGenerator moveGen(&board);
            std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KING);
            // cout << "Moves.size(): " << moves.size() << endl;
            board.fillEmpty();
        }
    }
}

/*
Tests move gneeration for KING placed in EDGES only.
Expected output: "Moves.size(): 5" for all edges.
*/
void testKingMoveGeneration_edges() {
    Board board;
    board.fillEmpty();
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (rank == 0 || rank == 7 || file == 0 || file == 7) {
                if (rank == 0 && file == 0) continue;
                if (rank == 0 && file == 7) continue;
                if (rank == 7 && file == 0) continue;
                if (rank == 7 && file == 7) continue;
                board.setPieceRF(Board::WHITE_KING, rank, file);
                MoveGenerator moveGen(&board);
                std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KING);
                cout << "Moves.size(): " << moves.size() << endl;
                board.fillEmpty();
            }
        }
    }
}

/*
Test move generation for KING placed in CORNERS only.
Expected output: "Moves.size(): 3" for all corners.
*/
void testKingMoveGeneration_corners() {
    Board board;
    board.fillEmpty();
    std::tuple<int, int> corners[4] = { {0, 0}, {0, 7}, {7, 0}, {7, 7} };
    for (int i = 0; i < 4 ; i++) {
        board.setPieceRF(Board::WHITE_KING, std::get<0>(corners[i]), std::get<1>(corners[i]));
        MoveGenerator moveGen(&board);
        std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KING);
        cout << "Moves.size(): " << moves.size() << endl;
        board.fillEmpty();
    }
}

/*
Test move generation for KING surrounded by same COLOR pieces
KING @ (4, 4)
PAWNS @ (3, 3), (3, 4), (3, 5), (4, 3), (4, 5), (5, 3), (5, 4), (5, 5)

Expected output: "Moves.size(): 0"
*/
void testKingMoveGeneration_blockedSameColor() {
    Board board;
    board.setPieceRF(Board::WHITE_KING, 4, 4);
    MoveGenerator moveGen(&board);
    unsigned long long orv = 0ULL;
    orv |= moveGen.gridToBinIdx(3, 3);
    orv |= moveGen.gridToBinIdx(3, 4);
    orv |= moveGen.gridToBinIdx(3, 5);
    orv |= moveGen.gridToBinIdx(4, 3);
    orv |= moveGen.gridToBinIdx(4, 5);
    orv |= moveGen.gridToBinIdx(5, 3);
    orv |= moveGen.gridToBinIdx(5, 4);
    orv |= moveGen.gridToBinIdx(5, 5);
    cout << orv << endl;

    board.setPieceBin(Board::WHITE_PAWNS, orv);
    board.prettyPrint();    

    std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KING);
    cout << "Moves.size(): " << moves.size() << endl;
}

void testKingMoveGeneration_blockedSameColorCorner() {
    Board board;
    board.setPieceRF(Board::WHITE_KING, 0, 0);
    board.setPieceRF(Board::WHITE_PAWNS, 1, 0);
    board.setPieceRF(Board::WHITE_PAWNS, 0, 1);
    board.setPieceRF(Board::WHITE_PAWNS, 1, 1);

    // cout << std::bitset<64>(board.getPiece(Board::WHITE_PAWNS)) << endl;
    board.prettyPrint();

    MoveGenerator moveGen(&board);
    std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KING);
    // for (auto move : moves) {
        // auto rankfile = moveGen.binIdxToGrid(move.getNewPosition());
        // cout << std::get<0>(rankfile) << "," << std::get<1>(rankfile) << endl;
    // }

    cout << "Moves.size(): " << moves.size() << endl;
}

/*
Test move generation for KING surrounded by different COLOR pieces
KING @ (3, 3)
PAWNS @ (2, 2), (2, 3), (2, 4), (3, 2), (3, 4), (4, 2), (4, 3), (4, 4)

Expected output: "Moves.size(): 8"
*/
void testKingMoveGeneration_blockedDiffColor() {
    Board board;
    board.setPieceRF(Board::WHITE_KING, 3, 3);
    
    board.setPieceRF(Board::BLACK_PAWNS, 2, 2);
    board.setPieceRF(Board::BLACK_PAWNS, 2, 3);
    board.setPieceRF(Board::BLACK_PAWNS, 2, 4);
    board.setPieceRF(Board::BLACK_PAWNS, 3, 2);
    board.setPieceRF(Board::BLACK_PAWNS, 3, 4);
    board.setPieceRF(Board::BLACK_PAWNS, 4, 2);
    board.setPieceRF(Board::BLACK_PAWNS, 4, 3);
    board.setPieceRF(Board::BLACK_PAWNS, 4, 4);
    board.prettyPrint();

    MoveGenerator moveGen(&board);
    std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KING);
    cout << "Moves.size(): " << moves.size() << endl;
}

void testKingMoveGeneration_mixedColors() {
    Board board;
    board.setPieceRF(Board::WHITE_KING, 3, 3);

    board.setPieceRF(Board::WHITE_PAWNS, 2, 2);
    board.setPieceRF(Board::BLACK_PAWNS, 2, 3);
    board.setPieceRF(Board::WHITE_PAWNS, 2, 4);
    board.setPieceRF(Board::BLACK_PAWNS, 3, 2);
    board.setPieceRF(Board::BLACK_PAWNS, 3, 4);
    board.setPieceRF(Board::WHITE_PAWNS, 4, 2);
    board.setPieceRF(Board::BLACK_PAWNS, 4, 3);
    board.setPieceRF(Board::WHITE_PAWNS, 4, 4);
    board.prettyPrint();

    MoveGenerator moveGen(&board);
    std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KING);
    cout << "Moves.size(): " << moves.size() << endl;
}

void test_gridToBinIdx() {
    MoveGenerator moveGen(nullptr);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            cout << std::bitset<64>(moveGen.gridToBinIdx(rank, file)) << endl;
        }
    }
    for (int i = 0; i < 64; i++) {
        auto rankfile = moveGen.binIdxToGrid(1ULL << i);
        cout << std::get<0>(rankfile) << "," << std::get<1>(rankfile) << endl;
    }
}

int main() {
    // testAllPositionsRF();
    // testAllPositionsBin();
    // testKingMoveGeneration_alldirections();
    // testKingMoveGeneration_alldirections_everywhere();
    // testKingMoveGeneration_edges();
    // testKingMoveGeneration_corners();
    // test_gridToBinIdx();
    // testKingMoveGeneration_blockedSameColor();
    // testKingMoveGeneration_blockedSameColorCorner();
    // testKingMoveGeneration_blockedDiffColor();
    testKingMoveGeneration_mixedColors();

    return 0;
}