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
	GTEST_ASSERT_TRUE(checkBoardNoPos(northOne(BoardUtils::posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEastOne(BoardUtils::posInd(0))));
	GTEST_ASSERT_TRUE(checkBoard(eastOne(BoardUtils::posInd(0)), 1));
	GTEST_ASSERT_TRUE(checkBoard(soEastOne(BoardUtils::posInd(0)), 9));
	GTEST_ASSERT_TRUE(checkBoard(southOne(BoardUtils::posInd(0)), 8));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestOne(BoardUtils::posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(westOne(BoardUtils::posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWestOne(BoardUtils::posInd(0))));

	GTEST_ASSERT_TRUE(checkBoardNoPos(northOne(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEastOne(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(eastOne(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEastOne(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoard(southOne(BoardUtils::posInd(7)), 15));
	GTEST_ASSERT_TRUE(checkBoard(soWestOne(BoardUtils::posInd(7)), 14));
	GTEST_ASSERT_TRUE(checkBoard(westOne(BoardUtils::posInd(7)), 6));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWestOne(BoardUtils::posInd(7))));

	GTEST_ASSERT_TRUE(checkBoard(northOne(BoardUtils::posInd(56)), 48));
	GTEST_ASSERT_TRUE(checkBoard(noEastOne(BoardUtils::posInd(56)), 49));
	GTEST_ASSERT_TRUE(checkBoard(eastOne(BoardUtils::posInd(56)), 57));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEastOne(BoardUtils::posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(southOne(BoardUtils::posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestOne(BoardUtils::posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(westOne(BoardUtils::posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWestOne(BoardUtils::posInd(56))));

	GTEST_ASSERT_TRUE(checkBoard(northOne(BoardUtils::posInd(63)), 55));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEastOne(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(eastOne(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEastOne(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(southOne(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestOne(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoard(westOne(BoardUtils::posInd(63)), 62));
	GTEST_ASSERT_TRUE(checkBoard(noWestOne(BoardUtils::posInd(63)), 54));
}

TEST(bitwiseTest, knightStepTest) {
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoEa(BoardUtils::posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEaEa(BoardUtils::posInd(0))));
	GTEST_ASSERT_TRUE(checkBoard(soEaEa(BoardUtils::posInd(0)), 10));
	GTEST_ASSERT_TRUE(checkBoard(soSoEa(BoardUtils::posInd(0)), 17));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoWe(BoardUtils::posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWeWe(BoardUtils::posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWeWe(BoardUtils::posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoWe(BoardUtils::posInd(0))));

	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoEa(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEaEa(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEaEa(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoEa(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoWe(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWeWe(BoardUtils::posInd(7))));
	GTEST_ASSERT_TRUE(checkBoard(soWeWe(BoardUtils::posInd(7)), 13));
	GTEST_ASSERT_TRUE(checkBoard(soSoWe(BoardUtils::posInd(7)), 22));

	GTEST_ASSERT_TRUE(checkBoard(noNoEa(BoardUtils::posInd(56)), 41));
	GTEST_ASSERT_TRUE(checkBoard(noEaEa(BoardUtils::posInd(56)), 50));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEaEa(BoardUtils::posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoEa(BoardUtils::posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoWe(BoardUtils::posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWeWe(BoardUtils::posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWeWe(BoardUtils::posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoWe(BoardUtils::posInd(56))));

	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoEa(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEaEa(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEaEa(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoEa(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoard(noNoWe(BoardUtils::posInd(63)), 46));
	GTEST_ASSERT_TRUE(checkBoard(noWeWe(BoardUtils::posInd(63)), 53));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWeWe(BoardUtils::posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoWe(BoardUtils::posInd(63))));
}