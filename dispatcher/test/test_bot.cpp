#include <gtest/gtest.h>
#include <iostream>
#include <tuple>
#include <bitset>

#include "../Board.h"
#include "../Move.h"
#include "../Utils.h"
#include "../Bot.h"

using namespace std;

class AITest : public ::testing::Test {
protected:
    void SetUp() override {
        board = new Board();
        moveGen = new MoveGenerator();
        ai = new ChessBot(*moveGen);
    }

    void TearDown() override {
        delete board;
        delete moveGen;
        delete ai;
        board = nullptr;
        moveGen = nullptr;
        ai = nullptr;
    }

    Board* board;
    MoveGenerator* moveGen;
    ChessBot* ai;
};

TEST_F(AITest, FindBestMoveInInitialPosition) {
    board->fillStandard();
    Move bestMove = ai->findBestMove(board, WHITE).first;

    // Print the best move
    std::cout << "Best move in initial position: " << std::endl;
    bestMove.print();

    // Verify that the best move is a valid move
    std::vector<Move> legalMoves = moveGen->generateAllMoves(*board, WHITE);
    bool isValidMove = false;
    for (const auto& move : legalMoves) {
        if (move.getNewPosition() == bestMove.getNewPosition()) {
            std::cout << "Best move is a legal move" << std::endl;
            isValidMove = true;
            break;
        }
    }
    ASSERT_TRUE(isValidMove);
}

TEST_F(AITest, AgainstSelf) {
    board->fillStandard();
    tileState player = WHITE;
    int iter = 0;
    while (!moveGen->isGameOver(board) && iter < 6000) {
        auto best = ai->findBestMove(board, player);
        Move bestMove = best.first;
        int score = best.second;
        cout << endl << "\t\tITER: " << iter << endl;
        cout << "Best: " << (player == WHITE ? "WHITE" : "BLACK") << " : " << score << endl;
        bestMove.print();
        cout << "\t\tTurn: " << player << endl;

        board->applyMove(bestMove);        
        board->prettyPrint();
        iter++;
        player = (player == WHITE) ? BLACK : WHITE;
    }

    ASSERT_TRUE(moveGen->isGameOver(board));
}

/**
 * Tests that the king does not put itself in check.
 * This is set up from a custom position.
 */
TEST_F(AITest, NoMoveIntoCheck) {
    // Set up black pieces
    board->setPieceBin(Board::BLACK_PAWNS, 0b0000000011101111000100000000000000000000000000000000000000000000);
    board->setPieceRF(Board::BLACK_ROOKS, 7, 0);
    board->setPieceRF(Board::BLACK_ROOKS, 7, 7);
    board->setPieceRF(Board::BLACK_BISHOPS, 7, 2);
    board->setPieceRF(Board::BLACK_BISHOPS, 4, 2);
    board->setPieceRF(Board::BLACK_KING, 7, 4);
    board->setPieceRF(Board::BLACK_KNIGHTS, 5, 2);
    board->setPieceRF(Board::BLACK_KNIGHTS, 7, 6);
    board->setPieceRF(Board::BLACK_QUEEN, 1, 2);
    
    // Set up white pieces
    board->setPieceBin(Board::WHITE_PAWNS, 0b00000000000000000000000001000100100000010011100000000000);
    board->setPieceBin(Board::WHITE_ROOKS, 0b00000000000000000000000000000000000000000000000010000001);
    board->setPieceBin(Board::WHITE_KNIGHTS, 0b00000000000000000000000000000000000000000000000001000010);
    board->setPieceBin(Board::WHITE_BISHOPS, 0b00000000000000000000000000000000000000000000000000100100);
    board->setPieceRF(Board::WHITE_KING, 0, 4);

    // White to move
    board->applyMove(ai->findBestMove(board, WHITE).first);

    // Test that white is not in check
    ASSERT_FALSE(moveGen->isInCheck(*board, WHITE));
}

/**
 * This test recreates a custom position that caused a segfault.
 */
TEST_F(AITest, NoSegFault) {
    // Set up black pieces
    board->setPieceBin(Board::BLACK_PAWNS, 0b0000000001100010000000000001000000000000100000000000000000000000);
    board->setPieceRF(Board::BLACK_KNIGHTS, 4, 0);
    board->setPieceRF(Board::BLACK_KNIGHTS, 5, 4);
    board->setPieceRF(Board::BLACK_KING, 5, 5);
    board->setPieceRF(Board::BLACK_BISHOPS, 4, 5);
    board->setPieceRF(Board::BLACK_ROOKS, 3, 7);
    board->setPieceRF(Board::BLACK_ROOKS, 0, 5);
    board->setPieceRF(Board::BLACK_QUEEN, 1, 2);

    // Set up white pieces
    board->setPieceBin(Board::WHITE_PAWNS, 0b0000100100000000000000000000000000000000000000000000000000000000);
    board->setPieceRF(Board::WHITE_KING, 2, 4);
    // board->prettyPrint();

    // Black to move
    if (!moveGen->isGameOver(board)) {
        board->applyMove(ai->findBestMove(board, WHITE).first);
        // board->prettyPrint();
    } else {
        cout << "GAME OVER" << endl;
    }

    // Test that game is still running or something, idk
}

