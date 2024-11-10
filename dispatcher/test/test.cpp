#include <gtest/gtest.h>
#include <iostream>
#include <tuple>
#include <bitset>

#include "../Board.h"
#include "../Move.h"
#include "../Utils.h"

using namespace std;

std::tuple<int, int> CORNERS[4] = { {0, 0}, {0, 7}, {7, 0}, {7, 7} };

class BoardTest : public ::testing::Test {
protected:
    void SetUp() override {
        board = new Board();
    }

    void TearDown() override {
        delete board;
    }

    Board* board;
};

TEST_F(BoardTest, StandardBoardSetup) {
    board->fillStandard();
    board->prettyPrint();
}

TEST_F(BoardTest, testAllPositionsRF) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            board->setPieceRF(Board::WHITE_KING, rank, file);
            board->prettyPrint();
            board->fillEmpty();
            cout << endl;
        }
    }
}

TEST_F(BoardTest, testAllPositionsBin) {
    for (int i = 0; i < 64; i++) {
        board->setPieceBin(Board::WHITE_KING, 1ULL << i);
        board->prettyPrint();
        board->fillEmpty();
        cout << endl;
    }
}

void testKnightMoveGeneration_all() {
    Board board;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            board.setPieceRF(Board::WHITE_KNIGHTS, rank, file);
            MoveGenerator moveGen(&board);
            std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_KNIGHTS);
            cout << "(" << rank << "," << file << "): " << "moves.size(): " << moves.size() << endl;
            board.fillEmpty();
        }
    }
}

void testQueenMoveGeneration_all() {
    Board board;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            board.setPieceRF(Board::WHITE_QUEEN, rank, file);
            MoveGenerator moveGen(&board);
            std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_QUEEN);
            cout << "(" << rank << "," << file << "): " << "moves.size(): " << moves.size() << endl;
            board.fillEmpty();
        }
    }
}

/*
Tests white PAWN move generation for a single PAWN 
without double moves/captures/promotions.
*/
void testPawnMoveGen_white_single() {
    Board board;
    board.setPieceRF(Board::WHITE_PAWNS, 4, 4);
    board.prettyPrint();
    MoveGenerator moveGen(&board);
    std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_PAWNS);
    cout << "moves.size(): " << moves.size() << endl;
    for (auto& move : moves) {
        move.print();
    }
}

void testPawnMoveGen_white_double() {
    Board board;
    board.setPieceRF(Board::WHITE_PAWNS, 1, 4);
    board.prettyPrint();
    MoveGenerator moveGen(&board);
    std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_PAWNS);
    cout << "moves.size(): " << moves.size() << endl;
    for (auto& move : moves) move.print();
}

/*
Check if pawn generates 0 moves if it is blocked in front
*/
void testPawnMoveGen_white_blocked() {
    Board board;
    board.setPieceRF(Board::WHITE_PAWNS, 1, 4);
    board.setPieceRF(Board::BLACK_PAWNS, 2, 4);
    board.prettyPrint();
    MoveGenerator moveGen(&board);
    std::vector<Move> moves = moveGen.generatePieceMoves(Board::WHITE_PAWNS);
    cout << "moves.size(): " << moves.size() << endl;
    for (auto& move : moves) move.print();
}

/*

*/

void test_gridToBinIdx() {
    MoveGenerator moveGen(nullptr);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            cout << std::bitset<64>(gridToBinIdx(rank, file)) << endl;
        }
    }
    for (int i = 0; i < 64; i++) {
        auto rankfile = binIdxToGrid(1ULL << i);
        cout << std::get<0>(rankfile) << "," << std::get<1>(rankfile) << endl;
    }
}


int main(int argc, char** argv) {
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
    // testKingMoveGeneration_mixedColors();
    // testBishopMoveGeneration();
    // testBishopMoveGeneration_corners();
    // testBishopMoveGeneration_edges();
    // testBishopMoveGeneration_all();
    // testBishopMoveGeneration_two();
    // testRookMoveGeneration_all();
    // testKnightMoveGeneration_all();
    // testQueenMoveGeneration_all();
    // testPawnMoveGen_white_single();
    // testPawnMoveGen_white_double();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    // testPawnMoveGen_white_blocked();

    // return 0;
}