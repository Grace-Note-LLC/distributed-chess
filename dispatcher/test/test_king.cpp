#include <gtest/gtest.h>
#include <algorithm>

#include "../Board.h"
#include "../Move.h"
#include "../Utils.h"

class KingTest : public ::testing::Test {
protected:
    void SetUp() override {
        board = new Board();
        board->fillEmpty();
        moveGen = new MoveGenerator(board);
    }

    void TearDown() override {
        delete moveGen;
        delete board;
    }

    Board* board;
    MoveGenerator* moveGen;
};

TEST_F(KingTest, KingMoveGeneration_AllDirections) {
    board->setPieceRF(Board::WHITE_KING, 4, 4);
    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_KING);
    std::vector<std::tuple<int, int>> expectedMoves = {
        {3, 3}, {3, 4}, {3, 5},
        {4, 3},         {4, 5},
        {5, 3}, {5, 4}, {5, 5}
    };

    ASSERT_EQ(moves.size(), expectedMoves.size());

    for (const auto& move : moves) {
        auto rankfile = binIdxToGrid(move.getNewPosition());
        ASSERT_TRUE(
            std::find(expectedMoves.begin(), expectedMoves.end(), rankfile) != expectedMoves.end()
        );
    }
}

TEST_F(KingTest, KingMoveGeneration_Edges) {
    std::vector<std::tuple<int, int>> edgePositions = {
        {0, 1}, {0, 6}, {1, 0}, {1, 7},
        {6, 0}, {6, 7}, {7, 1}, {7, 6}
    };

    for (const auto& pos : edgePositions) {
        board->setPieceRF(Board::WHITE_KING, std::get<0>(pos), std::get<1>(pos));
        std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_KING);
        ASSERT_EQ(moves.size(), 5);
        board->fillEmpty();
    }
}

TEST_F(KingTest, KingMoveGeneration_Corners) {
    std::vector<std::tuple<int, int>> cornerPositions = {
        {0, 0}, {0, 7}, {7, 0}, {7, 7}
    };

    for (const auto& pos : cornerPositions) {
        board->setPieceRF(Board::WHITE_KING, std::get<0>(pos), std::get<1>(pos));
        std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_KING);
        ASSERT_EQ(moves.size(), 3);
        board->fillEmpty();
    }
}

TEST_F(KingTest, KingMoveGeneration_BlockedSameColor) {
    board->setPieceRF(Board::WHITE_KING, 4, 4);
    unsigned long long orv = 0ULL;
    orv |= gridToBinIdx(3, 3);
    orv |= gridToBinIdx(3, 4);
    orv |= gridToBinIdx(3, 5);
    orv |= gridToBinIdx(4, 3);
    orv |= gridToBinIdx(4, 5);
    orv |= gridToBinIdx(5, 3);
    orv |= gridToBinIdx(5, 4);
    orv |= gridToBinIdx(5, 5);
    board->setPieceBin(Board::WHITE_PAWNS, orv);

    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_KING);
    ASSERT_EQ(moves.size(), 0);
}

/*
Test move generation for KING surrounded by different COLOR pieces
KING @ (3, 3)
PAWNS @ (2, 2), (2, 3), (2, 4), (3, 2), (3, 4), (4, 2), (4, 3), (4, 4)
*/
TEST_F(KingTest, KingMoveGeneration_BlockedDiffColor) {
    board->setPieceRF(Board::WHITE_KING, 3, 3);
    board->setPieceRF(Board::BLACK_PAWNS, 2, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 2, 3);
    board->setPieceRF(Board::BLACK_PAWNS, 2, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 3, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 3, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 4, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 4, 3);
    board->setPieceRF(Board::BLACK_PAWNS, 4, 4);

    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_KING);
    ASSERT_EQ(moves.size(), 8);
}
/*
Tests move generation for KING placed in all possible 
non-blocked positions on the board.
*/
TEST_F(KingTest, KingMoveGeneration_AllDirectionsEverywhere) {
    for (int rank = 6; rank > 0; rank--) {
        for (int file = 1; file < 7; file++) {
            board->setPieceRF(Board::WHITE_KING, rank, file);
            std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_KING);
            ASSERT_EQ(moves.size(), 8);
            board->fillEmpty();
        }
    }
}

TEST_F(KingTest, KingMoveGeneration_BlockedSameColorCorner) {
    board->setPieceRF(Board::WHITE_KING, 0, 0);
    board->setPieceRF(Board::WHITE_PAWNS, 1, 0);
    board->setPieceRF(Board::WHITE_PAWNS, 0, 1);
    board->setPieceRF(Board::WHITE_PAWNS, 1, 1);

    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_KING);
    ASSERT_EQ(moves.size(), 0);
}

TEST_F(KingTest, KingMoveGeneration_MixedColors) {
    board->setPieceRF(Board::WHITE_KING, 3, 3);

    board->setPieceRF(Board::WHITE_PAWNS, 2, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 2, 3);
    board->setPieceRF(Board::WHITE_PAWNS, 2, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 3, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 3, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 4, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 4, 3);
    board->setPieceRF(Board::WHITE_PAWNS, 4, 4);

    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_KING);
    ASSERT_EQ(moves.size(), 4);
}

TEST_F(KingTest, KingMoveGeneration_MixedColorsPrint) {
    board->setPieceRF(Board::WHITE_KING, 3, 3);

    board->setPieceRF(Board::WHITE_PAWNS, 2, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 2, 3);
    board->setPieceRF(Board::WHITE_PAWNS, 2, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 3, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 3, 4);
    board->setPieceRF(Board::WHITE_PAWNS, 4, 2);
    board->setPieceRF(Board::BLACK_PAWNS, 4, 3);
    board->setPieceRF(Board::WHITE_PAWNS, 4, 4);
    // board->prettyPrint();
    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_KING);
    ASSERT_EQ(moves.size(), 4);
}
