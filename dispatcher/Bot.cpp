#include "Bot.h"
#include "Move.h"
#include "Board.h"

#include <limits>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <iostream>
#include <algorithm>

#define MAX_DEPTH 3
#define NEG_INF -100000
#define POS_INF 100000

using namespace std;

vector<Move> ChessBot::generateAndFilterMoves(Board* board, tileState player) {
    MoveGenerator moveGen;
    std::vector<Move> possibleMoves = moveGen.generateAllMoves(*board, player);
    possibleMoves = moveGen.removeKingTargetingMoves(possibleMoves, 
        board->getPiece((player == WHITE) ? Board::BLACK_KING : Board::WHITE_KING)
    );
    possibleMoves = moveGen.removeMovesLeavingKingInCheck(board, possibleMoves, player);
    
    // Sort moves by captures first
    std::sort(possibleMoves.begin(), possibleMoves.end(), [](const Move& a, const Move& b) {
        return a.isCapture() && !b.isCapture();
    });
    return possibleMoves;
}


pair<Move, int> ChessBot::findBestMove(Board* board, tileState player) {
    int bestScore = (player == WHITE) ? NEG_INF : POS_INF;
    Move bestMove;

    // Generate all possible moves for the current player
    vector<Move> possibleMoves = generateAndFilterMoves(board, player);
    // Mutex to synchronize access to bestMove and bestScore
    std::mutex mutex;

    // Use futures to parallelize move evaluations
    std::vector<std::future<void>> futures;
    for (const auto& move : possibleMoves) {
        futures.push_back(std::async(std::launch::async, [&](Move currentMove) {
            Board copy = *board;
            copy.applyMove(currentMove);

            int score = minimax(&copy, MAX_DEPTH - 1, NEG_INF, POS_INF, (player == WHITE) ? BLACK : WHITE);

            // Lock to update the best score and move
            std::lock_guard<std::mutex> lock(mutex);
            if ((player == WHITE && score >= bestScore) || 
                (player == BLACK && score <= bestScore)) {

                bestScore = score;
                bestMove = currentMove;
            }
        }, move));
    }

    // Wait for all futures to complete
    for (auto& future : futures) {
        future.get();
    }

    return make_pair(bestMove, bestScore);
}

int ChessBot::minimax(Board* board, int depth, int alpha, int beta, tileState player) {
    auto isOver = moveGen.isCheckmate(board, player);
    if (depth == 0 || isOver) { return evaluateBoard(board, player); }

    std::vector<Move> possibleMoves = moveGen.generateAllMoves(*board, player);

    if (player == WHITE) {
        int maxEval = NEG_INF;
        for (const auto& move : possibleMoves) {
            Board boardCopy = *board;
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
            Board boardCopy = *board;
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
    if (moveGen.isCheckmate(board, WHITE)) { 
        return NEG_INF; 
    }
    if (moveGen.isCheckmate(board, BLACK)) { 
        return POS_INF; 
    }

    auto whiteMoves = moveGen.generateAllMoves(*board, WHITE);
    auto blackMoves = moveGen.generateAllMoves(*board, BLACK);
    
    // Stalemate, neutral position
    if (whiteMoves.size() == 0 && blackMoves.size() == 0) {
        cout << "Stalemate" << endl;
        return 0;  
    }

    // Material balance
    int score = 0;
    score += board->getPieceCount(Board::WHITE_PAWNS) * 10;
    score += board->getPieceCount(Board::WHITE_KNIGHTS) * 30;
    score += board->getPieceCount(Board::WHITE_BISHOPS) * 30;
    score += board->getPieceCount(Board::WHITE_ROOKS) * 50;
    score += board->getPieceCount(Board::WHITE_QUEEN) * 100;
    
    score -= board->getPieceCount(Board::BLACK_PAWNS) * 10;
    score -= board->getPieceCount(Board::BLACK_KNIGHTS) * 30;
    score -= board->getPieceCount(Board::BLACK_BISHOPS) * 30;
    score -= board->getPieceCount(Board::BLACK_ROOKS) * 50;
    score -= board->getPieceCount(Board::BLACK_QUEEN) * 100;
    
    // Mobility
    score += whiteMoves.size();
    score -= blackMoves.size();

    return (player == WHITE) ? score : -score;
}

int pieceSquareValue(Board::PieceIndex piece, int rank, int file) {
    // Example simple piece-square table for pawns
    static int pawnTable[8][8] = {
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 5,  5,  5,  5,  5,  5,  5,  5 },
        { 1,  1,  2,  3,  3,  2,  1,  1 },
        { 0,  0,  0,  2,  2,  0,  0,  0 },
        { 0,  0,  0, -2, -2,  0,  0,  0 },
        { 1, -1, -2,  0,  0, -2, -1,  1 },
        { 1,  2,  2, -2, -2,  2,  2,  1 },
        { 0,  0,  0,  0,  0,  0,  0,  0 }
    };
    if (piece == Board::WHITE_PAWNS || piece == Board::BLACK_PAWNS) {
        return pawnTable[rank][file];
    }
    // Implement tables for other pieces similarly
    return 0;
}