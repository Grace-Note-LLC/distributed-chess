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
    ChessBot(MoveGenerator moveGen) : moveGen(moveGen) {}

    Move findBestMove(Board* board, tileState player);

private:
    MoveGenerator moveGen;

    /*
    WHITE is always the maximizing player, and BLACK is the minimizing player.
    */
    int minimax(Board* boardCopy, int depth, int alpha, int beta, tileState player);

    int evaluateBoard(Board* board, tileState player);
};

#endif // BOT_H