#include <gtest/gtest.h>

#include "../utils/fen.h"
#include "testUtils.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;


class MovesCalculationTest : public testing::Test
{
protected:
    MovesCalculationTest() {
        initAttacks();
        initDestPosProviders();
        initPawnAttacksProviders();
    }
    ~MovesCalculationTest() {

    }
};

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
        initDestPosProviders();
        initPawnAttacksProviders();
    }
};

TEST_P(GetKingAttacksTest, getKingAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(allKingAttacks(game->board, params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetKingAttacksTest,
    ::testing::Values(
        new TestParams("8/8/8/3K4/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 18, 19, 20, 26, 28, 34, 35, 36 }),
        new TestParams("8/8/8/8/8/5k2/8/8 w - - 0 1", _BLACK, new list<Position>{ 36, 37, 38, 44, 46, 52, 53, 54 }),
        new TestParams("K7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 1, 8, 9 }),
        new TestParams("8/8/8/8/8/8/6k1/8 w - - 0 1", _BLACK, new list<Position>{ 45, 46, 47, 53, 55, 61, 62, 63 }),
        new TestParams("8/8/1p6/K7/1P6/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 16, 17, 25, 32 }),
        new TestParams("8/8/8/7P/6pk/8/8/8 w - - 0 1", _BLACK, new list<Position>{ 30, 31, 46, 47 })
    )
);


class GetKingMovesTest : public ::testing::TestWithParam<TestParams*> {
protected:
    GetKingMovesTest() {
        initAttacks();
        initDestPosProviders();
        initPawnAttacksProviders();
    }
};

TEST_P(GetKingMovesTest, getKingMovesTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);

    GTEST_ASSERT_TRUE(checkBoard(allKingMoves(game->board, params->side), *params->expectedPositions));

    delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetKingMovesTest,
    ::testing::Values(
        new TestParams("8/8/8/3K4/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{18, 19, 20, 26, 28, 34, 35, 36}),
        new TestParams("8/8/8/8/8/5k2/8/8 w - - 0 1", _BLACK, new list<Position>{36, 37, 38, 44, 46, 52, 53, 54}),
        new TestParams("K7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{1, 8, 9}),
        new TestParams("8/8/8/8/8/8/6k1/8 w - - 0 1", _BLACK, new list<Position>{45, 46, 47, 53, 55, 61, 62, 63}),
        new TestParams("8/8/1p6/K7/1P6/8/8/8 w - - 0 1", _WHITE, new list<Position>{16, 17, 25, 32}),
        new TestParams("8/8/8/7P/6pk/8/8/8 w - - 0 1", _BLACK, new list<Position>{30, 31, 46, 47}),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQkq - 0 1", _BLACK, new list<Position>{2, 3, 11, 12, 13, 5, 6}),
        new TestParams("8/8/8/8/8/8/8/4K3 w KQkq - 0 1", _WHITE, new list<Position>{58, 59, 51, 52, 53, 61, 62}),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQk - 0 1", _BLACK, new list<Position>{3, 11, 12, 13, 5, 6}),
        new TestParams("8/8/8/8/8/8/8/4K3 w Kkq - 0 1", _WHITE, new list<Position>{59, 51, 52, 53, 61, 62}),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQq - 0 1", _BLACK, new list<Position>{2, 3, 11, 12, 13, 5}),
        new TestParams("8/8/8/8/8/8/8/4K3 w Qkq - 0 1", _WHITE, new list<Position>{58, 59, 51, 52, 53, 61}),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQ - 0 1", _BLACK, new list<Position>{3, 11, 12, 13, 5}),
        new TestParams("8/8/8/8/8/8/8/4K3 w kq - 0 1", _WHITE, new list<Position>{ 59, 51, 52, 53, 61 })
    )
);


class GetQueenAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetQueenAttacksTest() {
		initAttacks();
	    initDestPosProviders();
	    initPawnAttacksProviders();
	}
};

TEST_P(GetQueenAttacksTest, getQueenAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
	GTEST_ASSERT_TRUE(checkBoard(allQueenAttacks(game->board, params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetQueenAttacksTest,
    ::testing::Values(
        new TestParams("8/8/8/3Q4/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{0, 9, 18, 36, 45, 54, 63, 6, 13, 20, 34, 41, 48, 19, 11, 3, 35, 43, 51, 59, 24, 25, 26, 28, 29, 30, 31}),
        new TestParams("8/8/8/8/8/5q2/8/8 w - - 0 1", _BLACK, new list<Position>{ 0, 9, 18, 27, 36, 54, 63, 38, 31, 52, 59, 37, 29, 21, 13, 5, 53, 61, 40, 41, 42, 43, 44, 46, 47 }),
        new TestParams("Q7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 9, 18, 27, 36, 45, 54, 63, 8, 16, 24, 32, 40, 48, 56, 1, 2, 3, 4, 5, 6, 7 }),
        new TestParams("8/8/8/8/8/8/6q1/8 w - - 0 1", _BLACK, new list<Position>{ 0, 9, 18, 27, 36, 45, 63, 61, 47, 46, 38, 30, 22, 14, 6, 62, 48, 49, 50, 51, 52, 53, 55 }),
        new TestParams("8/2p5/8/Q7/8/2P5/P7/8 w - - 0 1", _WHITE, new list<Position>{ 33, 17, 10, 25, 26, 27, 28, 29, 30, 31, 32, 40, 0, 8, 16 }),
        new TestParams("8/8/5P1P/8/7q/8/5p2/8 w - - 0 1", _BLACK, new list<Position>{ 46, 30, 21, 32, 33, 34, 35, 36, 37, 38, 63, 55, 47, 31, 23 })
    )
);


class GetRookAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetRookAttacksTest() {
		initAttacks();
	    initDestPosProviders();
	    initPawnAttacksProviders();
	}
};

TEST_P(GetRookAttacksTest, getRookAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(allRookAttacks(game->board, params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetRookAttacksTest,
    ::testing::Values(
        new TestParams("8/8/1R6/8/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 16, 18, 19, 20, 21, 22, 23, 1, 9, 25, 33, 41, 49, 57 }),
        new TestParams("8/8/8/8/8/8/6r1/8 w - - 0 1", _BLACK, new list<Position>{ 48, 49, 50, 51, 52, 53, 55, 62, 46, 38, 30, 22, 14, 6 }),
        new TestParams("RP6/P7/8/8/8/8/8/8 w - - 0 1", _WHITE, new list<Position>()),
        new TestParams("8/8/8/8/8/8/7p/6pr w - - 0 1", _BLACK, new list<Position>()),
        new TestParams("8/8/1R1P4/8/1p6/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 16, 18, 1, 9, 25, 33 }),
        new TestParams("8/8/8/8/6p1/8/4P1r1/8 w - - 0 1", _BLACK, new list<Position>{ 52, 53, 55, 62, 46 })
    )
);


class GetBishopAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetBishopAttacksTest() {
		initAttacks();
	    initDestPosProviders();
	    initPawnAttacksProviders();
	}
};

TEST_P(GetBishopAttacksTest, getBishopAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(allBishopAttacks(game->board, params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetBishopAttacksTest,
    ::testing::Values(
        new TestParams("8/8/8/3B4/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{0, 9, 18, 36, 45, 54, 63, 6, 13, 20, 34, 41, 48}),
        new TestParams("8/8/8/8/8/5b2/8/8 w - - 0 1", _BLACK, new list<Position>{0, 9, 18, 27, 36, 54, 63, 38, 31, 52, 59}),
        new TestParams("B7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{9, 18, 27, 36, 45, 54, 63}),
        new TestParams("8/8/8/8/8/8/6b1/8 w - - 0 1", _BLACK, new list<Position>{0, 9, 18, 27, 36, 45, 63, 61, 47}),
        new TestParams("8/2p5/8/B7/8/2P5/8/8 w - - 0 1", _WHITE, new list<Position>{33, 17, 10}),
        new TestParams("8/8/5P2/8/7b/8/5p2/8 w - - 0 1", _BLACK, new list<Position>{46, 30, 21})
    )
);


class GetKnightAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetKnightAttacksTest() {
		initAttacks();
	    initDestPosProviders();
	    initPawnAttacksProviders();
	}
};

TEST_P(GetKnightAttacksTest, getKnightAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(allKnightAttacks(game->board, params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetKnightAttacksTest,
    ::testing::Values(
        new TestParams("8/8/2N5/8/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 1, 8, 24, 33, 35, 28, 3, 12 }),
        new TestParams("8/8/8/8/3n4/8/8/8 w - - 0 1", _BLACK, new list<Position>{ 20, 18, 25, 29, 41, 45, 50, 52 }),
        new TestParams("N7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 10, 17 }),
        new TestParams("8/8/8/8/8/8/8/n7 w - - 0 1", _BLACK, new list<Position>{ 41, 50 }),
        new TestParams("1P6/P7/2N5/8/1p1p4/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 24, 33, 35, 28, 3, 12 }),
        new TestParams("8/8/8/8/3n4/1P3P2/2p1p3/8 w - - 0 1", _BLACK, new list<Position>{ 20, 18, 25, 29, 41, 45 })
    )
);


class GetPawnMovesTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetPawnMovesTest() {
		initAttacks();
	    initDestPosProviders();
	    initPawnAttacksProviders();
	}
};

TEST_P(GetPawnMovesTest, getPawnMovesTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(allPawnMoves(game->board, params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetPawnMovesTest,
    ::testing::Values(
        new TestParams("8/p7/8/8/8/8/8/8 w - - 0 1", _BLACK, new list<Position>{ 16, 24 }),
        new TestParams("8/8/p7/8/8/8/8/8 w - - 0 1", _BLACK, new list<Position>{ 24 }),
        new TestParams("8/8/8/8/8/8/P7/8 w - - 0 1", _WHITE, new list<Position>{ 40, 32 }),
        new TestParams("8/8/8/8/8/P7/8/8 w - - 0 1", _WHITE, new list<Position>{ 32 }),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", _BLACK, new list<Position>()),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", _WHITE, new list<Position>()),
        new TestParams("8/8/8/3p4/2P1b3/8/8/8 w - - 0 1", _BLACK, new list<Position>{ 34, 35 }),
        new TestParams("8/8/8/2B1p3/3P4/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 27, 28 }),
        new TestParams("8/8/8/pP6/8/8/8/8 w - a6 0 1", _WHITE, new list<Position>{ 16, 17 }),
        new TestParams("8/8/8/pP5P/8/8/8/8 w - a6 0 1", _WHITE, new list<Position>{ 16, 17, 23 }),
        new TestParams("8/8/8/8/6Pp/8/8/8 w - g3 0 1", _BLACK, new list<Position>{ 46, 47 }),
        new TestParams("8/8/8/8/p5Pp/8/8/8 w - g3 0 1", _BLACK, new list<Position>{ 40, 46, 47 })
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
	    initDestPosProviders();
	    initPawnAttacksProviders();
	}
};

TEST_P(GetSinglePawnMovesTest, getSinglePawnMovesTest) {
    TestParams2* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);

    if (params->side) {
        GTEST_ASSERT_TRUE(checkBoard(blackPawnMoves(game->board, params->position, params->side), *params->expectedPositions));
    } else {
        GTEST_ASSERT_TRUE(checkBoard(whitePawnMoves(game->board, params->position, params->side), *params->expectedPositions));
    }

	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetSinglePawnMovesTest,
    ::testing::Values(
        new TestParams2("8/6bb/8/8/R1pP2k1/4P3/P7/K7 w - d3 0 1", _BLACK, 34, new list<Position>{ 42, 43 }),						// Following from https://www.chessprogramming.org/En_passant
        new TestParams2("2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K w - b3 0 1", _BLACK, 32, new list<Position>{ 41 }),
        new TestParams2("2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K w - b3 0 1", _BLACK, 34, new list<Position>{ 41 }),
        new TestParams2("rnbqkbnr/ppppppp1/8/7p/P7/8/1PPPPPPP/RNBQKBNR w - h6 0 1", _WHITE, 32, new list<Position>{ 24 })
    )
);


class GetPawnAttacksTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetPawnAttacksTest() {
		initAttacks();
	    initDestPosProviders();
	    initPawnAttacksProviders();
	}
};

TEST_P(GetPawnAttacksTest, getPawnAttacksTest) {
    TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    GTEST_ASSERT_TRUE(checkBoard(allPawnAttacks(game->board, params->side), *params->expectedPositions));
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetPawnAttacksTest,
    ::testing::Values(
        new TestParams("8/p7/8/8/8/8/8/8 w - - 0 1", _BLACK, new list<Position>{ 17 }),
        new TestParams("8/8/p7/8/8/8/8/8 w - - 0 1", _BLACK, new list<Position>{ 25 }),
        new TestParams("8/8/8/8/8/8/P7/8 w - - 0 1", _WHITE, new list<Position>{ 41 }),
        new TestParams("8/8/8/8/8/P7/8/8 w - - 0 1", _WHITE, new list<Position>{ 33 }),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", _BLACK, new list<Position>{ 34, 36 }),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 26, 28 }),
        new TestParams("8/8/8/3p4/2P1b3/8/8/8 w - - 0 1", _BLACK, new list<Position>{ 34 }),
        new TestParams("8/8/8/2B1p3/3P4/8/8/8 w - - 0 1", _WHITE, new list<Position>{ 28 })
    )
);


TEST_F(MovesCalculationTest, getPawnMovesTest) {
    Board board;
    reset(board);
    setPiece(board, 49, WPawn);
    setPiece(board, 40, BPawn);
    setPiece(board, 42, BPawn);
    GTEST_ASSERT_TRUE(checkBoard(whitePawnMoves(board, 49, _WHITE), 40, 41, 42, 33));
    EXPECT_EQ(whitePawnMoves(board, 49, _WHITE), 0x70200000000LL);
    setPiece(board, 33, BPawn);   // disable double push
    EXPECT_EQ(whitePawnMoves(board, 49, _WHITE), 0x70000000000LL);
    setPiece(board, 41, BPawn);
    EXPECT_EQ(whitePawnMoves(board, 49, _WHITE), 0x50000000000LL);
    setPiece(board, 40, WPawn);
    EXPECT_EQ(whitePawnMoves(board, 49, _WHITE), 0x40000000000LL);

    setPiece(board, 9, BPawn);
    setPiece(board, 16, WPawn);
    setPiece(board, 18, WPawn);
    EXPECT_EQ(blackPawnMoves(board, 9, _BLACK), 0x2070000LL);
    setPiece(board, 25, WPawn);   // disable double push
    EXPECT_EQ(blackPawnMoves(board, 9, _BLACK), 0x70000LL);
    setPiece(board, 17, WPawn);
    EXPECT_EQ(blackPawnMoves(board, 9, _BLACK), 0x50000LL);
    setPiece(board, 16, BPawn);
    EXPECT_EQ(blackPawnMoves(board, 9, _BLACK), 0x40000LL);

    setPiece(board, 0, WPawn);
    EXPECT_EQ(whitePawnMoves(board, 0, _WHITE), 0x0LL);
    setPiece(board, 63, BPawn);
    GTEST_ASSERT_TRUE(checkBoardNoPos(blackPawnMoves(board, 63, _BLACK)));
    EXPECT_EQ(blackPawnMoves(board, 63, _BLACK), 0x0LL);

    reset(board);

    // EnPassant
	board.enPassantPosition = 18;
    EXPECT_EQ(whitePawnMoves(board, 24, _WHITE), 0x10000LL);
	board.enPassantPosition = 17;
    EXPECT_EQ(whitePawnMoves(board, 24, _WHITE), 0x30000LL);
	board.enPassantPosition = 18;
    EXPECT_EQ(whitePawnMoves(board, 25, _WHITE), 0x60000LL);
	board.enPassantPosition = 16;
    EXPECT_EQ(whitePawnMoves(board, 25, _WHITE), 0x30000LL);
	board.enPassantPosition = 42;
    EXPECT_EQ(blackPawnMoves(board, 32, _BLACK), 0x10000000000LL);
	board.enPassantPosition = 41;
    EXPECT_EQ(blackPawnMoves(board, 32, _BLACK), 0x30000000000LL);
	board.enPassantPosition = 42;
    EXPECT_EQ(blackPawnMoves(board, 33, _BLACK), 0x60000000000LL);
	board.enPassantPosition = 40;
    EXPECT_EQ(blackPawnMoves(board, 33, _BLACK), 0x30000000000LL);
}

TEST_F(MovesCalculationTest, getKingMovesTest) {
    Board board;
    reset(board);
	board.castlingInfo = 0b1111;
    EXPECT_EQ(blackKingMoves(board, 0, _BLACK), 0x302LL);
    EXPECT_EQ(blackKingMoves(board, 7, _BLACK), 0xc040LL);
    EXPECT_EQ(blackKingMoves(board, 63, _BLACK), 0x40c0000000000000LL);
    EXPECT_EQ(blackKingMoves(board, 56, _BLACK), 0x203000000000000LL);
    EXPECT_EQ(blackKingMoves(board, 54, _BLACK), 0xe0a0e00000000000LL);
    setPiece(board, 62, BPawn);
    EXPECT_EQ(blackKingMoves(board, 54, _BLACK), 0xa0a0e00000000000LL);
    EXPECT_EQ(whiteKingMoves(board, 54, _WHITE), 0xe0a0e00000000000LL);

    // castling
	board.castlingInfo = 0b1111;
    EXPECT_EQ(blackKingMoves(board, 4, _BLACK), 0x386cLL);
	board.castlingInfo = 0b1110;
    EXPECT_EQ(blackKingMoves(board, 4, _BLACK), 0x3868LL);
	board.castlingInfo = 0b1100;
    EXPECT_EQ(blackKingMoves(board, 4, _BLACK), 0x3828LL);
	board.castlingInfo = 0b1111;
    EXPECT_EQ(whiteKingMoves(board, 60, _WHITE), 0x2c38000000000000LL);
	board.castlingInfo = 0b1011;
    EXPECT_EQ(whiteKingMoves(board, 60, _WHITE), 0x2838000000000000LL);
	board.castlingInfo = 0b0011;
    EXPECT_EQ(whiteKingMoves(board, 60, _WHITE), 0x2838000000000000LL);
}


class TestParams3 {
public:
    TestParams3(Position position, Position excludePosition, Side side, bool expectedResult) {
        this->position = position;
        this->excludePosition = excludePosition;
        this->side = side;
        this->expectedResult = expectedResult;
    }

    Position position;
    Position excludePosition;
    Side side;
    bool expectedResult;
};

class IsOnXRayTest : public ::testing::TestWithParam<TestParams3*> {
protected:
    IsOnXRayTest() {
        initAttacks();
        initDestPosProviders();
        initPawnAttacksProviders();
    }
};

/* TODO da mettere a posto
TEST_P(IsOnXRayTest, isOnXRayTest) {
    TestParams3* params = GetParam();
    Game* game = FEN::fenToNewGame("3bqr2/8/8/7B/7Q/7R/8/8 w - - 0 1");
    game->sideToMove = params->side;

    EXPECT_EQ(isOnXRay(game->board, params->position, params->excludePosition), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsOnXRayTest,
    ::testing::Values(
        new TestParams3(24, 3, _WHITE, false),
        new TestParams3(25, 3, _WHITE, true),
        new TestParams3(26, 3, _WHITE, false),
        new TestParams3(27, 3, _WHITE, false),
        new TestParams3(28, 3, _WHITE, true),
        new TestParams3(29, 3, _WHITE, true),
        new TestParams3(30, 3, _WHITE, false),
        new TestParams3(31, 3, _WHITE, true),
        new TestParams3(4, 31, _BLACK, false),
        new TestParams3(12, 31, _BLACK, true),
        new TestParams3(20, 31, _BLACK, false),
        new TestParams3(28, 31, _BLACK, false),
        new TestParams3(36, 31, _BLACK, true),
        new TestParams3(44, 31, _BLACK, true),
        new TestParams3(52, 31, _BLACK, false),
        new TestParams3(60, 31, _BLACK, true)
    )
);*/