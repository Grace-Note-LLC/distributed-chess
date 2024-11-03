#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

/*
Bit implementation of a chess board. Each piece is represented by a 64-bit integer, 
unfolded to an 8x8 grid. The MSB is the bottom left corner of the board (A1), and 
the LSB is the top right (H8). The board is represented as follows: 

Given a uint64_t,
    A1                                                            H8
        ____    ____    ____    ____    ____    ____    ____    ____    
    ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/

The board is represented as follows:

        A B C D E F G H
        ---------------
    8\  4 5 6 7 8 9 + /
    7\  w x y z 0 1 2 3
    6\  o p q r s t u v
    5\  g h i j k l m n
    4\  Y Z a b c d e f
    3\  Q R S T U V W X
    2\  I J K L M N O P
    1\  A B C D E F G H

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
    void fillEmpty() { pieces = std::vector<uint64_t>(12, 0); }

    void prettyPrint();

    std::vector<uint64_t>* getPieces() { return &pieces; }

    uint64_t& getPiece(PieceIndex index) { return pieces[index]; }
    std::vector<PieceIndex> getPieceIndices() { return pivec; }
    char pieceAsASCII(PieceIndex index);

    void setPieceBin(PieceIndex index, uint64_t piece) { pieces[index] = piece; }
    
    void setPieceRF(PieceIndex index, int rank, int file) { 
        pieces[index] |= 1ULL << (63 - (rank * 8 + file)); 
    }

private:
    std::vector<uint64_t> pieces = std::vector<uint64_t>(12, 0);
    std::vector<PieceIndex> pivec = {
        WHITE_PAWNS, BLACK_PAWNS, WHITE_ROOKS, BLACK_ROOKS, WHITE_KNIGHTS, BLACK_KNIGHTS, WHITE_BISHOPS, BLACK_BISHOPS, WHITE_QUEEN, BLACK_QUEEN, WHITE_KING, BLACK_KING
    };
};

#endif // BOARD_H