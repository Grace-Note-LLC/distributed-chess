#include "Move.h"
#include "Board.h"


std::vector<Move> MoveGenerator::generatePieceMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    switch (pieceType) {
        case Board::WHITE_PAWNS:
        case Board::BLACK_PAWNS:
            moves = generatePawnMoves(pieceType);
            break;
        case Board::WHITE_ROOKS:
        case Board::BLACK_ROOKS:
            moves = generateRookMoves(pieceType);
            break;
        case Board::WHITE_KNIGHTS:
        case Board::BLACK_KNIGHTS:
            moves = generateKnightMoves(pieceType);
            break;
        case Board::WHITE_BISHOPS:
        case Board::BLACK_BISHOPS:
            moves = generateBishopMoves(pieceType);
            break;
        case Board::WHITE_QUEEN:
        case Board::BLACK_QUEEN:
            moves = generateQueenMoves(pieceType);
            break;
        case Board::WHITE_KING:
        case Board::BLACK_KING:
            moves = generateKingMoves(pieceType);
            break;
        default:
            break;
    }
    return moves;
}

bool MoveGenerator::isValidMove(const Move& move) {
    // Implement validation logic based on board state and piece rules
    return true; // Placeholder
}

void MoveGenerator::applyMove(const Move& move) {
    // Implement logic to update the board state with the given move
}

void MoveGenerator::undoMove(const Move& move) {
    // Implement logic to revert the board state to before the given move
}

std::vector<Move> MoveGenerator::generatePawnMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement pawn move generation logic
    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement rook move generation logic
    return moves;
}

std::vector<Move> MoveGenerator::generateKnightMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement knight move generation logic
    return moves;
}

std::vector<Move> MoveGenerator::generateBishopMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement bishop move generation logic
    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    // Implement queen move generation logic
    return moves;
}

std::vector<Move> MoveGenerator::generateKingMoves(Board::PieceIndex pieceType) {
    std::vector<Move> moves;
    if (pieceType == Board::WHITE_KING) {
        // get king position
        // generate 8 surrounding moves
        // check if cannot capture
        // return into moves

        uint64_t kingPiece = this->board->WHITE_KING;
        auto rankfile = binIdxToGrid(kingPiece);
        int rank = std::get<0>(rankfile);
        int file = std::get<1>(rankfile);
        tileState ourColor = static_cast<tileState>(pieceType % 2);

        // cardinals
        // right

        if (file != 7) {
            uint64_t proposedMove = this->board->WHITE_KING >> 1;
            auto occupant = getOccupant(pieceType, proposedMove);
        }


        // top
        if (rank != 7) {
            uint64_t proposedMove = this->board->WHITE_KING << 8;
        }
        
        // left
        if (file != 0) {
            uint64_t proposedMove = this->board->WHITE_KING >> 1;
        }
        // bottom
        if (rank != 0) {
            uint64_t proposedMove = this->board->WHITE_KING >> 8;
        }


        // diagonals

        



        return moves;
    }

    if (pieceType == Board::BLACK_KING) {

        return moves;
    }
}



MoveGenerator::tileState MoveGenerator::getOccupant(
    Board::PieceIndex pieceType, 
    uint64_t proposedMove) {

    // true = white
    // false = black
        
    for (int i = 0; i < 12; i++) {
        uint64_t piece = (this->board->getPieces())[i][0];
        if ((proposedMove & piece)) {
            return i % 2 == 0 ? WHITE : BLACK;
        }
    }
    
    return MoveGenerator::EMPTY;

}
