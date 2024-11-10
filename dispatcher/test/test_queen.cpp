#include <gtest/gtest.h>
#include <tuple>

#include "../Board.h"
#include "../Move.h"
#include "../Utils.h"

using namespace std;

/*
In general, if we can show that ROOK and BISHOP move generation is correct,
then QUEEN move generation should be correct as well.
*/
class QueenTest : public ::testing::Test {
protected:
    void SetUp() override {
        board = new Board();
        board->fillEmpty();
        moveGen = new MoveGenerator(board);

        boardRook = new Board();
        boardRook->fillEmpty();
        moveGenRook = new MoveGenerator(boardRook);

        boardBishop = new Board();
        boardBishop->fillEmpty();
        moveGenBishop = new MoveGenerator(boardBishop);
    }

    void TearDown() override {
        delete moveGen;
        delete board;
        delete moveGenRook;
        delete boardRook;
        delete moveGenBishop;
        delete boardBishop;
    }

    void bothSetPieceRF(int rank, int file) {
        boardRook->setPieceRF(Board::WHITE_ROOKS, rank, file);
        boardBishop->setPieceRF(Board::WHITE_BISHOPS, rank, file);
    }

    std::vector<Move> bothMoveGenSetPieceRF(int rank, int file) {
        auto moves = this->moveGenRook->generatePieceMoves(Board::WHITE_ROOKS);
        auto moves2 = this->moveGenBishop->generatePieceMoves(Board::WHITE_BISHOPS);
        moves.insert(moves.end(), moves2.begin(), moves2.end());
        return moves;
    }

    void bothEmpty() {
        boardRook->fillEmpty();
        boardBishop->fillEmpty();
    }

    Board* board;
    MoveGenerator* moveGen;

    Board* boardRook;
    MoveGenerator* moveGenRook;

    Board* boardBishop;
    MoveGenerator* moveGenBishop;

    std::vector<Move> movesBoth;

    std::vector<std::tuple<int, int>> CORNERS = {
        {0, 0}, {0, 7}, {7, 0}, {7, 7}
    };
};

TEST_F(QueenTest, QueenMoveGeneration_All) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            board->setPieceRF(Board::WHITE_QUEEN, rank, file);
            std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_QUEEN);
            
            bothSetPieceRF(rank, file);
            auto movesBoth = bothMoveGenSetPieceRF(rank, file);
            
            ASSERT_EQ(moves.size(), movesBoth.size());

            board->fillEmpty();
            bothEmpty();
        }
    }
};

TEST_F(QueenTest, QueenMoveGeneration_two_Blocked_SameColor) {
    board->setPieceRF(Board::WHITE_QUEEN, 0, 0);
    board->setPieceRF(Board::WHITE_QUEEN, 1, 1);
    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_QUEEN);    
    ASSERT_EQ(moves.size(), 36); // 14 + (14 + 6 + 2)
}
