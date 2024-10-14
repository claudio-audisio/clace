#include <gtest/gtest.h>

#include "../board/board.h"
#include "../utils/fen.h"
#include "../utils/boardUtils.h"

using namespace std;


class BoardTest : public testing::Test {
protected:
	BoardTest() {
		BoardUtils::initRayAttacks();
	}
	~BoardTest() {

	}
};

TEST_F(BoardTest, ConstructorTest) {
	Board board;
	EXPECT_EQ(board .pieceBoards[0], 0xffffffffffffffff);
	for (RawboardIndex i = 1; i < 13; i++) {
		EXPECT_EQ(board.pieceBoards[i], 0);
	}
}

TEST_F(BoardTest, allBoardsTest) {
    Game* game = FEN::fenToNewGame(Positions::INITIAL_FEN_POSITION);
    Board& board = game->getBoard();
    EXPECT_EQ(~board.EMPTY, board.BOARD(WHITE) | board.BOARD(BLACK));
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
    GTEST_SKIP_("fix Board::setBoard");
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

TEST_F(BoardTest, setPieceTest) {
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

TEST_F(BoardTest, isUnderCheckTest) {
    Board board;
    board.setPiece(18, WQueen);

    GTEST_ASSERT_FALSE(board.isUnderCheck(33, WHITE));
    GTEST_ASSERT_FALSE(board.isUnderCheck(33, BLACK));
    GTEST_ASSERT_FALSE(board.isUnderCheck(34, WHITE));
    GTEST_ASSERT_TRUE(board.isUnderCheck(34, BLACK));
}


template <typename... SetOfPosition>
static bool checkBoard(Rawboard board, SetOfPosition... expectedPositions) {
    Rawboard bitPositions = 0;

    for (Position position : {expectedPositions...}) {
        if (position != NO_POS) {
            bitPositions |= Board::posInd(position);
        }
    }

    return board == bitPositions;
}

static bool checkBoard(Rawboard board, list<Position> expectedPositions) {
    Rawboard bitPositions = 0;

    for (Position position : expectedPositions) {
        if (position != NO_POS) {
            bitPositions |= Board::posInd(position);
        }
    }

    return board == bitPositions;
}

static bool checkBoardNoPos(Rawboard board) {
    return checkBoard(board, NO_POS);
}


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


class GetKingAttacksTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(GetKingAttacksTest, getKingAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().getKingAttacks(params->side), *params->expectedPositions));
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


class GetKingMovesTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(GetKingMovesTest, getKingMovesTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().getKingMoves(params->side, game->getCastlingInfo()), *params->expectedPositions));
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
		BoardUtils::initRayAttacks();
	}
};

TEST_P(GetQueenAttacksTest, getQueenAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
	GTEST_ASSERT_TRUE(checkBoard(game->getBoard().getQueenAttacks(params->side), *params->expectedPositions));
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
		BoardUtils::initRayAttacks();
	}
};

TEST_P(GetRookAttacksTest, getRookAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().getRookAttacks(params->side), *params->expectedPositions));
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
		BoardUtils::initRayAttacks();
	}
};

TEST_P(GetBishopAttacksTest, getBishopAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().getBishopAttacks(params->side), *params->expectedPositions));
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
		BoardUtils::initRayAttacks();
	}
};

TEST_P(GetKnightAttacksTest, getKnightAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().getKnightAttacks(params->side), *params->expectedPositions));
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
		BoardUtils::initRayAttacks();
	}
};

TEST_P(GetPawnMovesTest, getPawnMovesTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().getPawnMoves(params->side, game->getEnPassantPosition()), *params->expectedPositions));
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
		BoardUtils::initRayAttacks();
	}
};

TEST_P(GetSinglePawnMovesTest, getSinglePawnMovesTest) {
    TestParams2* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().getPawnMoves(params->position, params->side, game->getEnPassantPosition()), *params->expectedPositions));
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
		BoardUtils::initRayAttacks();
	}
};

