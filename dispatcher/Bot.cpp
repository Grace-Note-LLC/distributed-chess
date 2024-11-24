#include "Bot.h"
#include "Move.h"
#include "Board.h"
#include "ValueTables.h"

#include <limits>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <iostream>

#define MAX_DEPTH 3
#define NEG_INF -100000
#define POS_INF 100000

using namespace std;

pair<Move, int> ChessBot::findBestMove(Board* board, tileState player) {
    int bestScore = (player == WHITE) ? NEG_INF : POS_INF;
    Move bestMove;

    // Generate all possible moves for the current player
    vector<Move> possibleMoves = moveGen.generateAndFilterMoves(board, player);
    // if (possibleMoves.size() == 0) {
    //     cout << "No moves available" << endl;
    //     return make_pair(bestMove, bestScore);
    // }
    // for (auto& move : possibleMoves) {
    //     move.print();
    // }
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

    pair<Move, int> p = make_pair(bestMove, bestScore);
    return p;
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
            if (move.isCapture()) { eval += 90; }
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
            if (move.isCapture()) { eval -= 90; }
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

    // Piece-square tables
    for (int i = 0; i < 12; i++) {
        auto piece = static_cast<Board::PieceIndex>(i);
        auto pieceIndex = board->getPiece(piece);
        auto rankfile = binIdxToGrid(pieceIndex);
        int rank = get<0>(rankfile);
        int file = get<1>(rankfile);
        score += pieceSquareValue(piece, rank, file);
    }

    return (player == WHITE) ? score : -score;
}