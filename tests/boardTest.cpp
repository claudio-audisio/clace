#include <gtest/gtest.h>

#include "../board/board.h"
#include "../utils/fen.h"
#include "../utils/boardUtils.h"

using namespace std;


TEST(BoardTest, ConstructorTest) {
	Board board;
	EXPECT_EQ(board.getBoard(0), 0xffffffffffffffff);
	for (RawboardIndex i = 1; i < 13; i++) {
		EXPECT_EQ(board.getBoard(i), 0);
	}
}

TEST(BoardTest, allBoardsTest) {
    Game* game = FEN::fenToNewGame(Positions::INITIAL_FEN_POSITION);
    Board board = game->getBoard();
    EXPECT_EQ(board.BOARD(), board.WHITE() | board.BLACK());
    EXPECT_EQ(board.EMPTY(), ~board.BOARD());
    delete game;
}

TEST(BoardTest, isEmptyTest) {
	Board board;
	for (Position i = 0; i < 63; ++i) {
		GTEST_ASSERT_TRUE(board.isEmpty(i));
	}
	board.setPiece(5, WPawn);
	GTEST_ASSERT_FALSE(board.isEmpty(5));
	board.setPiece(5, Empty);
	GTEST_ASSERT_TRUE(board.isEmpty(5));
}

TEST(BoardTest, isWhiteTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isWhite(i));
    }
    board.setPiece(0, WPawn);
    board.setPiece(7, BPawn);
    board.setPiece(56, BPawn);
    board.setPiece(63, WPawn);

    GTEST_ASSERT_TRUE(board.isWhite(0));
    GTEST_ASSERT_FALSE(board.isWhite(7));
    GTEST_ASSERT_FALSE(board.isWhite(56));
    GTEST_ASSERT_TRUE(board.isWhite(63));
}

TEST(BoardTest, isBlackTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isBlack(i));
    }
    board.setPiece(0, WPawn);
    board.setPiece(7, BPawn);
    board.setPiece(56, BPawn);
    board.setPiece(63, WPawn);

    GTEST_ASSERT_FALSE(board.isBlack(0));
    GTEST_ASSERT_TRUE(board.isBlack(7));
    GTEST_ASSERT_TRUE(board.isBlack(56));
    GTEST_ASSERT_FALSE(board.isBlack(63));
}

TEST(BoardTest, isPawnTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isPawn(i));
    }
    EXPECT_EQ(board.setPiece(0, BPawn), Empty);
    GTEST_ASSERT_TRUE(board.isPawn(0));
    EXPECT_EQ(board.setPiece(0, WPawn), BPawn);
    GTEST_ASSERT_TRUE(board.isPawn(0));
    EXPECT_EQ(board.setPiece(0, WRook), WPawn);
    GTEST_ASSERT_FALSE(board.isPawn(0));
}

TEST(BoardTest, isKnightTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isKnight(i));
    }
    EXPECT_EQ(board.setPiece(7, BKnight), Empty);
    GTEST_ASSERT_TRUE(board.isKnight(7));
    EXPECT_EQ(board.setPiece(7, WKnight), BKnight);
    GTEST_ASSERT_TRUE(board.isKnight(7));
    EXPECT_EQ(board.setPiece(7, WRook), WKnight);
    GTEST_ASSERT_FALSE(board.isKnight(7));
}

TEST(BoardTest, isBishopTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isBishop(i));
    }
    EXPECT_EQ(board.setPiece(0, BBishop), Empty);
    GTEST_ASSERT_TRUE(board.isBishop(0));
    EXPECT_EQ(board.setPiece(0, WBishop), BBishop);
    GTEST_ASSERT_TRUE(board.isBishop(0));
    EXPECT_EQ(board.setPiece(0, WRook), WBishop);
    GTEST_ASSERT_FALSE(board.isBishop(0));
}

TEST(BoardTest, isRookTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isRook(i));
    }
    EXPECT_EQ(board.setPiece(0, BRook), Empty);
    GTEST_ASSERT_TRUE(board.isRook(0));
    EXPECT_EQ(board.setPiece(0, WRook), BRook);
    GTEST_ASSERT_TRUE(board.isRook(0));
    EXPECT_EQ(board.setPiece(0, WPawn), WRook);
    GTEST_ASSERT_FALSE(board.isRook(0));
}

