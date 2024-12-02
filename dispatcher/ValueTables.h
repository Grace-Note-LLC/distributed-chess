#ifndef VALUETABLES_H
#define VALUETABLES_H

// White Pawn Table
static int whitePawnTable[8][8] = {
    {  0,  0,  0,   0,   0,  0,  0,  0 },
    {  0,  0, -2, -10, -10, -2,  0,  0 },
    {  1,  2,  3,   0,   0,  3,  2,  1 },
    {  5, 10, 10,  20,  20, 10, 10,  5 },
    {  0,  0,  0,  20,  20,  0,  0,  0 },
    {  5,  5, 10,  25,  25, 10,  5,  5 },
    { 10, 10, 20,  30,  30, 20, 10, 10 },
    { 50, 50, 50,  50,  50, 50, 50, 50 }
};

// Black Pawn Table (mirrored)
static int blackPawnTable[8][8] = {
    { 50, 50, 50,  50,  50, 50, 50, 50 },
    { 10, 10, 20,  30,  30, 20, 10, 10 },
    {  5,  5, 10,  25,  25, 10,  5,  5 },
    {  0,  0,  0,  20,  20,  0,  0,  0 },
    {  5, 10, 10,  20,  20, 10, 10,  5 },
    {  1,  2,  3,   0,   0,  3,  2,  1 },
    {  0,  0, -2, -10, -10, -2,  0,  0 },
    {  0,  0,  0,   0,   0,  0,  0,  0 }
};

// White Knight Table
static int whiteKnightTable[8][8] = {
    {-50, -40, -30, -30, -30, -30, -40, -50},  // Rank 1
    {-40, -20,   0,   0,   0,   0, -20, -40},  // Rank 2
    {-30,   0,  10,  15,  15,  10,   0, -30},  // Rank 3
    {-30,   5,  15,  20,  20,  15,   5, -30},  // Rank 4
    {-30,  10,  15,  20,  20,  15,  10, -30},  // Rank 5
    {-30,   0,  10,  15,  15,  10,   0, -30},  // Rank 6
    {-40, -20,   0,   5,   5,   0, -20, -40},  // Rank 7
    {-50, -40, -30, -30, -30, -30, -40, -50}   // Rank 8
};

// Black Knight Table (mirrored)
static int blackKnightTable[8][8] = {
    {-50, -40, -30, -30, -30, -30, -40, -50},  // Rank 8
    {-40, -20,   0,   5,   5,   0, -20, -40},  // Rank 7
    {-30,   0,  10,  15,  15,  10,   0, -30},  // Rank 6
    {-30,  10,  15,  20,  20,  15,  10, -30},  // Rank 5
    {-30,   5,  15,  20,  20,  15,   5, -30},  // Rank 4
    {-30,   0,  10,  15,  15,  10,   0, -30},  // Rank 3
    {-40, -20,   0,   0,   0,   0, -20, -40},  // Rank 2
    {-50, -40, -30, -30, -30, -30, -40, -50}   // Rank 1
};


// White Rook Table
static int whiteRookTable[8][8] = {
    {  0,   0,   0,   5,   5,   0,   0,   0 },  // Rank 1
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 2
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 3
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 4
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 5
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 6
    {  5,  10,  10,  10,  10,  10,  10,   5 },  // Rank 7
    {  0,   0,   0,   5,   5,   0,   0,   0 }   // Rank 8
};

// Black Rook Table
static int blackRookTable[8][8] = {
    {  0,   0,   0,   5,   5,   0,   0,   0 },  // Rank 8
    {  5,  10,  10,  10,  10,  10,  10,   5 },  // Rank 7
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 6
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 5
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 4
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 3
    { -5,   0,   0,   0,   0,   0,   0,  -5 },  // Rank 2
    {  0,   0,   0,   5,   5,   0,   0,   0 }   // Rank 1
};

