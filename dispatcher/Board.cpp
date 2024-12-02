#include <iostream>

#include "Board.h"
#include "Utils.h"
#include "Move.h"

using namespace std;


#define ANSI_DARK_YELLOW_BACKGROUND "\e[48;5;250m"
#define ANSI_BRIGHT_YELLOW_BACKGROUND "\e[48;5;255m"
#define ANSI_WHITE_FOREGROUND "\e[38;5;15m"
#define ANSI_BLACK_FOREGROUND "\e[38;5;0m"
#define ANSI_ESCAPE "\033[0m"



// Constructor implementation
Board::Board() {
    for (auto piece : pivec) pieces[piece] = 0ULL;
}

/*
Returns the enum to a char
*/
string Board::pieceAsASCII(Board::PieceIndex index) {
    if (index == Board::WHITE_PAWNS)    return "\u2659";
    if (index == Board::BLACK_PAWNS)    return "\u265F";
    if (index == Board::WHITE_ROOKS)    return "\u2656";
    if (index == Board::BLACK_ROOKS)    return "\u265C";
    if (index == Board::WHITE_KNIGHTS)  return "\u2658";
    if (index == Board::BLACK_KNIGHTS)  return "\u265E";
    if (index == Board::WHITE_BISHOPS)  return "\u2657";
    if (index == Board::BLACK_BISHOPS)  return "\u265D";
    if (index == Board::WHITE_QUEEN)    return "\u2655";
    if (index == Board::BLACK_QUEEN)    return "\u265B";
    if (index == Board::WHITE_KING)     return "\u2654";
    if (index == Board::BLACK_KING)     return "\u265A";
    return " ";
}

/*
Pretty print the board.

Black is represented by lowercase letters, and white is represented 
by uppercase letters.

This can be used for debugging purposes.
*/
void Board::prettyPrint() {
    vector<string> textBoard;
    for (int i = 0; i < 64; i++) textBoard.push_back(" ");

    for (Board::PieceIndex piece : pivec) {
        if (Board::getPiece(piece) == 0) continue;
        for (int i = 0; i < 64; i++) {
            uint64_t mask = 1ULL << i;
            if ((mask & Board::getPiece(piece)) > 0) {
                textBoard[i] = ((piece % 2) ? ANSI_WHITE_FOREGROUND : ANSI_BLACK_FOREGROUND) + pieceAsASCII(piece); 
            }
        }
    }

    cout << "    " << "\033[4m" << "\uFF10\uFF11\uFF12\uFF13\uFF14\uFF15\uFF16\uFF17" <<  ANSI_ESCAPE << endl;

    for (int rank = 7; rank >= 0; rank--) {
        cout << "  " << rank << "|";
        for (int file = 0; file < 8; file++) {
            string backgroundColor = ((!(file % 2) && (rank % 2)) || ((file % 2) && !(rank % 2))) ? ANSI_BRIGHT_YELLOW_BACKGROUND : ANSI_DARK_YELLOW_BACKGROUND;
            cout << backgroundColor << textBoard[(rank * 8) + file] << " " << ANSI_ESCAPE;
        }
        cout << "|" << endl;
    }
    cout << "    " << "\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E" << endl;
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

void Board::applyMove(Move move) {
    // Implement logic to update the board state based on the given move
    uint64_t currPosition = this->getPiece(move.getPieceType());
    uint64_t oldPosition = move.getOldPosition();
    uint64_t newPosition = move.getNewPosition();

    currPosition ^= oldPosition; // removes old position
    currPosition |= newPosition; // adds new position

    this->setPieceBin(move.getPieceType(), currPosition);

    if (move.getPieceType() == WHITE_PAWNS && (newPosition & 0xFF00000000000000)) {
        pieces[WHITE_QUEEN] |= newPosition;
        pieces[WHITE_PAWNS] ^= newPosition;
    }
    if (move.getPieceType() == BLACK_PAWNS && (newPosition & 0x00000000000000FF)) {
        pieces[BLACK_QUEEN] |= newPosition;
        pieces[BLACK_PAWNS] ^= newPosition;
    }

    // remove the piece from the board
    if (!move.isCapture()) return;
    for (auto piece : pivec) {
        if (pieces[piece] & newPosition && piece != move.getPieceType()) {
            // cout << "Captured piece: " << piece << endl;
            // cout << "old position: " << pieces[piece] << endl;
            // cout << "new position: " << newPosition << endl;
            pieces[piece] ^= newPosition;
            break;
        }
    }
}