/**
 * Sets up position where black leaves queen hanging so best move should be for white to take.
 */
TEST_F(AITest, TakeHangingQueen) {
    board->setPieceBin(Board::BLACK_PAWNS, 0b0000000011110100000001010000100000000000000000000000000000000000);
    board->setPieceBin(Board::BLACK_ROOKS, 0b1000000100000000000000000000000000000000000000000000000000000000);
    board->setPieceBin(Board::BLACK_BISHOPS, 0b0010010000000000000000000000000000000000000000000000000000000000);
    board->setPieceRF(Board::BLACK_KING, 7, 4);
    board->setPieceRF(Board::BLACK_KNIGHTS, 7, 6);
    board->setPieceRF(Board::BLACK_QUEEN, 4, 4);

    board->setPieceBin(Board::WHITE_PAWNS, 0b0000000000000000000000000000000000000000000100001110111100000000);
    board->setPieceBin(Board::WHITE_ROOKS, 0b0000000000000000000000000000000000000000000000000000000010000001);
    board->setPieceRF(Board::WHITE_KING, 0, 4);
    board->setPieceRF(Board::WHITE_BISHOPS, 0, 2);
    board->setPieceRF(Board::WHITE_KNIGHTS, 0, 1);
    board->setPieceRF(Board::WHITE_QUEEN, 4, 7);

    pair<Move, int> aiResult = ai->findBestMove(board, WHITE);

    Move bestMove = aiResult.first;
    int bestMoveScore = aiResult.second;

    board->prettyPrint();

    tuple<int, int> fromPos = binIdxToGrid(bestMove.getOldPosition());
    tuple<int, int> toPos = binIdxToGrid(bestMove.getNewPosition());

    cout << "Best move: (" << get<0>(fromPos) << ", " << get<1>(fromPos) << ") -> (" << get<0>(toPos) << ", " << get<1>(toPos) << ") with score of " << bestMoveScore << endl; 

    // In this case, (4, 7) -> (4, 4) should be the best move as it takes the hanging queen.
    ASSERT_EQ(fromPos, make_tuple(4, 7));
    ASSERT_EQ(toPos, make_tuple(4, 4));
}

/**
 * Custom position where white leaves knight hanging, black to move (should take the hanging knight).
 */
TEST_F(AITest, TakeHangingKnight) {
    board->fillStandard();

    board->removePieceRF(Board::BLACK_PAWNS, 6, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 5, 4);
    board->removePieceRF(Board::BLACK_KNIGHTS, 7, 1);
    board->setPieceRF(Board::BLACK_KNIGHTS, 3, 3);

    board->removePieceRF(Board::WHITE_KNIGHTS, 0, 6);
    board->removePieceRF(Board::WHITE_KNIGHTS, 0, 1);
    board->setPieceRF(Board::WHITE_KNIGHTS, 4, 1);

    board->prettyPrint();

    Move best = ai->findBestMove(board, BLACK).first;

    cout << "Best Move: " << best.moveToString() << endl;
    tuple<int, int> fromPos = binIdxToGrid(best.getOldPosition());
    tuple<int, int> toPos = binIdxToGrid(best.getNewPosition());

    // In this case, (3, 3) -> (4, 1) should be the best move as it takes the hanging knight.
    ASSERT_EQ(fromPos, make_tuple(3, 3));
    ASSERT_EQ(toPos, make_tuple(4, 1));
}

/**
 * Sets up custom position where white has the option to take a pawn but should not as it would leave the knight hanging, 
 *  i.e. black would take it on the next move.
 * Tests that the minimax algorithm is working for a depth of 2.
 */
TEST_F(AITest, DoNotHangKnight) {
    board->fillStandard();
    
    board->removePieceRF(Board::BLACK_KNIGHTS, 7, 1);
    board->setPieceRF(Board::BLACK_KNIGHTS, 5, 2);
    board->removePieceRF(Board::BLACK_PAWNS, 6, 4);
    board->setPieceRF(Board::BLACK_PAWNS, 4, 4);

    board->setPieceBin(Board::WHITE_KNIGHTS, 0b0000000000000000000000000000000000000000001001000000000000000000);

    board->prettyPrint();

    Move best = ai->findBestMove(board, WHITE).first;

    cout << "Best Move: " << best.moveToString() << endl;

    tuple<int, int> fromPos = binIdxToGrid(best.getOldPosition());
    tuple<int, int> toPos = binIdxToGrid(best.getNewPosition());

    // Assert that the best move is NOT to take the pawn
    if (fromPos == make_tuple(2, 5)) {
        ASSERT_NE(toPos, make_tuple(4, 4));
    }
}