// White Bishop Table
static int whiteBishopTable[8][8] = {
    {-20, -10, -10, -10, -10, -10, -10, -20},  // Rank 1
    {-10,   5,   0,   0,   0,   0,   5, -10},  // Rank 2
    {-10,  10,  10,  10,  10,  10,  10, -10},  // Rank 3
    {-10,   0,  10,  15,  15,  10,   0, -10},  // Rank 4
    {-10,   5,  15,  20,  20,  15,   5, -10},  // Rank 5
    {-10,  10,  10,  15,  15,  10,  10, -10},  // Rank 6
    {-10,   5,   0,   0,   0,   0,   5, -10},  // Rank 7
    {-20, -10, -10, -10, -10, -10, -10, -20}   // Rank 8
};

// Black Bishop Table
static int blackBishopTable[8][8] = {
    {-20, -10, -10, -10, -10, -10, -10, -20},  // Rank 8
    {-10,   5,   0,   0,   0,   0,   5, -10},  // Rank 7
    {-10,  10,  10,  15,  15,  10,  10, -10},  // Rank 6
    {-10,   5,  15,  20,  20,  15,   5, -10},  // Rank 5
    {-10,   0,  10,  15,  15,  10,   0, -10},  // Rank 4
    {-10,  10,  10,  10,  10,  10,  10, -10},  // Rank 3
    {-10,   5,   0,   0,   0,   0,   5, -10},  // Rank 2
    {-20, -10, -10, -10, -10, -10, -10, -20}   // Rank 1
};

static int whiteQueenTable[8][8] = {
    {-20, -10, -10,  -5,  -5, -10, -10, -20},  // Rank 1
    {-10,   0,   0,   0,   0,   0,   0, -10},  // Rank 2
    {-10,   0,   5,   5,   5,   5,   0, -10},  // Rank 3
    { -5,   0,   5,   5,   5,   5,   0,  -5},  // Rank 4
    { -5,   0,   5,   5,   5,   5,   0,  -5},  // Rank 5
    {-10,   5,   5,   5,   5,   5,   0, -10},  // Rank 6
    {-10,   0,   5,   0,   0,   0,   0, -10},  // Rank 7
    {-20, -10, -10,  -5,  -5, -10, -10, -20}   // Rank 8
};

static int blackQueenTable[8][8] = {
    {-20, -10, -10,  -5,  -5, -10, -10, -20},  // Rank 8
    {-10,   0,   0,   0,   0,   0,   0, -10},  // Rank 7
    {-10,   5,   5,   5,   5,   5,   0, -10},  // Rank 6
    { -5,   0,   5,   5,   5,   5,   0,  -5},  // Rank 5
    { -5,   0,   5,   5,   5,   5,   0,  -5},  // Rank 4
    {-10,   0,   5,   5,   5,   5,   0, -10},  // Rank 3
    {-10,   0,   0,   0,   0,   0,   0, -10},  // Rank 2
    {-20, -10, -10,  -5,  -5, -10, -10, -20}   // Rank 1
};

int pieceSquareValue(Board::PieceIndex piece, int rank, int file) {
    int score = 0;
    switch (piece) {
    case Board::WHITE_PAWNS:
        score += whitePawnTable[rank][file];
        break;
    case Board::BLACK_PAWNS:
        score -= blackPawnTable[rank][file];
        break;
    case Board::WHITE_KNIGHTS:
        score += whiteKnightTable[rank][file];
        break;
    case Board::BLACK_KNIGHTS:
        score -= blackKnightTable[rank][file];
        break;
    case Board::WHITE_ROOKS:
        score += whiteRookTable[rank][file];
        break;
    case Board::BLACK_ROOKS:
        score -= blackRookTable[rank][file];
        break;
    case Board::WHITE_BISHOPS:
        score += whiteBishopTable[rank][file];
        break;
    case Board::BLACK_BISHOPS:
        score -= blackBishopTable[rank][file];
        break;
    case Board::WHITE_QUEEN:
        score += whiteQueenTable[rank][file];
        break;
    case Board::BLACK_QUEEN:
        score -= blackQueenTable[rank][file];
        break;
    case Board::WHITE_KING:
    case Board::BLACK_KING:
        break;
    }

    return score;
}

#endif // VALUETABLES_H