TEST(BoardTest, isQueenTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isQueen(i));
    }
    EXPECT_EQ(board.setPiece(0, BQueen), Empty);
    GTEST_ASSERT_TRUE(board.isQueen(0));
    EXPECT_EQ(board.setPiece(0, WQueen), BQueen);
    GTEST_ASSERT_TRUE(board.isQueen(0));
    EXPECT_EQ(board.setPiece(0, WRook), WQueen);
    GTEST_ASSERT_FALSE(board.isQueen(0));
}

TEST(BoardTest, isKingTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isKing(i));
    }
    EXPECT_EQ(board.setPiece(0, BKing), Empty);
    GTEST_ASSERT_TRUE(board.isKing(0));
    EXPECT_EQ(board.setPiece(0, WKing), BKing);
    GTEST_ASSERT_TRUE(board.isKing(0));
    EXPECT_EQ(board.setPiece(0, WRook), WKing);
    GTEST_ASSERT_FALSE(board.isKing(0));
}

TEST(BoardTest, getPieceTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        EXPECT_EQ(board.getPiece(i), Empty);
    }
    board.setBoard(Empty, 0x7Ffffffffffffffc);
    board.setBoard(WPawn, 0x1);
    board.setBoard(BPawn, 0x2);
    board.setBoard(WRook, 0x8000000000000000);
    EXPECT_EQ(board.getPiece(0), WPawn);
    EXPECT_EQ(board.getPiece(1), BPawn);
    EXPECT_EQ(board.getPiece(63), WRook);
}

TEST(BoardTest, setPieceTest) {
    Board board;
    Piece oldPiece = board.setPiece(4, BKing);
    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_FALSE(board.isEmpty(4));
    GTEST_ASSERT_FALSE(board.isWhite(4));
    GTEST_ASSERT_TRUE(board.isBlack(4));
    GTEST_ASSERT_TRUE(board.isKing(4));
    oldPiece = board.setPiece(4, WQueen);
    EXPECT_EQ(oldPiece, BKing);
    GTEST_ASSERT_FALSE(board.isEmpty(4));
    GTEST_ASSERT_TRUE(board.isWhite(4));
    GTEST_ASSERT_FALSE(board.isBlack(4));
    GTEST_ASSERT_FALSE(board.isKing(4));
    GTEST_ASSERT_TRUE(board.isQueen(4));
}

TEST(BoardTest, moveTest) {
    Board board;
    board.setPiece(4, BKing);
    Piece oldPiece = board.move(4, 12, BKing);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(board.isEmpty(4));
    EXPECT_EQ(board.getPiece(12), BKing);

    board.setPiece(19, WPawn);
    oldPiece = board.move(19, 12, WPawn);

    EXPECT_EQ(oldPiece, BKing);
    GTEST_ASSERT_TRUE(board.isEmpty(19));
    EXPECT_EQ(board.getPiece(12), WPawn);
}

TEST(BoardTest, setTest) {
    Board board;
    Board newboard;
    newboard.setPiece(63, WPawn);
    board.set(newboard);

    GTEST_ASSERT_FALSE(board.isEmpty(63));
    GTEST_ASSERT_TRUE(board.isWhite(63));
    GTEST_ASSERT_FALSE(board.isBlack(63));
    GTEST_ASSERT_TRUE(board.isPawn(63));
}

TEST(BoardTest, isUnderCheckTest) {
    Board board;
    board.setPiece(18, WQueen);

    BoardUtils::printBoard(board.getWhiteAttacks());

    GTEST_ASSERT_FALSE(board.isUnderCheck(33, true));
    GTEST_ASSERT_FALSE(board.isUnderCheck(33, false));
    GTEST_ASSERT_FALSE(board.isUnderCheck(34, true));
    GTEST_ASSERT_TRUE(board.isUnderCheck(34, false));
}







