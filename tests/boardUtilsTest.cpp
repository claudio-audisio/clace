#include <gtest/gtest.h>
#include <list>

#include "../board/board.h"
#include "../utils/boardUtils.h"

using namespace std;

template <typename... SetOfPosition>
bool containsAll(list<Position> listOfPositions, SetOfPosition... positions) {
	for (Position positionToCheck : {positions...}) {
		bool found = false;
		for (Position position : listOfPositions) {
			if (position == positionToCheck) {
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}
	}
	return true;
}

TEST(BoardUtilsTest, boardToListTest) {
	Board board;
	board.setPiece(2, WPawn);
	board.setPiece(8, BPawn);
	board.setPiece(42, WQueen);
	board.setPiece(58, BKnight);
	board.setPiece(63, BRook);
	list<Position> positions;
	BoardUtils::boardToList(~board.EMPTY, positions);
	EXPECT_EQ(positions.size(), 5);
	GTEST_ASSERT_TRUE(containsAll(positions, 2, 8, 42, 58, 63));
}

TEST(BoardUtilsTest, positionsCountTest) {
	Board board;
	EXPECT_EQ(BoardUtils::positionsCount(~board.EMPTY), 0);
	board.setPiece(2, WPawn);
	board.setPiece(8, BPawn);
	board.setPiece(42, WQueen);
	board.setPiece(58, BKnight);
	board.setPiece(63, BRook);
	EXPECT_EQ(BoardUtils::positionsCount(~board.EMPTY), 5);
}
