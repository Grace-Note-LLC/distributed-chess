#include <iostream>
#include "Board.h"

int main() {
    
    // define a board struct with 12 uint64
    // define the starting board
    // define what it means to move a piece
    // define valid moves 
    // function to expand all possible moves
    Board board;
    board.fillStandard();
    board.prettyPrint(board);
    
    return 0;
}