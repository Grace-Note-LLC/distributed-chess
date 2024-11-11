#include "Bot.h"
#include "Move.h"
#include "Board.h"

#include <iostream>
#include <algorithm>

using namespace std;

Move ChessBot::findBestMove(Board* board, tileState player) {
    int bestScore = (player == WHITE) ? NEG_INF : POS_INF;
    Move bestMove;
    
    // Generate all possible moves for the current player
    std::vector<Move> possibleMoves = moveGen.generateAllMoves(*board, player);
    cout << "Possible moves: " << possibleMoves.size() << endl;
    if (possibleMoves.size() == 1) return possibleMoves[0];

    for (const auto& move : possibleMoves) {
        Board copy = *board;
        copy.applyMove(move);

        int score = minimax(&copy, MAX_DEPTH - 1, NEG_INF, POS_INF, (player == WHITE) ? BLACK : WHITE);
        
        if ((player == WHITE && score > bestScore) || (player == BLACK && score < bestScore)) {
            bestScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}

int ChessBot::minimax(Board* board, int depth, int alpha, int beta, tileState player) {
    if (depth == 0 || moveGen.isCheckmate(board, player)) return evaluateBoard(board, player);
    Board boardCopy = *board;

    std::vector<Move> possibleMoves = moveGen.generateAllMoves(*board, player);

    if (player == WHITE) {
        int maxEval = NEG_INF;
        for (const auto& move : possibleMoves) {
            boardCopy.applyMove(move);
            int eval = minimax(&boardCopy, depth - 1, alpha, beta, BLACK);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) { break; }
        }
        return maxEval;
    } else {
        int minEval = POS_INF;
        for (const auto& move : possibleMoves) {
            boardCopy.applyMove(move);
            int eval = minimax(&boardCopy, depth - 1, alpha, beta, WHITE);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) { break; }
        }
        return minEval;
    }
}

int ChessBot::evaluateBoard(Board* board, tileState player) {
    // Check for checkmate or stalemate
    if (moveGen.isCheckmate(board, WHITE)) { return NEG_INF; }
    if (moveGen.isCheckmate(board, BLACK)) { return POS_INF; }

    auto whiteMoves = moveGen.generateAllMoves(*board, WHITE);
    auto blackMoves = moveGen.generateAllMoves(*board, BLACK);
    
    // Stalemate, neutral position
    if (whiteMoves.size() == 0 && blackMoves.size() == 0) {
        cout << "Stalemate" << endl;
        return 0;  
    }

    // Material balance
    int score = 0;
    score += board->getPieceCount(Board::WHITE_PAWNS) * 2;
    score += board->getPieceCount(Board::WHITE_KNIGHTS) * 9;
    score += board->getPieceCount(Board::WHITE_BISHOPS) * 9;
    score += board->getPieceCount(Board::WHITE_ROOKS) * 25;
    score += board->getPieceCount(Board::WHITE_QUEEN) * 81;
    
    score -= board->getPieceCount(Board::BLACK_PAWNS) * 2;
    score -= board->getPieceCount(Board::BLACK_KNIGHTS) * 9;
    score -= board->getPieceCount(Board::BLACK_BISHOPS) * 9;
    score -= board->getPieceCount(Board::BLACK_ROOKS) * 25;
    score -= board->getPieceCount(Board::BLACK_QUEEN) * 81;
    
    // Mobility
    score += whiteMoves.size() * 0.05;
    score -= blackMoves.size() * 0.05;

    // Check if castled

    // Check for positional advantage



    return (player == WHITE) ? score : -score;
}