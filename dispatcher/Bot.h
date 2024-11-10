#ifndef BOT_H
#define BOT_H

#include <limits>
#include <vector>
#include "Board.h"
#include "Move.h"

// Define the depth of the minimax search
#define MAX_DEPTH 4

class ChessBot {
public:
    ChessBot(Board* board) : board(board) {}

    Move findBestMove() {
        int bestScore = std::numeric_limits<int>::min();
        Move bestMove;
        
        // Generate all possible moves for the current player
        MoveGenerator moveGen(board);
        std::vector<Move> possibleMoves = moveGen.generateAllMoves();

        // Evaluate each move using minimax
        for (const auto& move : possibleMoves) {
            board->makeMove(move);
            int score = minimax(MAX_DEPTH - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
            board->undoMove(move);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
        return bestMove;
    }

private:
    Board* board;

    int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer) {
        if (depth == 0 || board->isGameOver()) {
            return evaluateBoard();
        }

        MoveGenerator moveGen(board);
        std::vector<Move> possibleMoves = moveGen.generateAllMoves();

        if (isMaximizingPlayer) {
            int maxEval = std::numeric_limits<int>::min();
            for (const auto& move : possibleMoves) {
                board->makeMove(move);
                int eval = minimax(depth - 1, alpha, beta, false);
                board->undoMove(move);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break; // Beta cut-off
                }
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max();
            for (const auto& move : possibleMoves) {
                board->makeMove(move);
                int eval = minimax(depth - 1, alpha, beta, true);
                board->undoMove(move);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break; // Alpha cut-off
                }
            }
            return minEval;
        }
    }

    int evaluateBoard() {
        // A simple evaluation function that calculates the material balance.
        int score = 0;
        score += board->getPieceCount(Board::WHITE_PAWNS) * 1;
        score += board->getPieceCount(Board::WHITE_KNIGHTS) * 3;
        score += board->getPieceCount(Board::WHITE_BISHOPS) * 3;
        score += board->getPieceCount(Board::WHITE_ROOKS) * 5;
        score += board->getPieceCount(Board::WHITE_QUEEN) * 9;
        
        score -= board->getPieceCount(Board::BLACK_PAWNS) * 1;
        score -= board->getPieceCount(Board::BLACK_KNIGHTS) * 3;
        score -= board->getPieceCount(Board::BLACK_BISHOPS) * 3;
        score -= board->getPieceCount(Board::BLACK_ROOKS) * 5;
        score -= board->getPieceCount(Board::BLACK_QUEEN) * 9;
        
        return (board->getCurrentTurn() == Board::WHITE) ? score : -score;
    }
};

#endif // BOT_H