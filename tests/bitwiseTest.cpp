#include <gtest/gtest.h>

#include "../common/bitwise.h"
#include "testUtils.h"

using namespace std;


TEST(bitwiseTest, masksTest) {
    GTEST_ASSERT_TRUE(checkBoard(lineMask(0), 0, 1, 2, 3, 4, 5, 6, 7));
    GTEST_ASSERT_TRUE(checkBoard(columnMask(0), 0, 8, 16, 24, 32, 40, 48, 56));
    GTEST_ASSERT_TRUE(checkBoard(diagonalMask(0), 0, 9, 18, 27, 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(antiDiagonalMask(0), 0));

    GTEST_ASSERT_TRUE(checkBoard(lineMask(7), 0, 1, 2, 3, 4, 5, 6, 7));
    GTEST_ASSERT_TRUE(checkBoard(columnMask(7), 7, 15, 23, 31, 39, 47, 55, 63));
    GTEST_ASSERT_TRUE(checkBoard(diagonalMask(7), 7));
    GTEST_ASSERT_TRUE(checkBoard(antiDiagonalMask(7), 7, 14, 21, 28, 35, 42, 49, 56));

    GTEST_ASSERT_TRUE(checkBoard(lineMask(56), 56, 57, 58, 59, 60, 61, 62, 63));
    GTEST_ASSERT_TRUE(checkBoard(columnMask(56), 0, 8, 16, 24, 32, 40, 48, 56));
    GTEST_ASSERT_TRUE(checkBoard(diagonalMask(56), 56));
    GTEST_ASSERT_TRUE(checkBoard(antiDiagonalMask(56), 7, 14, 21, 28, 35, 42, 49, 56));

    GTEST_ASSERT_TRUE(checkBoard(lineMask(63), 56, 57, 58, 59, 60, 61, 62, 63));
    GTEST_ASSERT_TRUE(checkBoard(columnMask(63), 7, 15, 23, 31, 39, 47, 55, 63));
    GTEST_ASSERT_TRUE(checkBoard(diagonalMask(63), 0, 9, 18, 27, 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(antiDiagonalMask(63), 63));
}

TEST(bitwiseTest, raysTest) {
    GTEST_ASSERT_TRUE(checkBoardNoPos(northRay(0)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noEastRay(0)));
    GTEST_ASSERT_TRUE(checkBoard(eastRay(0), 1, 2, 3, 4, 5, 6, 7));
    GTEST_ASSERT_TRUE(checkBoard(soEastRay(0), 9, 18, 27, 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(southRay(0), 8, 16, 24, 32, 40, 48, 56));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soWestRay(0)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(westRay(0)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noWestRay(0)));

    GTEST_ASSERT_TRUE(checkBoardNoPos(northRay(7)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noEastRay(7)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(eastRay(7)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soEastRay(7)));
    GTEST_ASSERT_TRUE(checkBoard(southRay(7), 15, 23, 31, 39, 47, 55, 63));
    GTEST_ASSERT_TRUE(checkBoard(soWestRay(7), 14, 21, 28, 35, 42, 49, 56));
    GTEST_ASSERT_TRUE(checkBoard(westRay(7), 0, 1, 2, 3, 4, 5, 6));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noWestRay(7)));

    GTEST_ASSERT_TRUE(checkBoard(northRay(56), 0, 8, 16, 24, 32, 40, 48));
    GTEST_ASSERT_TRUE(checkBoard(noEastRay(56), 7, 14, 21, 28, 35, 42, 49));
    GTEST_ASSERT_TRUE(checkBoard(eastRay(56), 57, 58, 59, 60, 61, 62, 63));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soEastRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(southRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soWestRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(westRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noWestRay(56)));

    GTEST_ASSERT_TRUE(checkBoard(northRay(63), 7, 15, 23, 31, 39, 47, 55));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noEastRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(eastRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soEastRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(southRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soWestRay(63)));
    GTEST_ASSERT_TRUE(checkBoard(westRay(63), 56, 57, 58, 59, 60, 61, 62));
    GTEST_ASSERT_TRUE(checkBoard(noWestRay(63), 0, 9, 18, 27, 36, 45, 54));
}

TEST(bitwiseTest, oneStepTest) {
	GTEST_ASSERT_TRUE(checkBoardNoPos(northOne(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEastOne(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoard(eastOne(posInd(0)), 1));
	GTEST_ASSERT_TRUE(checkBoard(soEastOne(posInd(0)), 9));
	GTEST_ASSERT_TRUE(checkBoard(southOne(posInd(0)), 8));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestOne(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(westOne(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWestOne(posInd(0))));

	GTEST_ASSERT_TRUE(checkBoardNoPos(northOne(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEastOne(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(eastOne(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEastOne(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoard(southOne(posInd(7)), 15));
	GTEST_ASSERT_TRUE(checkBoard(soWestOne(posInd(7)), 14));
	GTEST_ASSERT_TRUE(checkBoard(westOne(posInd(7)), 6));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWestOne(posInd(7))));

	GTEST_ASSERT_TRUE(checkBoard(northOne(posInd(56)), 48));
	GTEST_ASSERT_TRUE(checkBoard(noEastOne(posInd(56)), 49));
	GTEST_ASSERT_TRUE(checkBoard(eastOne(posInd(56)), 57));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEastOne(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(southOne(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestOne(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(westOne(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWestOne(posInd(56))));

	GTEST_ASSERT_TRUE(checkBoard(northOne(posInd(63)), 55));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEastOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(eastOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEastOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(southOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoard(westOne(posInd(63)), 62));
	GTEST_ASSERT_TRUE(checkBoard(noWestOne(posInd(63)), 54));
}

TEST(bitwiseTest, knightStepTest) {
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoEa(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEaEa(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoard(soEaEa(posInd(0)), 10));
	GTEST_ASSERT_TRUE(checkBoard(soSoEa(posInd(0)), 17));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoWe(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWeWe(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWeWe(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoWe(posInd(0))));

	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoEa(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEaEa(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEaEa(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoEa(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoWe(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWeWe(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoard(soWeWe(posInd(7)), 13));
	GTEST_ASSERT_TRUE(checkBoard(soSoWe(posInd(7)), 22));

	GTEST_ASSERT_TRUE(checkBoard(noNoEa(posInd(56)), 41));
	GTEST_ASSERT_TRUE(checkBoard(noEaEa(posInd(56)), 50));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEaEa(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoEa(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoWe(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWeWe(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWeWe(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoWe(posInd(56))));

	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoEa(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEaEa(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEaEa(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoEa(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoard(noNoWe(posInd(63)), 46));
	GTEST_ASSERT_TRUE(checkBoard(noWeWe(posInd(63)), 53));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWeWe(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoWe(posInd(63))));
}