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

TEST_F(BoardTest, KingChecked_White_Rook) {
    board->setPieceRF(Board::WHITE_KING, 0, 0);
    board->setPieceRF(Board::BLACK_ROOKS, 0, 7);
    ASSERT_TRUE(board->isInCheck(WHITE));
}

TEST_F(BoardTest, KingChecked_White_Bishop) {
    board->setPieceRF(Board::WHITE_KING, 0, 0);
    board->setPieceRF(Board::BLACK_BISHOPS, 7, 7);
    ASSERT_TRUE(board->isInCheck(WHITE));
}

TEST_F(BoardTest, KingChecked_White_Queen) {
    board->setPieceRF(Board::WHITE_KING, 0, 0);
    board->setPieceRF(Board::BLACK_QUEEN, 0, 7);
    ASSERT_TRUE(board->isInCheck(WHITE));
}

TEST_F(BoardTest, KingChecked_White_Knight) {
    board->setPieceRF(Board::WHITE_KING, 0, 0);
    board->setPieceRF(Board::BLACK_KNIGHTS, 2, 1);
    ASSERT_TRUE(board->isInCheck(WHITE));
}

TEST_F(BoardTest, KingChecked_White_Pawn) {
    board->setPieceRF(Board::WHITE_KING, 0, 0);
    board->setPieceRF(Board::BLACK_PAWNS, 1, 1);
    ASSERT_TRUE(board->isInCheck(WHITE));
}

TEST_F(BoardTest, KingChecked_Black_Rook) {
    board->setPieceRF(Board::BLACK_KING, 7, 7);
    board->setPieceRF(Board::WHITE_ROOKS, 7, 0);
    ASSERT_TRUE(board->isInCheck(BLACK));
}

TEST_F(BoardTest, KingChecked_Black_Bishop) {
    board->setPieceRF(Board::BLACK_KING, 7, 7);
    board->setPieceRF(Board::WHITE_BISHOPS, 0, 0);
    ASSERT_TRUE(board->isInCheck(BLACK));
}

TEST_F(BoardTest, KingChecked_Black_Queen) {
    board->setPieceRF(Board::BLACK_KING, 7, 7);
    board->setPieceRF(Board::WHITE_QUEEN, 0, 0);
    ASSERT_TRUE(board->isInCheck(BLACK));
}

TEST_F(BoardTest, KingChecked_Black_Knight) {
    board->setPieceRF(Board::BLACK_KING, 7, 7);
    board->setPieceRF(Board::WHITE_KNIGHTS, 5, 6);
    ASSERT_TRUE(board->isInCheck(BLACK));
}

TEST_F(BoardTest, KingChecked_Black_Pawn) {
    board->setPieceRF(Board::BLACK_KING, 7, 7);
    board->setPieceRF(Board::WHITE_PAWNS, 6, 6);
    ASSERT_TRUE(board->isInCheck(BLACK));
}

/*
     0 1 2 3 4 5 6 7
     ---------------
  7| r n b q k b n r 
  6| p p p p p p p p 
  5|                 
  4|                 
  3| P               
  2|                 
  1|   P P P P P P P 
  0| R N B Q K B N R 

*/
TEST_F(BoardTest, ApplyMove_noCapture) {
    board->fillStandard();
    std::vector<Move> moves = moveGen->generateAllMoves(WHITE);
    Move move = moves[0];
    moveGen->applyMove(move);
    board->prettyPrint();
    ASSERT_EQ(board->getPiece(Board::WHITE_PAWNS), 0x000000000100FE00);
}