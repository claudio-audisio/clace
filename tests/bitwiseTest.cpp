#include <gtest/gtest.h>

#include "../common/bitwise.h"
#include "testUtils.h"

using namespace std;

class BitwiseTest : public testing::Test {
protected:
	BitwiseTest() {

	}
	~BitwiseTest() {

	}
};

TEST_F(BitwiseTest, masksTest) {
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

	GTEST_ASSERT_TRUE(checkBoard(lineMask(35), 32, 33, 34, 35, 36, 37, 38, 39));
	GTEST_ASSERT_TRUE(checkBoard(columnMask(35), 3, 11, 19, 27, 35, 43, 51, 59));
	GTEST_ASSERT_TRUE(checkBoard(diagonalMask(35), 8, 17, 26, 35, 44, 53, 62));
	GTEST_ASSERT_TRUE(checkBoard(antiDiagonalMask(35), 7, 14, 21, 28, 35, 42, 49, 56));
}

TEST_F(BitwiseTest, raysTest) {
    GTEST_ASSERT_TRUE(checkBoardNoPos(southRay(0)));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestRay(0)));
    GTEST_ASSERT_TRUE(checkBoard(eastRay(0), 1, 2, 3, 4, 5, 6, 7));
    GTEST_ASSERT_TRUE(checkBoard(noEastRay(0), 9, 18, 27, 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(northRay(0), 8, 16, 24, 32, 40, 48, 56));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soEastRay(0)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(westRay(0)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noWestRay(0)));

    GTEST_ASSERT_TRUE(checkBoardNoPos(southRay(7)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noEastRay(7)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(eastRay(7)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soWestRay(7)));
    GTEST_ASSERT_TRUE(checkBoard(northRay(7), 15, 23, 31, 39, 47, 55, 63));
    GTEST_ASSERT_TRUE(checkBoard(noWestRay(7), 14, 21, 28, 35, 42, 49, 56));
    GTEST_ASSERT_TRUE(checkBoard(westRay(7), 0, 1, 2, 3, 4, 5, 6));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soEastRay(7)));

    GTEST_ASSERT_TRUE(checkBoard(southRay(56), 0, 8, 16, 24, 32, 40, 48));
    GTEST_ASSERT_TRUE(checkBoard(soEastRay(56), 7, 14, 21, 28, 35, 42, 49));
    GTEST_ASSERT_TRUE(checkBoard(eastRay(56), 57, 58, 59, 60, 61, 62, 63));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noEastRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(northRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soWestRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(westRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noWestRay(56)));

    GTEST_ASSERT_TRUE(checkBoard(southRay(63), 7, 15, 23, 31, 39, 47, 55));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noEastRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(eastRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(soEastRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(northRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(noWestRay(63)));
    GTEST_ASSERT_TRUE(checkBoard(westRay(63), 56, 57, 58, 59, 60, 61, 62));
    GTEST_ASSERT_TRUE(checkBoard(soWestRay(63), 0, 9, 18, 27, 36, 45, 54));

	GTEST_ASSERT_TRUE(checkBoard(southRay(35), 3, 11, 19, 27));
	GTEST_ASSERT_TRUE(checkBoard(soEastRay(35), 7, 14, 21, 28));
	GTEST_ASSERT_TRUE(checkBoard(eastRay(35), 36, 37, 38, 39));
	GTEST_ASSERT_TRUE(checkBoard(noEastRay(35), 44, 53, 62));
	GTEST_ASSERT_TRUE(checkBoard(northRay(35), 43, 51, 59));
	GTEST_ASSERT_TRUE(checkBoard(noWestRay(35), 42, 49, 56));
	GTEST_ASSERT_TRUE(checkBoard(westRay(35), 32, 33, 34));
	GTEST_ASSERT_TRUE(checkBoard(soWestRay(35), 8, 17, 26));
}

TEST_F(BitwiseTest, oneStepTest) {
	GTEST_ASSERT_TRUE(checkBoardNoPos(southOne(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEastOne(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoard(eastOne(posInd(0)), 1));
	GTEST_ASSERT_TRUE(checkBoard(noEastOne(posInd(0)), 9));
	GTEST_ASSERT_TRUE(checkBoard(northOne(posInd(0)), 8));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWestOne(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(westOne(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestOne(posInd(0))));

	GTEST_ASSERT_TRUE(checkBoardNoPos(southOne(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEastOne(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(eastOne(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEastOne(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoard(northOne(posInd(7)), 15));
	GTEST_ASSERT_TRUE(checkBoard(noWestOne(posInd(7)), 14));
	GTEST_ASSERT_TRUE(checkBoard(westOne(posInd(7)), 6));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestOne(posInd(7))));

	GTEST_ASSERT_TRUE(checkBoard(southOne(posInd(56)), 48));
	GTEST_ASSERT_TRUE(checkBoard(soEastOne(posInd(56)), 49));
	GTEST_ASSERT_TRUE(checkBoard(eastOne(posInd(56)), 57));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEastOne(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(northOne(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWestOne(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(westOne(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWestOne(posInd(56))));

	GTEST_ASSERT_TRUE(checkBoard(southOne(posInd(63)), 55));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEastOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(eastOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEastOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(northOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWestOne(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoard(westOne(posInd(63)), 62));
	GTEST_ASSERT_TRUE(checkBoard(soWestOne(posInd(63)), 54));

	GTEST_ASSERT_TRUE(checkBoard(southOne(posInd(35)), 27));
	GTEST_ASSERT_TRUE(checkBoard(soEastOne(posInd(35)), 28));
	GTEST_ASSERT_TRUE(checkBoard(eastOne(posInd(35)), 36));
	GTEST_ASSERT_TRUE(checkBoard(noEastOne(posInd(35)), 44));
	GTEST_ASSERT_TRUE(checkBoard(northOne(posInd(35)), 43));
	GTEST_ASSERT_TRUE(checkBoard(noWestOne(posInd(35)), 42));
	GTEST_ASSERT_TRUE(checkBoard(westOne(posInd(35)), 34));
	GTEST_ASSERT_TRUE(checkBoard(soWestOne(posInd(35)), 26));
}

TEST_F(BitwiseTest, knightStepTest) {
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoEa(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEaEa(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoard(noEaEa(posInd(0)), 10));
	GTEST_ASSERT_TRUE(checkBoard(noNoEa(posInd(0)), 17));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoWe(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWeWe(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWeWe(posInd(0))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoWe(posInd(0))));

	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoEa(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEaEa(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEaEa(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoEa(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoWe(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWeWe(posInd(7))));
	GTEST_ASSERT_TRUE(checkBoard(noWeWe(posInd(7)), 13));
	GTEST_ASSERT_TRUE(checkBoard(noNoWe(posInd(7)), 22));

	GTEST_ASSERT_TRUE(checkBoard(soSoEa(posInd(56)), 41));
	GTEST_ASSERT_TRUE(checkBoard(soEaEa(posInd(56)), 50));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEaEa(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoEa(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoWe(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soWeWe(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWeWe(posInd(56))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoWe(posInd(56))));

	GTEST_ASSERT_TRUE(checkBoardNoPos(soSoEa(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(soEaEa(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noEaEa(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoEa(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoard(soSoWe(posInd(63)), 46));
	GTEST_ASSERT_TRUE(checkBoard(soWeWe(posInd(63)), 53));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noWeWe(posInd(63))));
	GTEST_ASSERT_TRUE(checkBoardNoPos(noNoWe(posInd(63))));

	GTEST_ASSERT_TRUE(checkBoard(soSoEa(posInd(35)), 20));
	GTEST_ASSERT_TRUE(checkBoard(soEaEa(posInd(35)), 29));
	GTEST_ASSERT_TRUE(checkBoard(noEaEa(posInd(35)), 45));
	GTEST_ASSERT_TRUE(checkBoard(noNoEa(posInd(35)), 52));
	GTEST_ASSERT_TRUE(checkBoard(soSoWe(posInd(35)), 18));
	GTEST_ASSERT_TRUE(checkBoard(soWeWe(posInd(35)), 25));
	GTEST_ASSERT_TRUE(checkBoard(noWeWe(posInd(35)), 41));
	GTEST_ASSERT_TRUE(checkBoard(noNoWe(posInd(35)), 50));
}