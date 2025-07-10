#include <gtest/gtest.h>

#include "../board/board.h"
#include "../utils/fen.h"
#include "testUtils.h"

using namespace std;


class BoardTest : public testing::Test {
protected:
	BoardTest() {
		initAttacks();
	}
	~BoardTest() {

	}
};

TEST_F(BoardTest, ConstructorTest) {
	Board board;
	EXPECT_EQ(board.pieceBoards[0], 0xffffffffffffffff);
	for (RawboardIndex i = 1; i < SIZE; i++) {
		EXPECT_EQ(board.pieceBoards[i], Empty);
	}
	EXPECT_EQ(board.castlingInfo, 0);
	EXPECT_EQ(board.enPassantPosition, NO_POS);
}

TEST_F(BoardTest, allBoardsTest) {
    Game* game = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    Board& board = game->board;
    EXPECT_EQ(~board.EMPTY, board.PIECES(WHITE) | board.PIECES(BLACK));
	EXPECT_EQ(~board.PIECES(WHITE), board.EMPTY | board.OPP_PIECES(WHITE));
	EXPECT_EQ(~board.PIECES(BLACK), board.EMPTY | board.OPP_PIECES(BLACK));
    delete game;
}

TEST_F(BoardTest, isEmptyTest) {
	Board board;
	for (Position i = 0; i < 64; ++i) {
		GTEST_ASSERT_TRUE(board.isEmpty(i));
	}
	board.setPiece(5, WPawn);
	GTEST_ASSERT_FALSE(board.isEmpty(5));
	board.setEmpty(5);
	GTEST_ASSERT_TRUE(board.isEmpty(5));
}