TEST_P(GetPawnAttacksTest, getPawnAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().getPawnAttacks(params->side), *params->expectedPositions));
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
    GTEST_ASSERT_TRUE(checkBoard(board.getPawnMoves(49, WHITE, 0), 40, 41, 42, 33));
    EXPECT_EQ(board.getPawnMoves(49, WHITE, 0), 0x70200000000LL);
    board.setPiece(33, BPawn);   // disable double push
    EXPECT_EQ(board.getPawnMoves(49, WHITE, 0), 0x70000000000LL);
    board.setPiece(41, BPawn);
    EXPECT_EQ(board.getPawnMoves(49, WHITE, 0), 0x50000000000LL);
    board.setPiece(40, WPawn);
    EXPECT_EQ(board.getPawnMoves(49, WHITE, 0), 0x40000000000LL);

    board.setPiece(9, BPawn);
    board.setPiece(16, WPawn);
    board.setPiece(18, WPawn);
    EXPECT_EQ(board.getPawnMoves(9, BLACK, 0), 0x2070000LL);
    board.setPiece(25, WPawn);   // disable double push
    EXPECT_EQ(board.getPawnMoves(9, BLACK, 0), 0x70000LL);
    board.setPiece(17, WPawn);
    EXPECT_EQ(board.getPawnMoves(9, BLACK, 0), 0x50000LL);
    board.setPiece(16, BPawn);
    EXPECT_EQ(board.getPawnMoves(9, BLACK, 0), 0x40000LL);

    board.setPiece(0, WPawn);
    EXPECT_EQ(board.getPawnMoves(0, WHITE, 0), 0x0LL);
    board.setPiece(63, BPawn);
    GTEST_ASSERT_TRUE(checkBoardNoPos(board.getPawnMoves(63, BLACK, 0)));
    EXPECT_EQ(board.getPawnMoves(63, BLACK, 0), 0x0LL);

    board.reset();

    // EnPassant
    EXPECT_EQ(board.getPawnMoves(24, WHITE, 18), 0x10000LL);
    EXPECT_EQ(board.getPawnMoves(24, WHITE, 17), 0x30000LL);
    EXPECT_EQ(board.getPawnMoves(25, WHITE, 18), 0x60000LL);
    EXPECT_EQ(board.getPawnMoves(25, WHITE, 16), 0x30000LL);
    EXPECT_EQ(board.getPawnMoves(32, BLACK, 42), 0x10000000000LL);
    EXPECT_EQ(board.getPawnMoves(32, BLACK, 41), 0x30000000000LL);
    EXPECT_EQ(board.getPawnMoves(33, BLACK, 42), 0x60000000000LL);
    EXPECT_EQ(board.getPawnMoves(33, BLACK, 40), 0x30000000000LL);
}

TEST_F(BoardTest, getKnightPositionsTest) {
    Board board;
    EXPECT_EQ(board.getKnightMoves(0, BLACK), 0x20400LL);
    EXPECT_EQ(board.getKnightMoves(7, BLACK), 0x402000LL);
    EXPECT_EQ(board.getKnightMoves(63, BLACK), 0x20400000000000LL);
    EXPECT_EQ(board.getKnightMoves(56, BLACK), 0x4020000000000LL);
    EXPECT_EQ(board.getKnightMoves(45, BLACK), 0x5088008850000000LL);
    board.setPiece(55, BPawn);
    EXPECT_EQ(board.getKnightMoves(45, WHITE), 0x5088008850000000LL);
    EXPECT_EQ(board.getKnightMoves(45, BLACK), 0x5008008850000000LL);
}

TEST_F(BoardTest, getBishopPositionsTest) {
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
}

TEST_F(BoardTest, getKingPositionTest) {
    Board board;
    EXPECT_EQ(board.getKingMoves(0, BLACK, (CastlingInfo)0b1111), 0x302LL);
    EXPECT_EQ(board.getKingMoves(7, BLACK, (CastlingInfo)0b1111), 0xc040LL);
    EXPECT_EQ(board.getKingMoves(63, BLACK, (CastlingInfo)0b1111), 0x40c0000000000000LL);
    EXPECT_EQ(board.getKingMoves(56, BLACK, (CastlingInfo)0b1111), 0x203000000000000LL);
    EXPECT_EQ(board.getKingMoves(54, BLACK, (CastlingInfo)0b1111), 0xe0a0e00000000000LL);
    board.setPiece(62, BPawn);
    EXPECT_EQ(board.getKingMoves(54, BLACK, (CastlingInfo)0b1111), 0xa0a0e00000000000LL);
    EXPECT_EQ(board.getKingMoves(54, WHITE, (CastlingInfo)0b1111), 0xe0a0e00000000000LL);

    // castling
    EXPECT_EQ(board.getKingMoves(4, BLACK, (CastlingInfo)0b1111), 0x386cLL);
    EXPECT_EQ(board.getKingMoves(4, BLACK, (CastlingInfo)0b1110), 0x3868LL);
    EXPECT_EQ(board.getKingMoves(4, BLACK, (CastlingInfo)0b1100), 0x3828LL);
    EXPECT_EQ(board.getKingMoves(60, WHITE, (CastlingInfo)0b1111), 0x2c38000000000000LL);
    EXPECT_EQ(board.getKingMoves(60, WHITE, (CastlingInfo)0b1011), 0x2838000000000000LL);
    EXPECT_EQ(board.getKingMoves(60, WHITE, (CastlingInfo)0b0011), 0x2838000000000000LL);
}

TEST_F(BoardTest, slidingAttackTest) {
    Board board;
    Rawboard posInd = Board::posInd(36);
    EXPECT_EQ(board.slidingAttack(Board::soEastOne, posInd, board.BOARD(WHITE)), 0x8040200000000000LL);
    board.setPiece(54, WPawn);
    EXPECT_EQ(board.slidingAttack(Board::soEastOne, posInd, board.BOARD(WHITE)), 0x40200000000000LL);
    board.setPiece(54, BPawn);
    EXPECT_EQ(board.slidingAttack(Board::soEastOne, posInd, board.BOARD(WHITE)), 0x200000000000LL);
    board.setPiece(29, WPawn);
    GTEST_ASSERT_TRUE(checkBoard(board.slidingAttack(Board::noEastOne, posInd, board.BOARD(WHITE)), 29));
}

