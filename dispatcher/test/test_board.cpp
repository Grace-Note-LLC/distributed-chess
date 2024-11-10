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
        moveGen = new MoveGenerator(board);
    }

    void TearDown() override {
        delete board;
    }

    Board* board;
    MoveGenerator* moveGen;
};

TEST_F(BoardTest, testAllPositionsRF) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            board->setPieceRF(Board::WHITE_KING, rank, file);
            board->fillEmpty();
        }
    }
}

TEST_F(BoardTest, testAllPositionsBin) {
    for (int i = 0; i < 64; i++) {
        board->setPieceBin(Board::WHITE_KING, 1ULL << i);
        board->fillEmpty();
    }
}

TEST_F(BoardTest, MoveGenerator_AllPieces_Standard_White) {
    board->fillStandard();
    std::vector<Move> moves = moveGen->generateAllMoves(WHITE);
    ASSERT_EQ(moves.size(), 20);
}

TEST_F(BoardTest, MoveGenerator_AllPieces_Standard_Black) {
    board->fillStandard();
    std::vector<Move> moves = moveGen->generateAllMoves(BLACK);
    ASSERT_EQ(moves.size(), 20);
}

TEST_F(BoardTest, KingChecked_White) {
    board->setPieceRF(Board::WHITE_KING, 0, 0);
    board->setPieceRF(Board::BLACK_ROOKS, 0, 7);
    ASSERT_TRUE(board->isInCheck(WHITE));
}

TEST_F(BoardTest, KingChecked_Black) {
    board->setPieceRF(Board::BLACK_KING, 7, 7);
    board->setPieceRF(Board::WHITE_ROOKS, 7, 0);
    ASSERT_TRUE(board->isInCheck(BLACK));
}