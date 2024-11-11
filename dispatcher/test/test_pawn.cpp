#include <gtest/gtest.h>
#include <tuple>
#include <algorithm>

#include "../Board.h"
#include "../Move.h"
#include "../Utils.h"

using namespace std;

class PawnTest : public ::testing::Test {
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

/*
Tests white PAWN move generation for a single PAWN 
without double moves/captures/promotions.
*/
TEST_F(PawnTest, PawnMoveGeneration_White_Single) {
    board->setPieceRF(Board::WHITE_PAWNS, 4, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_PAWNS);
    ASSERT_EQ(moves.size(), 1);
}

TEST_F(PawnTest, PawnMoveGeneration_White_Double) {
    board->setPieceRF(Board::WHITE_PAWNS, 1, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_PAWNS);
    ASSERT_EQ(moves.size(), 2);
}

TEST_F(PawnTest, PawnMoveGeneration_White_Capture) {
    board->setPieceRF(Board::WHITE_PAWNS, 4, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 5, 5);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_PAWNS);
    ASSERT_EQ(moves.size(), 2);
}

/*
Pawn can capture, move forward, or move 2x forward.
*/
TEST_F(PawnTest, PawnMoveGeneration_White_Capture_Double) {
    board->setPieceRF(Board::WHITE_PAWNS, 1, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 2, 5);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_PAWNS);
    ASSERT_EQ(moves.size(), 3);
}

TEST_F(PawnTest, PawnMoveGeneration_White_Blocked_DiffColor) {
    board->setPieceRF(Board::WHITE_PAWNS, 1, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 2, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_PAWNS);
    ASSERT_EQ(moves.size(), 0);
}

TEST_F(PawnTest, PawnMoveGeneration_White_Blocked_SameColor) {
    board->setPieceRF(Board::WHITE_PAWNS, 1, 4);
    board->setPieceRF(Board::WHITE_KING, 2, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_PAWNS);
    ASSERT_EQ(moves.size(), 0);
}

TEST_F(PawnTest, PawnMoveGeneration_White_Blocked_AnotherPawn_SameColor) {
    board->setPieceRF(Board::WHITE_PAWNS, 1, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 2, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::WHITE_PAWNS);
    ASSERT_EQ(moves.size(), 1);
}

/**************************************
Black Pawn Tests
**************************************/ 
TEST_F(PawnTest, PawnMoveGeneration_Black_Single) {
    board->setPieceRF(Board::BLACK_PAWNS, 4, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::BLACK_PAWNS);
    ASSERT_EQ(moves.size(), 1);
    auto rankfile = binIdxToGrid(moves.at(0).getNewPosition());
    ASSERT_EQ(std::get<0>(rankfile), 3); // decrease rank
    ASSERT_EQ(std::get<1>(rankfile), 4);
}

TEST_F(PawnTest, PawnMoveGeneration_Black_Double) {
    board->setPieceRF(Board::BLACK_PAWNS, 6, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::BLACK_PAWNS);
    ASSERT_EQ(moves.size(), 2);

    std::vector<std::tuple<int, int>> expectedMoves = { {4, 4}, {5, 4} };
    ASSERT_EQ(moves.size(), expectedMoves.size());
    for (const auto& move : moves) {
        auto rankfile = binIdxToGrid(move.getNewPosition());
        ASSERT_TRUE(
            std::find(expectedMoves.begin(), expectedMoves.end(), rankfile) != expectedMoves.end()
        );
    }
}

TEST_F(PawnTest, PawnMoveGeneration_Black_Capture) {
    board->setPieceRF(Board::BLACK_PAWNS, 4, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 3, 5);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::BLACK_PAWNS);
    ASSERT_EQ(moves.size(), 2);
}

TEST_F(PawnTest, PawnMoveGeneration_Black_Capture_Double) {
    board->setPieceRF(Board::BLACK_PAWNS, 6, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 5, 5);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::BLACK_PAWNS);
    std::vector<std::tuple<int, int>> expectedMoves = { {4, 4}, {5, 4}, {5, 5} };
    ASSERT_EQ(moves.size(), expectedMoves.size());
    for (auto& move : moves) move.print();
    for (const auto& move : moves) {
        auto rankfile = binIdxToGrid(move.getNewPosition());
        ASSERT_TRUE(
            std::find(expectedMoves.begin(), expectedMoves.end(), rankfile) != expectedMoves.end()
        );
    }
    ASSERT_EQ(moves.size(), 3);
}

TEST_F(PawnTest, PawnMoveGeneration_Black_Blocked_DiffColor) {
    board->setPieceRF(Board::BLACK_PAWNS, 6, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 5, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::BLACK_PAWNS);
    ASSERT_EQ(moves.size(), 0);
}

TEST_F(PawnTest, PawnMoveGeneration_Black_Blocked_SameColor) {
    board->setPieceRF(Board::BLACK_PAWNS, 6, 4);
    board->setPieceRF(Board::BLACK_KING, 5, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::BLACK_PAWNS);
    ASSERT_EQ(moves.size(), 0);
}

TEST_F(PawnTest, PawnMoveGeneration_Black_Blocked_AnotherPawn_SameColor) {
    board->setPieceRF(Board::BLACK_PAWNS, 6, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 5, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(board, Board::BLACK_PAWNS);
    ASSERT_EQ(moves.size(), 1);
}