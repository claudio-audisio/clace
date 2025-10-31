#include <gtest/gtest.h>
#include <list>

#include "testUtils.h"
#include "../board/board.h"
#include "../utils/boardUtils.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;


class BoardUtilsTest : public testing::Test {
protected:
	BoardUtilsTest() {
		initAttacks();
		initDestPosProviders();
	}
	~BoardUtilsTest() {

	}
};

TEST_F(BoardUtilsTest, boardToListTest) {
	Board board;
	reset(board);
	setPiece(board, 2, WPawn);
	setPiece(board, 8, BPawn);
	setPiece(board, 42, WQueen);
	setPiece(board, 58, BKnight);
	setPiece(board, 63, BRook);
	list<Position> positions;
	boardToList(~board.pieceBoards[Empty], positions);
	EXPECT_EQ(positions.size(), 5);
	GTEST_ASSERT_TRUE(containsAll(positions, 2, 8, 42, 58, 63));
}

TEST_F(BoardUtilsTest, boardToSpacesTest) {
	Board board;
	reset(board);
	setPiece(board, 2, WPawn);
	setPiece(board, 8, BPawn);
	setPiece(board, 42, WQueen);
	setPiece(board, 58, BKnight);
	setPiece(board, 63, BRook);
	list<Position> spaces;
	boardToSpaces(~board.pieceBoards[Empty], spaces);
	EXPECT_EQ(spaces.size(), 6);
	GTEST_ASSERT_TRUE(containsAll(spaces, 5, 2, 5, 33, 15, 4));
}

TEST_F(BoardUtilsTest, positionsCountTest) {
	Board board;
	reset(board);
	EXPECT_EQ(positionsCount(~board.pieceBoards[Empty]), 0);
	setPiece(board, 2, WPawn);
	setPiece(board, 8, BPawn);
	setPiece(board, 42, WQueen);
	setPiece(board, 58, BKnight);
	setPiece(board, 63, BRook);
	EXPECT_EQ(positionsCount(~board.pieceBoards[Empty]), 5);
}