template <typename... SetOfPosition>
static bool checkBoard(Rawboard board, SetOfPosition... expectedPositions) {
    Rawboard bitPositions = 0;

    for (Position position : {expectedPositions...}) {
        if (position != NO_POS) {
            bitPositions |= BoardUtils::posInd(position);
        }
    }

    return board == bitPositions;
    //EXPECT_EQ(board, bitPositions);
}

static bool checkBoardNoPos(Rawboard board) {
    return checkBoard(board, NO_POS);
}

TEST(BoardTest, masksTest) {
    Board board;
    GTEST_ASSERT_TRUE(checkBoard(board.lineMask(0), 0, 1, 2, 3, 4, 5, 6, 7));
    GTEST_ASSERT_TRUE(checkBoard(board.columnMask(0), 0, 8, 16, 24, 32, 40, 48, 56));
    GTEST_ASSERT_TRUE(checkBoard(board.diagonalMask(0), 0, 9, 18, 27, 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.antiDiagonalMask(0), 0));

    GTEST_ASSERT_TRUE(checkBoard(board.lineMask(7), 0, 1, 2, 3, 4, 5, 6, 7));
    GTEST_ASSERT_TRUE(checkBoard(board.columnMask(7), 7, 15, 23, 31, 39, 47, 55, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.diagonalMask(7), 7));
    GTEST_ASSERT_TRUE(checkBoard(board.antiDiagonalMask(7), 7, 14, 21, 28, 35, 42, 49, 56));

    GTEST_ASSERT_TRUE(checkBoard(board.lineMask(56), 56, 57, 58, 59, 60, 61, 62, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.columnMask(56), 0, 8, 16, 24, 32, 40, 48, 56));
    GTEST_ASSERT_TRUE(checkBoard(board.diagonalMask(56), 56));
    GTEST_ASSERT_TRUE(checkBoard(board.antiDiagonalMask(56), 7, 14, 21, 28, 35, 42, 49, 56));

    GTEST_ASSERT_TRUE(checkBoard(board.lineMask(63), 56, 57, 58, 59, 60, 61, 62, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.columnMask(63), 7, 15, 23, 31, 39, 47, 55, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.diagonalMask(63), 0, 9, 18, 27, 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.antiDiagonalMask(63), 63));
}

TEST(BoardTest, raysTest) {
    Board board;
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.northRay(0)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.noEastRay(0)));
    GTEST_ASSERT_TRUE(checkBoard(board.eastRay(0), 1, 2, 3, 4, 5, 6, 7));
    GTEST_ASSERT_TRUE(checkBoard(board.soEastRay(0), 9, 18, 27, 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.southRay(0), 8, 16, 24, 32, 40, 48, 56));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.soWestRay(0)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.westRay(0)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.noWestRay(0)));

    GTEST_ASSERT_TRUE(checkBoardNoPos(board.northRay(7)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.noEastRay(7)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.eastRay(7)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.soEastRay(7)));
    GTEST_ASSERT_TRUE(checkBoard(board.southRay(7), 15, 23, 31, 39, 47, 55, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.soWestRay(7), 14, 21, 28, 35, 42, 49, 56));
    GTEST_ASSERT_TRUE(checkBoard(board.westRay(7), 0, 1, 2, 3, 4, 5, 6));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.noWestRay(7)));

    GTEST_ASSERT_TRUE(checkBoard(board.northRay(56), 0, 8, 16, 24, 32, 40, 48));
    GTEST_ASSERT_TRUE(checkBoard(board.noEastRay(56), 7, 14, 21, 28, 35, 42, 49));
    GTEST_ASSERT_TRUE(checkBoard(board.eastRay(56), 57, 58, 59, 60, 61, 62, 63));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.soEastRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.southRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.soWestRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.westRay(56)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.noWestRay(56)));

    GTEST_ASSERT_TRUE(checkBoard(board.northRay(63), 7, 15, 23, 31, 39, 47, 55));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.noEastRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.eastRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.soEastRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.southRay(63)));
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.soWestRay(63)));
    GTEST_ASSERT_TRUE(checkBoard(board.westRay(63), 56, 57, 58, 59, 60, 61, 62));
    GTEST_ASSERT_TRUE(checkBoard(board.noWestRay(63), 0, 9, 18, 27, 36, 45, 54));
}

