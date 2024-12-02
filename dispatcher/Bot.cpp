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
#include <queue>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>


#define MAX_DEPTH 3
#define NEG_INF -100000
#define POS_INF 100000

using namespace std;

/**
 * Finds the best move for the current player's turn using the minimax algorithm.
 * 
 * @param board: The current board state.
 * @param player: The player whose turn it is.
 * 
 * Returns: A pair containing (1) the calculated best move and (2) the evaluated score of the best move.
 */
pair<Move, int> ChessBot::findBestMove(Board* board, tileState player) {
    int bestScore = (player == WHITE) ? NEG_INF : POS_INF;
    Move bestMove;

    // Generate and prioritize moves
    vector<Move> possibleMoves = moveGen.generateAndFilterMoves(board, player);
    if (possibleMoves.empty()) {
        cout << "No moves available" << endl;
        return make_pair(bestMove, bestScore);
    }
    bestMove = possibleMoves[0];

    // Priority queue with a custom comparator
    auto comparator = [player](const pair<int, Move>& a, const pair<int, Move>& b) {
        return (player == WHITE) ? (a.first < b.first) : (a.first > b.first);
    };
    priority_queue<pair<int, Move>, vector<pair<int, Move>>, decltype(comparator)> moveQueue(comparator);

    for (const auto& move : possibleMoves) {
        Board copy = *board;
        copy.applyMove(move);
        int score = evaluateBoard(&copy, player, move.isCapture());
        moveQueue.push(make_pair(score, move));
    }

    // Mutex for thread-safe updates
    mutex scoreMutex;

    // Concurrency limit for parallelism
    size_t threadLimit = thread::hardware_concurrency();
    vector<future<void>> futures;
    size_t activeThreads = 0;

    while (!moveQueue.empty() || activeThreads > 0) {
        if (activeThreads < threadLimit && !moveQueue.empty()) {
            auto movePair = moveQueue.top();
            moveQueue.pop();

            futures.push_back(async(launch::async, [&, movePair]() {
                Board copy = *board;
                copy.applyMove(movePair.second);
                int score = minimax(&copy, MAX_DEPTH - 1, NEG_INF, POS_INF, player, movePair.second);

                {
                    lock_guard<mutex> lock(scoreMutex);
                    if ((player == WHITE && score > bestScore) || (player == BLACK && score < bestScore)) {
                        bestScore = score;
                        bestMove = movePair.second;
                    }
                }
            }));
            activeThreads++;
        }

        // Check completed threads
        for (auto it = futures.begin(); it != futures.end();) {
            if (it->wait_for(chrono::milliseconds(0)) == future_status::ready) {
                it->get(); // Wait for completion
                it = futures.erase(it); // Remove completed future
                activeThreads--;
            } else {
                ++it;
            }
        }
    }

    return make_pair(bestMove, bestScore);
}


int ChessBot::minimax(Board* board, int depth, int alpha, int beta, tileState player, Move m) {
    auto isOver = moveGen.isCheckmate(board, player);
    if (depth == 0 || isOver) { return evaluateBoard(board, player, m.isCapture()); }

    vector<Move> possibleMoves = moveGen.generateAllMoves(*board, player);

    if (player == WHITE) {
        int maxEval = NEG_INF;
        for (const Move& move : possibleMoves) {
            Board boardCopy = *board;
            boardCopy.applyMove(move);
            int eval = minimax(&boardCopy, depth - 1, alpha, beta, BLACK, move);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) { break; }
        }
        return maxEval;
    } else {
        int minEval = POS_INF;
        for (const Move& move : possibleMoves) {
            Board boardCopy = *board;
            boardCopy.applyMove(move);
            int eval = minimax(&boardCopy, depth - 1, alpha, beta, WHITE, move);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) { break; }
        }
        return minEval;
    }
}

