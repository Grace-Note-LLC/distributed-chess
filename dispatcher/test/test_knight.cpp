#include <gtest/gtest.h>
#include <tuple>
#include <vector>

#include "../Board.h"
#include "../Move.h"
#include "../Utils.h"

class KnightTest : public ::testing::Test {
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

TEST_F(KnightTest, KnightMoveGeneration_Single_Center) {
    board->setPieceRF(Board::WHITE_KNIGHTS, 4, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_KNIGHTS);
    ASSERT_EQ(moves.size(), 8);
};

TEST_F(KnightTest, KnightMoveGeneration_Corners) {
    for (auto& corner : CORNERS) {
        board->setPieceRF(Board::WHITE_KNIGHTS, std::get<0>(corner), std::get<1>(corner));
        std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_KNIGHTS);
        ASSERT_EQ(moves.size(), 2);
        board->fillEmpty();
    }
};

TEST_F(KnightTest, KnightMoveGeneration_Edges_Max) {
    for (int i = 2; i < 6; i++) {
        board->setPieceRF(Board::WHITE_KNIGHTS, 0, i);
        std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_KNIGHTS);
        ASSERT_EQ(moves.size(), 4);
        board->fillEmpty();
    }
};

TEST_F(KnightTest, KnightMoveGeneration_Surrounded_SameColor) {
    board->setPieceRF(Board::WHITE_KNIGHTS, 4, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 3, 3);
    board->setPieceRF(Board::WHITE_PAWNS, 3, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 3, 5);
    board->setPieceRF(Board::WHITE_PAWNS, 4, 3);
    board->setPieceRF(Board::WHITE_PAWNS, 4, 5);
    board->setPieceRF(Board::WHITE_PAWNS, 5, 3);
    board->setPieceRF(Board::WHITE_PAWNS, 5, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 5, 5);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_KNIGHTS);
    ASSERT_EQ(moves.size(), 8);
};

TEST_F(KnightTest, KnightMoveGeneration_All_Capture_DiffColor) {
    board->setPieceRF(Board::WHITE_KNIGHTS, 4, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 6, 5);    
    board->setPieceRF(Board::BLACK_PAWNS, 5, 6);
    board->setPieceRF(Board::BLACK_PAWNS, 3, 6);
    board->setPieceRF(Board::BLACK_PAWNS, 2, 5);
    board->setPieceRF(Board::BLACK_PAWNS, 3, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 2, 3);
    board->setPieceRF(Board::BLACK_PAWNS, 6, 3);
    board->setPieceRF(Board::BLACK_PAWNS, 5, 2);    
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_KNIGHTS);
    ASSERT_EQ(moves.size(), 8);
}

TEST_F(KnightTest, KnightMoveGeneration_All_Blocked_SameColor) {
    board->setPieceRF(Board::WHITE_KNIGHTS, 4, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 6, 5);    
    board->setPieceRF(Board::WHITE_PAWNS, 5, 6);
    board->setPieceRF(Board::WHITE_PAWNS, 3, 6);
    board->setPieceRF(Board::WHITE_PAWNS, 2, 5);
    board->setPieceRF(Board::WHITE_PAWNS, 3, 2);
    board->setPieceRF(Board::WHITE_PAWNS, 2, 3);
    board->setPieceRF(Board::WHITE_PAWNS, 6, 3);
    board->setPieceRF(Board::WHITE_PAWNS, 5, 2);    
    board->prettyPrint();
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_KNIGHTS);
    ASSERT_EQ(moves.size(), 0);
}