#include <gtest/gtest.h>
#include <list>

#include "testUtils.h"
#include "../board/board.h"
#include "../utils/boardUtils.h"

using namespace std;


class BoardUtilsTest : public testing::Test {
protected:
	BoardUtilsTest() {
		initAttacks();
	}
	~BoardUtilsTest() {

	}
};

TEST_F(BoardUtilsTest, boardToListTest) {
	Board board;
	board.setPiece(2, WPawn);
	board.setPiece(8, BPawn);
	board.setPiece(42, WQueen);
	board.setPiece(58, BKnight);
	board.setPiece(63, BRook);
	list<Position> positions;
	boardToList(~board.EMPTY, positions);
	EXPECT_EQ(positions.size(), 5);
	GTEST_ASSERT_TRUE(containsAll(positions, 2, 8, 42, 58, 63));
}

TEST_F(BoardUtilsTest, boardToSpacesTest) {
	Board board;
	board.setPiece(2, WPawn);
	board.setPiece(8, BPawn);
	board.setPiece(42, WQueen);
	board.setPiece(58, BKnight);
	board.setPiece(63, BRook);
	list<Position> spaces;
	boardToSpaces(~board.EMPTY, spaces);
	EXPECT_EQ(spaces.size(), 6);
	GTEST_ASSERT_TRUE(containsAll(spaces, 5, 2, 5, 33, 15, 4));
}

TEST_F(BoardUtilsTest, positionsCountTest) {
	Board board;
	EXPECT_EQ(positionsCount(~board.EMPTY), 0);
	board.setPiece(2, WPawn);
	board.setPiece(8, BPawn);
	board.setPiece(42, WQueen);
	board.setPiece(58, BKnight);
	board.setPiece(63, BRook);
	EXPECT_EQ(positionsCount(~board.EMPTY), 5);
}
