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

    void bothBlackSetPieceRF(int rank, int file) {
        boardRook->setPieceRF(Board::BLACK_ROOKS, rank, file);
        boardBishop->setPieceRF(Board::BLACK_BISHOPS, rank, file);
    }

    std::vector<Move> bothMoveGenSetPieceRF(int rank, int file) {
        auto moves = this->moveGenRook->generatePieceMoves(Board::WHITE_ROOKS);
        auto moves2 = this->moveGenBishop->generatePieceMoves(Board::WHITE_BISHOPS);
        moves.insert(moves.end(), moves2.begin(), moves2.end());
        return moves;
    }

    std::vector<Move> bothBlackMoveGenSetPieceRF(int rank, int file) {
        auto moves = this->moveGenRook->generatePieceMoves(Board::BLACK_ROOKS);
        auto moves2 = this->moveGenBishop->generatePieceMoves(Board::BLACK_BISHOPS);
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
};

TEST_F(QueenTest, QueenMoveGeneration_two_Blocked_OppositeColor) {
    board->setPieceRF(Board::WHITE_QUEEN, 0, 0);
    board->setPieceRF(Board::BLACK_QUEEN, 1, 1);
    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_QUEEN);    
    ASSERT_EQ(moves.size(), 15); // 14 + 1 
};

TEST_F(QueenTest, QueenMoveGeneration_multiple_Blocked_SameColor) {
    board->setPieceRF(Board::WHITE_QUEEN, 0, 0);
    board->setPieceRF(Board::WHITE_QUEEN, 1, 1);
    board->setPieceRF(Board::WHITE_QUEEN, 2, 1);
    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_QUEEN);    
    ASSERT_EQ(moves.size(), 51); // 14 + (14 + 6 + 2) + (14 + 6 + 2 + 1)
};

TEST_F(QueenTest, QueenMoveGeneration_multiple_Blocked_OppositeColor) {
    board->setPieceRF(Board::WHITE_QUEEN, 0, 0);
    board->setPieceRF(Board::BLACK_QUEEN, 1, 1);
    board->setPieceRF(Board::BLACK_QUEEN, 2, 1);
    std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_QUEEN);    
    ASSERT_EQ(moves.size(), 15); // 14 + 1
};

TEST_F(QueenTest, QueenMoveGeneration_Corners) {
    for (auto [rank, file] : CORNERS) {
        board->setPieceRF(Board::WHITE_QUEEN, rank, file);
        std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_QUEEN);
        
        bothSetPieceRF(rank, file);
        auto movesBoth = bothMoveGenSetPieceRF(rank, file);
        
        ASSERT_EQ(moves.size(), movesBoth.size());

        board->fillEmpty();
        bothEmpty();
    }
};

TEST_F(QueenTest, QueenMoveGeneration_Corners_Blocked) {
    for (auto [rank, file] : CORNERS) {
        board->setPieceRF(Board::WHITE_QUEEN, rank, file);
        board->setPieceRF(Board::WHITE_QUEEN, rank + 1, file + 1);
        std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_QUEEN);
        
        bothSetPieceRF(rank, file);
        bothSetPieceRF(rank + 1, file + 1);
        auto movesBoth = bothMoveGenSetPieceRF(rank, file);
        
        ASSERT_EQ(moves.size(), movesBoth.size());

        board->fillEmpty();
        bothEmpty();
    }
};

TEST_F(QueenTest, QueenMoveGeneration_Corners_Blocked_OppositeColor) {
    for (auto [rank, file] : CORNERS) {
        board->setPieceRF(Board::WHITE_QUEEN, rank, file);
        board->setPieceRF(Board::BLACK_QUEEN, rank + 1, file + 1);
        std::vector<Move> moves = moveGen->generatePieceMoves(Board::WHITE_QUEEN);
        
        bothSetPieceRF(rank, file);
        bothBlackSetPieceRF(rank + 1, file + 1);
        auto movesBoth = bothMoveGenSetPieceRF(rank, file);
        // movesBoth += bothBlackMoveGenSetPieceRF(rank + 1, file + 1);
        
        ASSERT_EQ(moves.size(), movesBoth.size());

        board->fillEmpty();
        bothEmpty();
    }
};




