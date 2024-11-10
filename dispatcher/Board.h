#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

/*
Bit implementation of a chess board. Each piece is represented by a 64-bit integer, 
unfolded to an 8x8 grid. The MSB is the bottom left corner of the board (A1), and 
the LSB is the top right (H8). The board is represented as follows: 

Given a uint64_t,
    H7                                                            A0
        ____    ____    ____    ____    ____    ____    ____    ____    
    ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/

The board is represented as follows:

        A B C D E F G H
        ---------------
    7\  H G F E D C B A
    6\  P O N M L K J I
    5\  X W V U T S R Q
    4\  f e d c b a Z Y
    3\  n m l k j i h g
    2\  v u t s r q p o
    1\  3 2 1 0 z y x w
    0\  / + 9 8 7 6 5 4

For example, we can represent a black pawn on as 0x0000000000000FF0
in which the row (or rank) 2 is filled with 1s, indicating the presence of
a black pawn. The rest of the board is empty.
*/
#include <vector>

class Board {
public:
    enum PieceIndex {
        WHITE_PAWNS,
        BLACK_PAWNS,
        WHITE_ROOKS,
        BLACK_ROOKS,
        WHITE_KNIGHTS,
        BLACK_KNIGHTS,
        WHITE_BISHOPS,
        BLACK_BISHOPS,
        WHITE_QUEEN,
        BLACK_QUEEN,
        WHITE_KING,
        BLACK_KING
    };
    

    Board();  // Constructor declaration
    void fillStandard();
    void fillEmpty() { std::fill(std::begin(pieces), std::end(pieces), 0); }

    void prettyPrint();

    uint64_t* getPieces() { return pieces; }

    uint64_t& getPiece(PieceIndex index) { return pieces[index]; }
    const PieceIndex* getPieceIndices() { return pivec; }
    char pieceAsASCII(PieceIndex index);

    void setPieceBin(PieceIndex index, uint64_t piece) { pieces[index] = piece; }
    
    void setPieceRF(PieceIndex index, int rank, int file) { 
        pieces[index] |= 1ULL << (rank * 8 + file); 
    }

private:
    uint64_t pieces[12] = {};
    const PieceIndex pivec[12] = {
        WHITE_PAWNS, BLACK_PAWNS, WHITE_ROOKS, BLACK_ROOKS, WHITE_KNIGHTS, BLACK_KNIGHTS, WHITE_BISHOPS, BLACK_BISHOPS, WHITE_QUEEN, BLACK_QUEEN, WHITE_KING, BLACK_KING
    };
};

#endif // BOARD_H