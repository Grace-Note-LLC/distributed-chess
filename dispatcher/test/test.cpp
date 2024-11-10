#include <gtest/gtest.h>
#include <iostream>
#include <tuple>
#include <bitset>

#include "../Board.h"
#include "../Move.h"
#include "../Utils.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}