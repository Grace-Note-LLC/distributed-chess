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
    MoveGenerator moveGen(board);
    std::vector<Move> moves = moveGen.generateAllMoves(WHITE);
    ASSERT_EQ(moves.size(), 20);
}

TEST_F(BoardTest, MoveGenerator_AllPieces_Standard_Black) {
    board->fillStandard();
    MoveGenerator moveGen(board);
    std::vector<Move> moves = moveGen.generateAllMoves(BLACK);
    ASSERT_EQ(moves.size(), 20);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}