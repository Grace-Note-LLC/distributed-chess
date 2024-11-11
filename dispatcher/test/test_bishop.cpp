#include <gtest/gtest.h>
#include <tuple>

#include "../Board.h"
#include "../Move.h"
#include "../Utils.h"

using namespace std;

class BishopTest : public ::testing::Test {
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

TEST_F(BishopTest, BishopMoveGeneration_Edges) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (rank == 0 || rank == 7 || file == 0 || file == 7) {
                board->setPieceRF(Board::WHITE_BISHOPS, rank, file);
                std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_BISHOPS);
                ASSERT_EQ(moves.size(), 7);
                board->fillEmpty();
            }
        }
    }
}

TEST_F(BishopTest, BishopMoveGeneration_Corners) {
    for (int i = 0; i < 4; i++) {
        board->setPieceRF(
            Board::WHITE_BISHOPS, 
            std::get<0>(CORNERS[i]), std::get<1>(CORNERS[i])
        );
        std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_BISHOPS);
        // for (auto& move : moves) { move.print(); }
        // cout << "new corner" << endl;
        ASSERT_EQ(moves.size(), 7);
        board->fillEmpty();
    }
}

/*
Test move generation for two BISHOPS placed on the board.
*/
TEST_F(BishopTest, BishopMoveGeneration_Two) {
    board->setPieceRF(Board::WHITE_BISHOPS, 4, 4);
    board->setPieceRF(Board::WHITE_BISHOPS, 4, 3);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_BISHOPS);
    ASSERT_EQ(moves.size(), 26);
};

TEST_F(BishopTest, BishopMoveGeneration_Blocked_SameColor) {
    board->setPieceRF(Board::WHITE_BISHOPS, 4, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 5, 5);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_BISHOPS);
    ASSERT_EQ(moves.size(), 10);
};

TEST_F(BishopTest, BishopMoveGeneration_Blocked_DiffColor) {
    board->setPieceRF(Board::WHITE_BISHOPS, 4, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 5, 5);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_BISHOPS);
    ASSERT_EQ(moves.size(), 11); // Bishop can capture the black pawn
};
/*
Tests collision with same color piece at a far distance
*/
TEST_F(BishopTest, BishopMoveGeneration_Blocked_SameColor_Far) {
    board->setPieceRF(Board::WHITE_BISHOPS, 4, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 6, 6);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_BISHOPS);
    ASSERT_EQ(moves.size(), 11);
};

TEST_F(BishopTest, BishopMoveGeneration_SameDiagonal) {
    board->setPieceRF(Board::WHITE_BISHOPS, 0, 0);
    board->setPieceRF(Board::WHITE_BISHOPS, 2, 2);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_BISHOPS);
    ASSERT_EQ(moves.size(), 11); // 1 + 10 
};

TEST_F(BishopTest, BishopMoveGeneration_DiffDiagonal) {
    board->setPieceRF(Board::WHITE_BISHOPS, 0, 0);
    board->setPieceRF(Board::WHITE_BISHOPS, 2, 1);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_BISHOPS);
    ASSERT_EQ(moves.size(), 16); // 7 + 9 
};