int ChessBot::evaluateBoard(Board* board, tileState player, bool isCapture) {
    // Check for checkmate or stalemate
    if (moveGen.isCheckmate(board, WHITE)) { 
        return NEG_INF; 
    }
    if (moveGen.isCheckmate(board, BLACK)) { 
        return POS_INF; 
    }

    auto whiteMoves = moveGen.generateAndFilterMoves(board, WHITE);
    auto blackMoves = moveGen.generateAndFilterMoves(board, BLACK);
    
    // Stalemate, neutral position
    if (whiteMoves.size() == 0 && blackMoves.size() == 0) {
        cout << "Stalemate" << endl;
        return 0;  
    }

    // Material balance, Mobility
    int score = 0;
    int whiteMaterial = 0;
    int blackMaterial = 0;
    whiteMaterial += board->getPieceCount(Board::WHITE_PAWNS) * 2;
    whiteMaterial += board->getPieceCount(Board::WHITE_KNIGHTS) * 3;
    whiteMaterial += board->getPieceCount(Board::WHITE_BISHOPS) * 4;
    whiteMaterial += board->getPieceCount(Board::WHITE_ROOKS) * 6;
    whiteMaterial += board->getPieceCount(Board::WHITE_QUEEN) * 8;
    score += whiteMoves.size();

    blackMaterial -= board->getPieceCount(Board::BLACK_PAWNS) * 2;
    blackMaterial -= board->getPieceCount(Board::BLACK_KNIGHTS) * 3;
    blackMaterial -= board->getPieceCount(Board::BLACK_BISHOPS) * 4;
    blackMaterial -= board->getPieceCount(Board::BLACK_ROOKS) * 6;
    blackMaterial -= board->getPieceCount(Board::BLACK_QUEEN) * 8;
    score -= blackMoves.size();

    auto mobilityRatio = make_pair(10, 6);
    if (player == WHITE) {
        score += mobilityRatio.first * whiteMaterial + mobilityRatio.second * blackMaterial;
    } else {
        score += mobilityRatio.first * blackMaterial + mobilityRatio.second * whiteMaterial;
    }

    // Favor moves that capture pieces
    if (isCapture) {
        score += (player == WHITE) ? 100 : -100; 
    }

    // Piece-square tables
    for (int i = 0; i < 12; i++) {
        Board::PieceIndex piece = static_cast<Board::PieceIndex>(i);
        uint64_t pieceIndex = board->getPiece(piece);
        if (pieceIndex == 0) continue; // Do not count score if no pieces exist
        tuple<int, int> rankfile = binIdxToGrid(pieceIndex);
        int rank = get<0>(rankfile);
        int file = get<1>(rankfile);
        score += pieceSquareValue(piece, rank, file);
    }

    return score;
}

std::vector<uint64_t> packageEvalParams(Board* board, tileState player, bool isCapture) {
    std::vector<uint64_t> boardState;
    for (int i = 0; i < 12; i++) {
        boardState.push_back(board->getPiece(static_cast<Board::PieceIndex>(i)));
    }
    boardState.push_back(player);
    boardState.push_back(isCapture);

    return boardState;
}

void ChessBot::detectSPIDevices() {
    std::vector<std::string> detectedDevices = {"/dev/spidev0.0"};
    std::lock_guard<std::mutex> lock(deviceMutex);
    for (const auto& device : detectedDevices) {
        availableDevices.push(device);
    }
}

bool ChessBot::communicateWithDevice(const std::string& devicePath, const std::vector<uint64_t>& data, int& response) {
    int fd = open(devicePath.c_str(), O_RDWR);
    if (fd < 0) {
        std::cerr << "Error opening SPI device: " << strerror(errno) << std::endl;
        return false;
    }

    // SPI transfer setup (mock for now)
    struct spi_ioc_transfer transfer {};
    transfer.tx_buf = reinterpret_cast<uintptr_t>(data.data());
    transfer.rx_buf = reinterpret_cast<uintptr_t>(&response);
    transfer.len = static_cast<uint32_t>(data.size() * sizeof(uint64_t));

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
        std::cerr << "SPI transfer failed: " << strerror(errno) << std::endl;
        close(fd);
        return false;
    }

    close(fd);
    return true;
}

int ChessBot::distributedBoardEval(Board* board, tileState player, bool isCapture) {
    std::unique_lock<std::mutex> lock(deviceMutex);
    while (availableDevices.empty()) {
        std::cout << "No devices available, waiting..." << std::endl;
        deviceAvailable.wait(lock);
    }

    std::string device = availableDevices.front();
    availableDevices.pop();
    lock.unlock();

    int response;
    std::vector<uint64_t> data = packageEvalParams(board, player, isCapture);
    bool success = communicateWithDevice(device, data, response);

    lock.lock();
    availableDevices.push(device);
    lock.unlock();
    deviceAvailable.notify_one();

    if (success) {
        std::cout << "Received response from " << device << ": ";
        std::cout << response << " " << std::endl;
        return response;
    } else {
        std::cerr << "Failed to communicate with device: " << device << std::endl;
        return 0;
    }
}