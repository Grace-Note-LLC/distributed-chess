#include <iostream>

#include "Board.h"
#include "Utils.h"
#include "Move.h"

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
void Board::prettyPrint() {
    vector<char> textBoard;
    for (int i = 0; i < 64; i++) textBoard.push_back(' ');

    for (Board::PieceIndex piece : pivec) {
        if (Board::getPiece(piece) == 0) continue;
        for (int i = 0; i < 64; i++) {
            uint64_t mask = 1ULL << i;
            if ((mask & Board::getPiece(piece)) > 0) {
                textBoard[i] = pieceAsASCII(piece); 
            }
        }
    }

    cout << "     " << "0 1 2 3 4 5 6 7" << endl;
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
    pieces[WHITE_PAWNS]   = 0x000000000000FF00;  
    pieces[BLACK_PAWNS]   = 0x00FF000000000000;  
    pieces[WHITE_ROOKS]   = 0x0000000000000081;  
    pieces[BLACK_ROOKS]   = 0x8100000000000000;  
    pieces[WHITE_KNIGHTS] = 0x0000000000000042;  
    pieces[BLACK_KNIGHTS] = 0x4200000000000000;  
    pieces[WHITE_BISHOPS] = 0x0000000000000024;  
    pieces[BLACK_BISHOPS] = 0x2400000000000000;  
    pieces[WHITE_QUEEN]   = 0x0000000000000008;  
    pieces[BLACK_QUEEN]   = 0x0800000000000000;  
    pieces[WHITE_KING]    = 0x0000000000000010;
    pieces[BLACK_KING]    = 0x1000000000000000;
}

bool Board::isInCheck(tileState color) {
        // Find the position of the king of the given color
        uint64_t kingPosition = getPiece((color == WHITE) ? Board::WHITE_KING : Board::BLACK_KING);

        // Generate all possible moves for the opponent
        tileState opponentColor = (color == WHITE) ? BLACK : WHITE;
        MoveGenerator moveGen(this);
        std::vector<Move> opponentMoves = moveGen.generateAllMoves(opponentColor);

        // Check if any of the opponent's moves can capture the king
        for (const auto& move : opponentMoves) {
            if (move.getNewPosition() == kingPosition) {
                return true;
            }
        }
        return false;
    }

/*
Check if the game is over.
*/
bool Board::isGameOver() {
    


    return false;
}