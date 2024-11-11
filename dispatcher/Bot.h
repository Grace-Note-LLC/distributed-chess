#ifndef BOT_H
#define BOT_H

#include <limits>
#include <vector>
#include "Board.h"
#include "Move.h"

// Define the depth of the minimax search
#define MAX_DEPTH 4
#define NEG_INF numeric_limits<int>::min()
#define POS_INF numeric_limits<int>::max()


class ChessBot {
public:
    ChessBot(Board* board, MoveGenerator moveGen) : board(board), moveGen(moveGen) {}

    Move findBestMove() {
        int bestScore = NEG_INF;
        Move bestMove;
        
        // Generate all possible moves for the current player
        std::vector<Move> possibleMoves = moveGen.generateAllMoves(*board, WHITE);
        if (possibleMoves.size() == 1) return possibleMoves[0];

        for (const auto& move : possibleMoves) {
            Board copy = *board;
            copy.applyMove(move);

            int score = minimax(*board, MAX_DEPTH - 1, NEG_INF, POS_INF, WHITE);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
        return bestMove;
    }

private:
    Board* board;
    MoveGenerator moveGen;

    /*
    WHITE is always the maximizing player, and BLACK is the minimizing player.
    */
    int minimax(Board boardCopy, int depth, int alpha, int beta, tileState player) {
        if (depth == 0 || moveGen.isCheckmate(board, player)) return evaluateBoard(player);

        std::vector<Move> possibleMoves = moveGen.generateAllMoves(boardCopy, player);

        if (player == WHITE) {
            int maxEval = NEG_INF;
            for (const auto& move : possibleMoves) {
                boardCopy.applyMove(move);
                int eval = minimax(boardCopy, depth - 1, alpha, beta, BLACK);
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
                boardCopy.applyMove(move);
                int eval = minimax(boardCopy, depth - 1, alpha, beta, WHITE);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break; // Alpha cut-off
                }
            }
            return minEval;
        }
    }

    int evaluateBoard(tileState player) {
        // Check for checkmate or stalemate
        if (moveGen.isCheckmate(board, WHITE)) { return NEG_INF; }
        if (moveGen.isCheckmate(board, BLACK)) { return POS_INF; }

        auto whiteMoves = moveGen.generateAllMoves(*board, WHITE);
        auto blackMoves = moveGen.generateAllMoves(*board, BLACK);
        
        // Stalemate, neutral position
        if (whiteMoves.size() == 0 && blackMoves.size() == 0) {
            return 0;  
        }

        // Material balance
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
        
        // Mobility
        score += whiteMoves.size() * 0.1;
        score -= blackMoves.size() * 0.1;

        // Check if castled

        // Check for positional advantage

        return (player == WHITE) ? score : -score;
    }
};

#endif // BOT_H