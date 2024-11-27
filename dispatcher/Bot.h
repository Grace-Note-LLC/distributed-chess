#ifndef BOT_H
#define BOT_H

#include <limits>
#include <vector>
#include "Board.h"
#include "Move.h"


class ChessBot {
public:
    ChessBot(MoveGenerator moveGen) : moveGen(moveGen) {}

    std::pair<Move, int> findBestMove(Board* board, tileState player);

private:
    MoveGenerator moveGen;

    /*
    WHITE is always the maximizing player, and BLACK is the minimizing player.
    */
    int minimax(Board* boardCopy, int depth, int alpha, int beta, tileState player, Move m);

    int evaluateBoard(Board* board, tileState player, bool isCapture);
};

#endif // BOT_H