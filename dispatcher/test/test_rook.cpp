#include <gtest/gtest.h>
#include <tuple>

#include "../Board.h"
#include "../Move.h"
#include "../Utils.h"

using namespace std;

class RookTest : public ::testing::Test {
protected:
    void SetUp() override {
        board = new Board();
        board->fillEmpty();
        moveGen = new MoveGenerator();
    }

    void TearDown() override {
        delete moveGen;
        delete board;
    }

    Board* board;
    MoveGenerator* moveGen;
    std::vector<std::tuple<int, int>> CORNERS = {
        {0, 0}, {0, 7}, {7, 0}, {7, 7}
    };
};


TEST_F(RookTest, RookMoveGeneration_All) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            board->setPieceRF(Board::WHITE_ROOKS, rank, file);
            std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_ROOKS);
            ASSERT_EQ(moves.size(), 14);
            board->fillEmpty();
        }
    }
};

TEST_F(RookTest, RookMoveGeneration_DiffRankFile) {
    board->setPieceRF(Board::WHITE_ROOKS, 0, 0);
    board->setPieceRF(Board::WHITE_ROOKS, 1, 1);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_ROOKS);
    ASSERT_EQ(moves.size(), 28); // 14 + 14
};

TEST_F(RookTest, RookMoveGeneration_DiffRank_SameFile) {
    board->setPieceRF(Board::WHITE_ROOKS, 0, 0);
    board->setPieceRF(Board::WHITE_ROOKS, 1, 0);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_ROOKS);
    ASSERT_EQ(moves.size(), 20); // 7 + 13
};

TEST_F(RookTest, RookMoveGeneration_DiffRank_SameFileFar) {
    board->setPieceRF(Board::WHITE_ROOKS, 0, 0);
    board->setPieceRF(Board::WHITE_ROOKS, 4, 0);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_ROOKS);
    ASSERT_EQ(moves.size(), 23); // 10 + 13
};

TEST_F(RookTest, RookMoveGeneration_SameRank_DiffFile) {
    board->setPieceRF(Board::WHITE_ROOKS, 0, 0);
    board->setPieceRF(Board::WHITE_ROOKS, 0, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_ROOKS);
    ASSERT_EQ(moves.size(), 23); // 10 + 13
};


TEST_F(RookTest, RookMoveGeneration_SameRank_DiffFile_BlockedAbove) {
    board->setPieceRF(Board::WHITE_ROOKS, 0, 0);
    board->setPieceRF(Board::WHITE_ROOKS, 0, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 1, 0);
    board->setPieceRF(Board::WHITE_PAWNS, 1, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_ROOKS);
    ASSERT_EQ(moves.size(), 9); // 3 + 6
};