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
    }

    Board* board;
    MoveGenerator* moveGen;
    ChessBot* ai;
};

TEST_F(AITest, FindBestMoveInInitialPosition) {
    board->fillStandard();
    Move bestMove = ai->findBestMove(board, WHITE);

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
    tileState player = BLACK;
    int iter = 0;
    while (!moveGen->isGameOver()) {
        Move bestMove = ai->findBestMove(board, player);
        board->applyMove(bestMove);
        player = (player == WHITE) ? BLACK : WHITE;
        cout << endl << "ITER: " << iter << endl;
        iter++;
        cout << "Best move for:" << (player == WHITE ? "WHITE" : "BLACK") << endl;
        bestMove.print();
        board->prettyPrint();
    }
    ASSERT_TRUE(moveGen->isGameOver());
}