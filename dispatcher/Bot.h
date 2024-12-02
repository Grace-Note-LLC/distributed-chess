#ifndef BOT_H
#define BOT_H

#include <limits>
#include <vector>
#include <queue>
#include <condition_variable>
#include "Board.h"
#include "Move.h"


class ChessBot {
public:
    ChessBot(MoveGenerator moveGen) : moveGen(moveGen) {}

    std::pair<Move, int> findBestMove(Board* board, tileState player);
    void detectSPIDevices();
    bool communicateWithDevice(
        const std::string& devicePath, 
        const std::vector<uint64_t>& data, 
        int& response
    );
    int distributedBoardEval(Board* board, tileState player, bool isCapture);

private:
    MoveGenerator moveGen;
    std::mutex deviceMutex;
    std::condition_variable deviceAvailable;
    std::queue<std::string> availableDevices;
    /*
    WHITE is always the maximizing player, and BLACK is the minimizing player.
    */
    int minimax(Board* boardCopy, int depth, int alpha, int beta, tileState player, Move m);

    int evaluateBoard(Board* board, tileState player, bool isCapture);
};

#endif // BOT_H