TEST_F(BoardTest, isWhiteTest) {
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

TEST_F(BoardTest, isBlackTest) {
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

TEST_F(BoardTest, isPawnTest) {
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

TEST_F(BoardTest, isKnightTest) {
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

TEST_F(BoardTest, isBishopTest) {
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

TEST_F(BoardTest, isRookTest) {
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

TEST_F(BoardTest, isQueenTest) {
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

TEST_F(BoardTest, isKingTest) {
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

TEST_F(BoardTest, getPieceTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        EXPECT_EQ(board.getPiece(i), Empty);
    }
	board.setPiece(0, WPawn);
	board.setPiece(1, BPawn);
	board.setPiece(63, WRook);
    EXPECT_EQ(board.getPiece(0), WPawn);
    EXPECT_EQ(board.getPiece(1), BPawn);
    EXPECT_EQ(board.getPiece(63), WRook);
}

TEST_F(BoardTest, setPieceTest) {
    Board board;
    Piece oldPiece = board.setPiece(4, BKing);
    EXPECT_EQ(oldPiece, Empty);
	EXPECT_EQ(board.piecePositions[4], BKing);
	EXPECT_EQ(board.pieceBoards[BKing], 0x10);
	EXPECT_EQ(board.EMPTY, 0xFFFFFFFFFFFFFFEF);
    oldPiece = board.setPiece(4, WQueen);
    EXPECT_EQ(oldPiece, BKing);
	EXPECT_EQ(board.piecePositions[4], WQueen);
	EXPECT_EQ(board.pieceBoards[WQueen], 0x10);
	EXPECT_EQ(board.pieceBoards[BKing], 0);
	EXPECT_EQ(board.EMPTY, 0xFFFFFFFFFFFFFFEF);
}

TEST_F(BoardTest, setEmptyTest) {
	Board board;
	Piece oldPiece = board.setPiece(4, BKing);
	EXPECT_EQ(oldPiece, Empty);
	oldPiece = board.setEmpty(4);
	EXPECT_EQ(oldPiece, BKing);
	EXPECT_EQ(board.piecePositions[4], Empty);
	EXPECT_EQ(board.pieceBoards[BKing], 0);
	EXPECT_EQ(board.EMPTY, 0xFFFFFFFFFFFFFFFF);
}

// TODO aggiungere i test mancanti

TEST_F(BoardTest, moveTest) {
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

TEST_F(BoardTest, setTest) {
    Board board;
    Board newboard;
    newboard.setPiece(63, WPawn);
    board.set(newboard);

    GTEST_ASSERT_FALSE(board.isEmpty(63));
    GTEST_ASSERT_TRUE(board.isWhite(63));
    GTEST_ASSERT_FALSE(board.isBlack(63));
    GTEST_ASSERT_TRUE(board.isPawn(63));
}

/*TEST_F(BoardTest, isUnderCheckTest) {
    Board board;
    board.setPiece(18, WQueen);

    GTEST_ASSERT_FALSE(board.isUnderCheck(33, WHITE));
    GTEST_ASSERT_FALSE(board.isUnderCheck(33, BLACK));
    GTEST_ASSERT_FALSE(board.isUnderCheck(34, WHITE));
    GTEST_ASSERT_TRUE(board.isUnderCheck(34, BLACK));
}*/

class TestParams {
public:
    TestParams(string fenGame, Side side, list<Position>* expectedPositions) {
        this->fenGame = fenGame;
        this->side = side;
        this->expectedPositions = expectedPositions;
    }

    string fenGame;
    Side side;
    list<Position>* expectedPositions;
};


class GetKingAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
    GetKingAttacksTest() {
        initAttacks();
    }
};

TEST_P(GetKingAttacksTest, getKingAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->board.getKingAttacks(params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    BoardTest,
    GetKingAttacksTest,
    ::testing::Values(
        new TestParams("8/8/8/3K4/8/8/8/8 w - - 0 1", WHITE, new list<Position>{ 18, 19, 20, 26, 28, 34, 35, 36 }),
        new TestParams("8/8/8/8/8/5k2/8/8 w - - 0 1", BLACK, new list<Position>{ 36, 37, 38, 44, 46, 52, 53, 54 }),
        new TestParams("K7/8/8/8/8/8/8/8 w - - 0 1", WHITE, new list<Position>{ 1, 8, 9 }),
        new TestParams("8/8/8/8/8/8/6k1/8 w - - 0 1", BLACK, new list<Position>{ 45, 46, 47, 53, 55, 61, 62, 63 }),
        new TestParams("8/8/1p6/K7/1P6/8/8/8 w - - 0 1", WHITE, new list<Position>{ 16, 17, 25, 32 }),
        new TestParams("8/8/8/7P/6pk/8/8/8 w - - 0 1", BLACK, new list<Position>{ 30, 31, 46, 47 })
    )
);


class GetKingMovesTest : public ::testing::TestWithParam<TestParams*> {
protected:
    GetKingMovesTest() {
        initAttacks();
    }
};

TEST_P(GetKingMovesTest, getKingMovesTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->board.getKingMoves(params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    BoardTest,
    GetKingMovesTest,
    ::testing::Values(
        new TestParams("8/8/8/3K4/8/8/8/8 w - - 0 1", WHITE, new list<Position>{18, 19, 20, 26, 28, 34, 35, 36}),
        new TestParams("8/8/8/8/8/5k2/8/8 w - - 0 1", BLACK, new list<Position>{36, 37, 38, 44, 46, 52, 53, 54}),
        new TestParams("K7/8/8/8/8/8/8/8 w - - 0 1", WHITE, new list<Position>{1, 8, 9}),
        new TestParams("8/8/8/8/8/8/6k1/8 w - - 0 1", BLACK, new list<Position>{45, 46, 47, 53, 55, 61, 62, 63}),
        new TestParams("8/8/1p6/K7/1P6/8/8/8 w - - 0 1", WHITE, new list<Position>{16, 17, 25, 32}),
        new TestParams("8/8/8/7P/6pk/8/8/8 w - - 0 1", BLACK, new list<Position>{30, 31, 46, 47}),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQkq - 0 1", BLACK, new list<Position>{2, 3, 11, 12, 13, 5, 6}),
        new TestParams("8/8/8/8/8/8/8/4K3 w KQkq - 0 1", WHITE, new list<Position>{58, 59, 51, 52, 53, 61, 62}),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQk - 0 1", BLACK, new list<Position>{3, 11, 12, 13, 5, 6}),
        new TestParams("8/8/8/8/8/8/8/4K3 w Kkq - 0 1", WHITE, new list<Position>{59, 51, 52, 53, 61, 62}),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQq - 0 1", BLACK, new list<Position>{2, 3, 11, 12, 13, 5}),
        new TestParams("8/8/8/8/8/8/8/4K3 w Qkq - 0 1", WHITE, new list<Position>{58, 59, 51, 52, 53, 61}),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQ - 0 1", BLACK, new list<Position>{3, 11, 12, 13, 5}),
        new TestParams("8/8/8/8/8/8/8/4K3 w kq - 0 1", WHITE, new list<Position>{ 59, 51, 52, 53, 61 })
    )
);


class GetQueenAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetQueenAttacksTest() {
		initAttacks();
	}
};

TEST_P(GetQueenAttacksTest, getQueenAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
	GTEST_ASSERT_TRUE(checkBoard(game->board.getQueenAttacks(params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    BoardTest,
    GetQueenAttacksTest,
    ::testing::Values(
        new TestParams("8/8/8/3Q4/8/8/8/8 w - - 0 1", WHITE, new list<Position>{0, 9, 18, 36, 45, 54, 63, 6, 13, 20, 34, 41, 48, 19, 11, 3, 35, 43, 51, 59, 24, 25, 26, 28, 29, 30, 31}),
        new TestParams("8/8/8/8/8/5q2/8/8 w - - 0 1", BLACK, new list<Position>{ 0, 9, 18, 27, 36, 54, 63, 38, 31, 52, 59, 37, 29, 21, 13, 5, 53, 61, 40, 41, 42, 43, 44, 46, 47 }),
        new TestParams("Q7/8/8/8/8/8/8/8 w - - 0 1", WHITE, new list<Position>{ 9, 18, 27, 36, 45, 54, 63, 8, 16, 24, 32, 40, 48, 56, 1, 2, 3, 4, 5, 6, 7 }),
        new TestParams("8/8/8/8/8/8/6q1/8 w - - 0 1", BLACK, new list<Position>{ 0, 9, 18, 27, 36, 45, 63, 61, 47, 46, 38, 30, 22, 14, 6, 62, 48, 49, 50, 51, 52, 53, 55 }),
        new TestParams("8/2p5/8/Q7/8/2P5/P7/8 w - - 0 1", WHITE, new list<Position>{ 33, 17, 10, 25, 26, 27, 28, 29, 30, 31, 32, 40, 0, 8, 16 }),
        new TestParams("8/8/5P1P/8/7q/8/5p2/8 w - - 0 1", BLACK, new list<Position>{ 46, 30, 21, 32, 33, 34, 35, 36, 37, 38, 63, 55, 47, 31, 23 })
    )
);


class GetRookAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetRookAttacksTest() {
		initAttacks();
	}
};

TEST_P(GetRookAttacksTest, getRookAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->board.getRookAttacks(params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    BoardTest,
    GetRookAttacksTest,
    ::testing::Values(
        new TestParams("8/8/1R6/8/8/8/8/8 w - - 0 1", WHITE, new list<Position>{ 16, 18, 19, 20, 21, 22, 23, 1, 9, 25, 33, 41, 49, 57 }),
        new TestParams("8/8/8/8/8/8/6r1/8 w - - 0 1", BLACK, new list<Position>{ 48, 49, 50, 51, 52, 53, 55, 62, 46, 38, 30, 22, 14, 6 }),
        new TestParams("RP6/P7/8/8/8/8/8/8 w - - 0 1", WHITE, new list<Position>()),
        new TestParams("8/8/8/8/8/8/7p/6pr w - - 0 1", BLACK, new list<Position>()),
        new TestParams("8/8/1R1P4/8/1p6/8/8/8 w - - 0 1", WHITE, new list<Position>{ 16, 18, 1, 9, 25, 33 }),
        new TestParams("8/8/8/8/6p1/8/4P1r1/8 w - - 0 1", BLACK, new list<Position>{ 52, 53, 55, 62, 46 })
    )
);


class GetBishopAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetBishopAttacksTest() {
		initAttacks();
	}
};

TEST_P(GetBishopAttacksTest, getBishopAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->board.getBishopAttacks(params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    BoardTest,
    GetBishopAttacksTest,
    ::testing::Values(
        new TestParams("8/8/8/3B4/8/8/8/8 w - - 0 1", WHITE, new list<Position>{0, 9, 18, 36, 45, 54, 63, 6, 13, 20, 34, 41, 48}),
        new TestParams("8/8/8/8/8/5b2/8/8 w - - 0 1", BLACK, new list<Position>{0, 9, 18, 27, 36, 54, 63, 38, 31, 52, 59}),
        new TestParams("B7/8/8/8/8/8/8/8 w - - 0 1", WHITE, new list<Position>{9, 18, 27, 36, 45, 54, 63}),
        new TestParams("8/8/8/8/8/8/6b1/8 w - - 0 1", BLACK, new list<Position>{0, 9, 18, 27, 36, 45, 63, 61, 47}),
        new TestParams("8/2p5/8/B7/8/2P5/8/8 w - - 0 1", WHITE, new list<Position>{33, 17, 10}),
        new TestParams("8/8/5P2/8/7b/8/5p2/8 w - - 0 1", BLACK, new list<Position>{46, 30, 21})
    )
);


class GetKnightAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetKnightAttacksTest() {
		initAttacks();
	}
};

TEST_P(GetKnightAttacksTest, getKnightAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->board.getKnightAttacks(params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    BoardTest,
    GetKnightAttacksTest,
    ::testing::Values(
        new TestParams("8/8/2N5/8/8/8/8/8 w - - 0 1", WHITE, new list<Position>{ 1, 8, 24, 33, 35, 28, 3, 12 }),
        new TestParams("8/8/8/8/3n4/8/8/8 w - - 0 1", BLACK, new list<Position>{ 20, 18, 25, 29, 41, 45, 50, 52 }),
        new TestParams("N7/8/8/8/8/8/8/8 w - - 0 1", WHITE, new list<Position>{ 10, 17 }),
        new TestParams("8/8/8/8/8/8/8/n7 w - - 0 1", BLACK, new list<Position>{ 41, 50 }),
        new TestParams("1P6/P7/2N5/8/1p1p4/8/8/8 w - - 0 1", WHITE, new list<Position>{ 24, 33, 35, 28, 3, 12 }),
        new TestParams("8/8/8/8/3n4/1P3P2/2p1p3/8 w - - 0 1", BLACK, new list<Position>{ 20, 18, 25, 29, 41, 45 })
    )
);


class GetPawnMovesTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetPawnMovesTest() {
		initAttacks();
	}
};

TEST_P(GetPawnMovesTest, getPawnMovesTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->board.getPawnMoves(params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    BoardTest,
    GetPawnMovesTest,
    ::testing::Values(
        new TestParams("8/p7/8/8/8/8/8/8 w - - 0 1", BLACK, new list<Position>{ 16, 24 }),
        new TestParams("8/8/p7/8/8/8/8/8 w - - 0 1", BLACK, new list<Position>{ 24 }),
        new TestParams("8/8/8/8/8/8/P7/8 w - - 0 1", WHITE, new list<Position>{ 40, 32 }),
        new TestParams("8/8/8/8/8/P7/8/8 w - - 0 1", WHITE, new list<Position>{ 32 }),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", BLACK, new list<Position>()),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", WHITE, new list<Position>()),
        new TestParams("8/8/8/3p4/2P1b3/8/8/8 w - - 0 1", BLACK, new list<Position>{ 34, 35 }),
        new TestParams("8/8/8/2B1p3/3P4/8/8/8 w - - 0 1", WHITE, new list<Position>{ 27, 28 }),
        new TestParams("8/8/8/pP6/8/8/8/8 w - a6 0 1", WHITE, new list<Position>{ 16, 17 }),
        new TestParams("8/8/8/pP5P/8/8/8/8 w - a6 0 1", WHITE, new list<Position>{ 16, 17, 23 }),
        new TestParams("8/8/8/8/6Pp/8/8/8 w - g3 0 1", BLACK, new list<Position>{ 46, 47 }),
        new TestParams("8/8/8/8/p5Pp/8/8/8 w - g3 0 1", BLACK, new list<Position>{ 40, 46, 47 })
    )
);


class TestParams2 {
public:
    TestParams2(string fenGame, Side side, Position position, list<Position>* expectedPositions) {
        this->fenGame = fenGame;
        this->side = side;
        this->position = position;
        this->expectedPositions = expectedPositions;
    }

    
    string fenGame;
    Side side;
    Position position;
    list<Position>* expectedPositions;
};


class GetSinglePawnMovesTest : public ::testing::TestWithParam<TestParams2*> {
protected:
	GetSinglePawnMovesTest() {
		initAttacks();
	}
};

TEST_P(GetSinglePawnMovesTest, getSinglePawnMovesTest) {
    TestParams2* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->board.getPawnMoves(params->position, params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    BoardTest,
    GetSinglePawnMovesTest,
    ::testing::Values(
        new TestParams2("8/6bb/8/8/R1pP2k1/4P3/P7/K7 w - d3 0 1", BLACK, 34, new list<Position>{ 42, 43 }),						// Following from https://www.chessprogramming.org/En_passant
        new TestParams2("2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K w - b3 0 1", BLACK, 32, new list<Position>{ 41 }),
        new TestParams2("2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K w - b3 0 1", BLACK, 34, new list<Position>{ 41 }),
        new TestParams2("rnbqkbnr/ppppppp1/8/7p/P7/8/1PPPPPPP/RNBQKBNR w - h6 0 1", WHITE, 32, new list<Position>{ 24 })
    )
);


class GetPawnAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetPawnAttacksTest() {
		initAttacks();
	}
};

TEST_P(GetPawnAttacksTest, getPawnAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->board.getPawnAttacks(params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    BoardTest,
    GetPawnAttacksTest,
    ::testing::Values(
        new TestParams("8/p7/8/8/8/8/8/8 w - - 0 1", BLACK, new list<Position>{ 17 }),
        new TestParams("8/8/p7/8/8/8/8/8 w - - 0 1", BLACK, new list<Position>{ 25 }),
        new TestParams("8/8/8/8/8/8/P7/8 w - - 0 1", WHITE, new list<Position>{ 41 }),
        new TestParams("8/8/8/8/8/P7/8/8 w - - 0 1", WHITE, new list<Position>{ 33 }),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", BLACK, new list<Position>{ 34, 36 }),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", WHITE, new list<Position>{ 26, 28 }),
        new TestParams("8/8/8/3p4/2P1b3/8/8/8 w - - 0 1", BLACK, new list<Position>{ 34 }),
        new TestParams("8/8/8/2B1p3/3P4/8/8/8 w - - 0 1", WHITE, new list<Position>{ 28 })
    )
);


TEST_F(BoardTest, getPawnMovesTest) {
    Board board;
    board.setPiece(49, WPawn);
    board.setPiece(40, BPawn);
    board.setPiece(42, BPawn);
    GTEST_ASSERT_TRUE(checkBoard(board.getPawnMoves(49, WHITE), 40, 41, 42, 33));
    EXPECT_EQ(board.getPawnMoves(49, WHITE), 0x70200000000LL);
    board.setPiece(33, BPawn);   // disable double push
    EXPECT_EQ(board.getPawnMoves(49, WHITE), 0x70000000000LL);
    board.setPiece(41, BPawn);
    EXPECT_EQ(board.getPawnMoves(49, WHITE), 0x50000000000LL);
    board.setPiece(40, WPawn);
    EXPECT_EQ(board.getPawnMoves(49, WHITE), 0x40000000000LL);

    board.setPiece(9, BPawn);
    board.setPiece(16, WPawn);
    board.setPiece(18, WPawn);
    EXPECT_EQ(board.getPawnMoves(9, BLACK), 0x2070000LL);
    board.setPiece(25, WPawn);   // disable double push
    EXPECT_EQ(board.getPawnMoves(9, BLACK), 0x70000LL);
    board.setPiece(17, WPawn);
    EXPECT_EQ(board.getPawnMoves(9, BLACK), 0x50000LL);
    board.setPiece(16, BPawn);
    EXPECT_EQ(board.getPawnMoves(9, BLACK), 0x40000LL);

    board.setPiece(0, WPawn);
    EXPECT_EQ(board.getPawnMoves(0, WHITE), 0x0LL);
    board.setPiece(63, BPawn);
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.getPawnMoves(63, BLACK)));
    EXPECT_EQ(board.getPawnMoves(63, BLACK), 0x0LL);

    board.reset();

    // EnPassant
	board.enPassantPosition = 18;
    EXPECT_EQ(board.getPawnMoves(24, WHITE), 0x10000LL);
	board.enPassantPosition = 17;
    EXPECT_EQ(board.getPawnMoves(24, WHITE), 0x30000LL);
	board.enPassantPosition = 18;
    EXPECT_EQ(board.getPawnMoves(25, WHITE), 0x60000LL);
	board.enPassantPosition = 16;
    EXPECT_EQ(board.getPawnMoves(25, WHITE), 0x30000LL);
	board.enPassantPosition = 42;
    EXPECT_EQ(board.getPawnMoves(32, BLACK), 0x10000000000LL);
	board.enPassantPosition = 41;
    EXPECT_EQ(board.getPawnMoves(32, BLACK), 0x30000000000LL);
	board.enPassantPosition = 42;
    EXPECT_EQ(board.getPawnMoves(33, BLACK), 0x60000000000LL);
	board.enPassantPosition = 40;
    EXPECT_EQ(board.getPawnMoves(33, BLACK), 0x30000000000LL);
}

/*TEST_F(BoardTest, getKnightPositionsTest) {
    Board board;
    EXPECT_EQ(board.getKnightMoves(0, BLACK), 0x20400LL);
    EXPECT_EQ(board.getKnightMoves(7, BLACK), 0x402000LL);
    EXPECT_EQ(board.getKnightMoves(63, BLACK), 0x20400000000000LL);
    EXPECT_EQ(board.getKnightMoves(56, BLACK), 0x4020000000000LL);
    EXPECT_EQ(board.getKnightMoves(45, BLACK), 0x5088008850000000LL);
    board.setPiece(55, BPawn);
    EXPECT_EQ(board.getKnightMoves(45, WHITE), 0x5088008850000000LL);
    EXPECT_EQ(board.getKnightMoves(45, BLACK), 0x5008008850000000LL);
}*/

/*TEST_F(BoardTest, getBishopPositionsTest) {
    Board board;
    EXPECT_EQ(board.getBishopMoves(0, BLACK), 0x8040201008040200LL);
    EXPECT_EQ(board.getBishopMoves(7, BLACK), 0x102040810204000LL);
    EXPECT_EQ(board.getBishopMoves(63, BLACK), 0x40201008040201LL);
    EXPECT_EQ(board.getBishopMoves(56, BLACK), 0x2040810204080LL);
    EXPECT_EQ(board.getBishopMoves(54, BLACK), 0xa000a01008040201LL);
    board.setPiece(27, BPawn);
    EXPECT_EQ(board.getBishopMoves(54, WHITE), 0xa000a01008000000LL);
    EXPECT_EQ(board.getBishopMoves(54, BLACK), 0xa000a01000000000LL);

}

TEST_F(BoardTest, getRookPositionsTest) {
    Board board;
    EXPECT_EQ(board.getRookMoves(0, BLACK), 0x1010101010101feLL);
    EXPECT_EQ(board.getRookMoves(7, BLACK), 0x808080808080807fLL);
    EXPECT_EQ(board.getRookMoves(63, BLACK), 0x7f80808080808080LL);
    EXPECT_EQ(board.getRookMoves(56, BLACK), 0xfe01010101010101LL);
    EXPECT_EQ(board.getRookMoves(54, BLACK), 0x40bf404040404040LL);
    board.setPiece(30, BPawn);
    EXPECT_EQ(board.getRookMoves(54, WHITE), 0x40bf404040000000LL);
    EXPECT_EQ(board.getRookMoves(54, BLACK), 0x40bf404000000000LL);

}

TEST_F(BoardTest, getQueenPositionsTest) {
    Board board;
    EXPECT_EQ(board.getQueenMoves(0, BLACK), 0x81412111090503feLL);
    EXPECT_EQ(board.getQueenMoves(7, BLACK), 0x8182848890a0c07fLL);
    EXPECT_EQ(board.getQueenMoves(63, BLACK), 0x7fc0a09088848281LL);
    EXPECT_EQ(board.getQueenMoves(56, BLACK), 0xfe03050911214181LL);
    EXPECT_EQ(board.getQueenMoves(54, BLACK), 0xe0bfe05048444241LL);
    board.setPiece(51, BPawn);
    EXPECT_EQ(board.getQueenMoves(54, WHITE), 0xe0b8e05048444241LL);
    EXPECT_EQ(board.getQueenMoves(54, BLACK), 0xe0b0e05048444241LL);
}*/

TEST_F(BoardTest, getKingPositionTest) {
    Board board;
	board.castlingInfo = 0b1111;
    EXPECT_EQ(board.getKingMoves(0, BLACK), 0x302LL);
    EXPECT_EQ(board.getKingMoves(7, BLACK), 0xc040LL);
    EXPECT_EQ(board.getKingMoves(63, BLACK), 0x40c0000000000000LL);
    EXPECT_EQ(board.getKingMoves(56, BLACK), 0x203000000000000LL);
    EXPECT_EQ(board.getKingMoves(54, BLACK), 0xe0a0e00000000000LL);
    board.setPiece(62, BPawn);
    EXPECT_EQ(board.getKingMoves(54, BLACK), 0xa0a0e00000000000LL);
    EXPECT_EQ(board.getKingMoves(54, WHITE), 0xe0a0e00000000000LL);

    // castling
	board.castlingInfo = 0b1111;
    EXPECT_EQ(board.getKingMoves(4, BLACK), 0x386cLL);
	board.castlingInfo = 0b1110;
    EXPECT_EQ(board.getKingMoves(4, BLACK), 0x3868LL);
	board.castlingInfo = 0b1100;
    EXPECT_EQ(board.getKingMoves(4, BLACK), 0x3828LL);
	board.castlingInfo = 0b1111;
    EXPECT_EQ(board.getKingMoves(60, WHITE), 0x2c38000000000000LL);
	board.castlingInfo = 0b1011;
    EXPECT_EQ(board.getKingMoves(60, WHITE), 0x2838000000000000LL);
	board.castlingInfo = 0b0011;
    EXPECT_EQ(board.getKingMoves(60, WHITE), 0x2838000000000000LL);
}

/*TEST_F(BoardTest, slidingAttackTest) {
    Board board;
    Rawboard posInd = Board::posInd(36);
    EXPECT_EQ(board.slidingAttack(Board::soEastOne, posInd, board.BOARD(WHITE)), 0x8040200000000000LL);
    board.setPiece(54, WPawn);
    EXPECT_EQ(board.slidingAttack(Board::soEastOne, posInd, board.BOARD(WHITE)), 0x40200000000000LL);
    board.setPiece(54, BPawn);
    EXPECT_EQ(board.slidingAttack(Board::soEastOne, posInd, board.BOARD(WHITE)), 0x200000000000LL);
    board.setPiece(29, WPawn);
    GTEST_ASSERT_TRUE(checkBoard(board.slidingAttack(Board::noEastOne, posInd, board.PIECES(WHITE)), 29));
}*/

TEST_F(BoardTest, attackTest1) {
    Game* game = FEN::fenToNewGame("4k3/8/8/8/3Q4/8/8/4K3 w - - 0 1");
    Rawboard occupied = ~game->board.EMPTY;

    GTEST_ASSERT_TRUE(checkBoard(game->board.northAttack(occupied, 35), 3, 11, 19, 27));
    GTEST_ASSERT_TRUE(checkBoard(game->board.noEastAttack(occupied, 35), 7, 14, 21, 28));
    GTEST_ASSERT_TRUE(checkBoard(game->board.eastAttack(occupied, 35), 36, 37, 38, 39));
    GTEST_ASSERT_TRUE(checkBoard(game->board.soEastAttack(occupied, 35), 44, 53, 62));
    GTEST_ASSERT_TRUE(checkBoard(game->board.southAttack(occupied, 35), 43, 51, 59));
    GTEST_ASSERT_TRUE(checkBoard(game->board.soWestAttack(occupied, 35), 42, 49, 56));
    GTEST_ASSERT_TRUE(checkBoard(game->board.westAttack(occupied, 35), 34, 33, 32));
    GTEST_ASSERT_TRUE(checkBoard(game->board.noWestAttack(occupied, 35), 8, 17, 26));

	delete game;
}

TEST_F(BoardTest, attackTest2) {
    Game* game = FEN::fenToNewGame("4k3/8/1P1p1p2/8/1p1Q1P2/8/1P1P1p2/4K3 w - - 0 1");
    Rawboard occupied = ~game->board.EMPTY;

    GTEST_ASSERT_TRUE(checkBoard(game->board.northAttack(occupied, 35), 19, 27));
    GTEST_ASSERT_TRUE(checkBoard(game->board.noEastAttack(occupied, 35), 21, 28));
    GTEST_ASSERT_TRUE(checkBoard(game->board.eastAttack(occupied, 35), 36, 37));
    GTEST_ASSERT_TRUE(checkBoard(game->board.soEastAttack(occupied, 35), 44, 53));
    GTEST_ASSERT_TRUE(checkBoard(game->board.southAttack(occupied, 35), 43, 51));
    GTEST_ASSERT_TRUE(checkBoard(game->board.soWestAttack(occupied, 35), 42, 49));
    GTEST_ASSERT_TRUE(checkBoard(game->board.westAttack(occupied, 35), 34, 33));
    GTEST_ASSERT_TRUE(checkBoard(game->board.noWestAttack(occupied, 35), 17, 26));

	delete game;
}

TEST_F(BoardTest,  attackTest3) {
    Game* game = FEN::fenToNewGame("4k3/p2P2P1/1P1p1p2/8/Pp1Q1Pp1/8/1P1P1p2/p2pK1P1 w - - 0 1");
    Rawboard occupied = ~game->board.EMPTY;

    GTEST_ASSERT_TRUE(checkBoard(game->board.northAttack(occupied, 35), 19, 27));
    GTEST_ASSERT_TRUE(checkBoard(game->board.noEastAttack(occupied, 35), 21, 28));
    GTEST_ASSERT_TRUE(checkBoard(game->board.eastAttack(occupied, 35), 36, 37));
    GTEST_ASSERT_TRUE(checkBoard(game->board.soEastAttack(occupied, 35), 44, 53));
    GTEST_ASSERT_TRUE(checkBoard(game->board.southAttack(occupied, 35), 43, 51));
    GTEST_ASSERT_TRUE(checkBoard(game->board.soWestAttack(occupied, 35), 42, 49));
    GTEST_ASSERT_TRUE(checkBoard(game->board.westAttack(occupied, 35), 34, 33));
    GTEST_ASSERT_TRUE(checkBoard(game->board.noWestAttack(occupied, 35), 17, 26));

	delete game;
}

TEST_F(BoardTest, attackTest4) {
    Game* game = FEN::fenToNewGame("4k3/p2P2P1/8/2Ppp3/P1pQP1p1/2PPp3/8/p2pK1P1 w - - 0 1");
    Rawboard occupied = ~game->board.EMPTY;

    GTEST_ASSERT_TRUE(checkBoard(game->board.northAttack(occupied, 35), 27));
    GTEST_ASSERT_TRUE(checkBoard(game->board.noEastAttack(occupied, 35), 28));
    GTEST_ASSERT_TRUE(checkBoard(game->board.eastAttack(occupied, 35), 36));
    GTEST_ASSERT_TRUE(checkBoard(game->board.soEastAttack(occupied, 35), 44));
    GTEST_ASSERT_TRUE(checkBoard(game->board.southAttack(occupied, 35), 43));
    GTEST_ASSERT_TRUE(checkBoard(game->board.soWestAttack(occupied, 35), 42));
    GTEST_ASSERT_TRUE(checkBoard(game->board.westAttack(occupied, 35), 34));
    GTEST_ASSERT_TRUE(checkBoard(game->board.noWestAttack(occupied, 35), 26));

	delete game;
}
