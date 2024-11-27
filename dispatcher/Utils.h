#ifndef UTILS_H
#define UTILS_H

#include <tuple>
#include <cstdint>
#include <iostream>
#pragma once

enum tileState {
    WHITE,
    BLACK,
    EMPTY
};

/*
Converts a binary index to a grid index (rank, file).

Function `__builtin_ctzll` counts the number of trailing 
zeros in a 64-bit integer. This is used to determine the
index of the piece in the board.
*/
inline std::tuple<int, int> binIdxToGrid(uint64_t bin) {
    if (bin == 0) throw std::invalid_argument("This piece does not exist on the board; cannot convert to grid index");
    
    int index = __builtin_ctzll(bin);
    return std::make_tuple(index >> 3, index % 8ULL);
}

inline uint64_t gridToBinIdx(int rank, int file) {
    return 1ULL << (rank * 8 + file);
}

inline uint64_t gridToBinIdx(std::tuple<int, int> twoDIndex) {
    return gridToBinIdx(std::get<0>(twoDIndex), std::get<1>(twoDIndex));
}

inline bool isOnBoard(int rank, int file) {
    return rank >= 0 && rank < 8 && file >= 0 && file < 8;
}

#endif // UTILS_H