// TODO move to BitwiseTest
/*TEST_F(BoardTest, masksTest) {
    Board board;
    GTEST_ASSERT_TRUE(checkBoard(Board::lineMask(0), 0, 1, 2, 3, 4, 5, 6, 7));
    GTEST_ASSERT_TRUE(checkBoard(Board::columnMask(0), 0, 8, 16, 24, 32, 40, 48, 56));
    GTEST_ASSERT_TRUE(checkBoard(board.diagonalMask(0), 0, 9, 18, 27, 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.antiDiagonalMask(0), 0));

    GTEST_ASSERT_TRUE(checkBoard(Board::lineMask(7), 0, 1, 2, 3, 4, 5, 6, 7));
    GTEST_ASSERT_TRUE(checkBoard(Board::columnMask(7), 7, 15, 23, 31, 39, 47, 55, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.diagonalMask(7), 7));
    GTEST_ASSERT_TRUE(checkBoard(board.antiDiagonalMask(7), 7, 14, 21, 28, 35, 42, 49, 56));

    GTEST_ASSERT_TRUE(checkBoard(Board::lineMask(56), 56, 57, 58, 59, 60, 61, 62, 63));
    GTEST_ASSERT_TRUE(checkBoard(Board::columnMask(56), 0, 8, 16, 24, 32, 40, 48, 56));
    GTEST_ASSERT_TRUE(checkBoard(board.diagonalMask(56), 56));
    GTEST_ASSERT_TRUE(checkBoard(board.antiDiagonalMask(56), 7, 14, 21, 28, 35, 42, 49, 56));

    GTEST_ASSERT_TRUE(checkBoard(Board::lineMask(63), 56, 57, 58, 59, 60, 61, 62, 63));
    GTEST_ASSERT_TRUE(checkBoard(Board::columnMask(63), 7, 15, 23, 31, 39, 47, 55, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.diagonalMask(63), 0, 9, 18, 27, 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(board.antiDiagonalMask(63), 63));
}

TEST_F(BoardTest, raysTest) {
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
}*/

TEST_F(BoardTest, attackTest1) {
    Game* game = FEN::fenToNewGame("4k3/8/8/8/3Q4/8/8/4K3 w - - 0 1");
    Rawboard occupied = ~game->getBoard().EMPTY;

    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().northAttack(occupied, 35), 3, 11, 19, 27));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().noEastAttack(occupied, 35), 7, 14, 21, 28));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().eastAttack(occupied, 35), 36, 37, 38, 39));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().soEastAttack(occupied, 35), 44, 53, 62));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().southAttack(occupied, 35), 43, 51, 59));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().soWestAttack(occupied, 35), 42, 49, 56));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().westAttack(occupied, 35), 34, 33, 32));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().noWestAttack(occupied, 35), 8, 17, 26));

	delete game;
}

TEST_F(BoardTest, attackTest2) {
    Game* game = FEN::fenToNewGame("4k3/8/1P1p1p2/8/1p1Q1P2/8/1P1P1p2/4K3 w - - 0 1");
    Rawboard occupied = ~game->getBoard().EMPTY;

    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().northAttack(occupied, 35), 19, 27));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().noEastAttack(occupied, 35), 21, 28));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().eastAttack(occupied, 35), 36, 37));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().soEastAttack(occupied, 35), 44, 53));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().southAttack(occupied, 35), 43, 51));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().soWestAttack(occupied, 35), 42, 49));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().westAttack(occupied, 35), 34, 33));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().noWestAttack(occupied, 35), 17, 26));

	delete game;
}

TEST_F(BoardTest,  attackTest3) {
    Game* game = FEN::fenToNewGame("4k3/p2P2P1/1P1p1p2/8/Pp1Q1Pp1/8/1P1P1p2/p2pK1P1 w - - 0 1");
    Rawboard occupied = ~game->getBoard().EMPTY;

    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().northAttack(occupied, 35), 19, 27));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().noEastAttack(occupied, 35), 21, 28));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().eastAttack(occupied, 35), 36, 37));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().soEastAttack(occupied, 35), 44, 53));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().southAttack(occupied, 35), 43, 51));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().soWestAttack(occupied, 35), 42, 49));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().westAttack(occupied, 35), 34, 33));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().noWestAttack(occupied, 35), 17, 26));

	delete game;
}

TEST_F(BoardTest, attackTest4) {
    Game* game = FEN::fenToNewGame("4k3/p2P2P1/8/2Ppp3/P1pQP1p1/2PPp3/8/p2pK1P1 w - - 0 1");
    Rawboard occupied = ~game->getBoard().EMPTY;

    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().northAttack(occupied, 35), 27));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().noEastAttack(occupied, 35), 28));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().eastAttack(occupied, 35), 36));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().soEastAttack(occupied, 35), 44));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().southAttack(occupied, 35), 43));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().soWestAttack(occupied, 35), 42));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().westAttack(occupied, 35), 34));
    GTEST_ASSERT_TRUE(checkBoard(game->getBoard().noWestAttack(occupied, 35), 26));

	delete game;
}