#include "Board.h"
#include <iostream>

using namespace std;

// Constructor implementation
Board::Board() {
    for (int i = 0; i < 12; i++) pieces[i] = 0ULL;
}

/*
Returns the enum to a char
*/
char Board::pieceAsASCII(Board::PieceIndex index) {
    if (index == Board::WHITE_PAWNS)    return 'P';
    if (index == Board::BLACK_PAWNS)    return 'p';
    if (index == Board::WHITE_ROOKS)    return 'R';
    if (index == Board::BLACK_ROOKS)    return 'r';
    if (index == Board::WHITE_KNIGHTS)  return 'N';
    if (index == Board::BLACK_KNIGHTS)  return 'n';
    if (index == Board::WHITE_BISHOPS)  return 'B';
    if (index == Board::BLACK_BISHOPS)  return 'b';
    if (index == Board::WHITE_QUEEN)    return 'Q';
    if (index == Board::BLACK_QUEEN)    return 'q';
    if (index == Board::WHITE_KING)     return 'K';
    if (index == Board::BLACK_KING)     return 'k';
    return ' ';
}

/*
Pretty print the board.

Black is represented by lowercase letters, and white is represented 
by uppercase letters.

This can be used for debugging purposes.
*/
void Board::prettyPrint(Board board) {
    vector<char> textBoard;
    for (int i = 0; i < 64; i++) textBoard.push_back(' ');

    for (Board::PieceIndex piece : Board::getPieceIndices()) {
        if (Board::getPiece(piece) == 0) continue;
        for (int i = 0; i < 64; i++) {
            uint64_t mask = 1ULL << i;
            if ((mask & Board::getPiece(piece)) > 0) {
                textBoard[63 - i] = pieceAsASCII(piece); 
                // needs to be 63 - i because the board is printed from top to bottom
            }
        }
    }

    cout << "     " << "A B C D E F G H" << endl;
    cout << "     " << "---------------";
    for (int rank = 7; rank >= 0; rank--) {
        cout << endl << "  " << rank << "| ";
        for (int file = 0; file < 8; file++) {
            cout << textBoard[(rank * 8) + file] << " ";
        }
    }
    cout << endl;
}


/*
Generate the standard chess board.
*/
void Board::fillStandard() {
    std::vector<uint64_t>* pieces = Board::getPieces();
    (*pieces)[0] = 0x00FF000000000000;  // WHITE_PAWNS
    (*pieces)[1] = 0x000000000000FF00;  // BLACK_PAWNS
    (*pieces)[2] = 0x8100000000000000;  // WHITE_ROOKS
    (*pieces)[3] = 0x0000000000000081;  // BLACK_ROOKS
    (*pieces)[4] = 0x4200000000000000;  // WHITE_KNIGHTS
    (*pieces)[5] = 0x0000000000000042;  // BLACK_KNIGHTS
    (*pieces)[6] = 0x2400000000000000;  // WHITE_BISHOPS
    (*pieces)[7] = 0x0000000000000024;  // BLACK_BISHOPS
    (*pieces)[8] = 0x0800000000000000;  // WHITE_QUEEN
    (*pieces)[9] = 0x0000000000000008;  // BLACK_QUEEN
    (*pieces)[10] = 0x1000000000000000; // WHITE_KING
    (*pieces)[11] = 0x0000000000000010; // BLACK_KING
}
