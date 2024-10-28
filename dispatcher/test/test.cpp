#include <iostream>
#include <tuple>
#include "../Board.h"
#include "../Move.h"

using namespace std;

void testStandardBoard() {
    Board board;
    board.fillStandard();
    board.prettyPrint(board);
}

void testAllPositionsRF() {
    Board board;
    board.fillEmpty();
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            board.setPieceRF(Board::WHITE_KING, rank, file);
            board.prettyPrint(board);
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
        board.prettyPrint(board);
        board.fillEmpty();
        cout << endl;
    }
}

void testKingMoveGeneration_alldirections() {
    Board board;
    board.fillEmpty();
    board.setPieceRF(Board::WHITE_KING, 4, 4);
    board.prettyPrint(board);
    MoveGenerator moveGen(&board);
    std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KING);
    for (const auto& move : moves) {
        // cout << "New position: " << move.getNewPosition() << endl;
        auto rankfile = moveGen.binIdxToGrid(move.getNewPosition());
        cout << "Rank: " << std::get<0>(rankfile) << " File: " << std::get<1>(rankfile) << endl;
    }
}

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


int main() {
    // testAllPositionsRF();
    // testAllPositionsBin();
    // testKingMoveGeneration_alldirections();
    testKingMoveGeneration_alldirections_everywhere();

    return 0;
}