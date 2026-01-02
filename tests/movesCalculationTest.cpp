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
    TestParams(string fenGame, Side side, Rawboard expectedPositions) {
        this->fenGame = fenGame;
        this->side = side;
        this->expectedPositions = expectedPositions;
    }

    string fenGame;
    Side side;
    Rawboard expectedPositions;
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
    EXPECT_EQ(allKingAttacks(game->board, params->side), params->expectedPositions);
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetKingAttacksTest,
    ::testing::Values(
        new TestParams("8/8/8/3K4/8/8/8/8 w - - 0 1", _WHITE, 30872694685696),
        new TestParams("8/8/8/8/8/5k2/8/8 w - - 0 1", _BLACK, 1884319744),
        new TestParams("K7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, 144959613005987840),
        new TestParams("8/8/8/8/8/8/6k1/8 w - - 0 1", _BLACK, 14721248),
        new TestParams("8/8/1p6/K7/1P6/8/8/8 w - - 0 1", _WHITE, 3307141595136),
        new TestParams("8/8/8/7P/6pk/8/8/8 w - - 0 1", _BLACK, 824646303744)
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

    EXPECT_EQ(allKingMoves(game->board, params->side), params->expectedPositions);

    delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetKingMovesTest,
    ::testing::Values(
        new TestParams("8/8/8/3K4/8/8/8/8 w - - 0 1", _WHITE, 30872694685696),
        new TestParams("8/8/8/8/8/5k2/8/8 w - - 0 1", _BLACK, 1884319744),
        new TestParams("K7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, 144959613005987840),
        new TestParams("8/8/8/8/8/8/6k1/8 w - - 0 1", _BLACK, 14721248),
        new TestParams("8/8/1p6/K7/1P6/8/8/8 w - - 0 1", _WHITE, 3307141595136),
        new TestParams("8/8/8/7P/6pk/8/8/8 w - - 0 1", _BLACK, 824646303744),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQkq - 0 1", _BLACK, 7797982754792013824),
        new TestParams("8/8/8/8/8/8/8/4K3 w KQkq - 0 1", _WHITE, 14444),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQk - 0 1", _BLACK, 7509752378640302080),
        new TestParams("8/8/8/8/8/8/8/4K3 w Kkq - 0 1", _WHITE, 14440),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQq - 0 1", _BLACK, 3186296736364625920),
        new TestParams("8/8/8/8/8/8/8/4K3 w Qkq - 0 1", _WHITE, 14380),
        new TestParams("4k3/8/8/8/8/8/8/8 w KQ - 0 1", _BLACK, 2898066360212914176),
        new TestParams("8/8/8/8/8/8/8/4K3 w kq - 0 1", _WHITE, 14376)
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
	EXPECT_EQ(allQueenAttacks(game->board, params->side), params->expectedPositions);
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetQueenAttacksTest,
    ::testing::Values(
        new TestParams("8/8/8/3Q4/8/8/8/8 w - - 0 1", _WHITE, 5272058161445620104),
        new TestParams("8/8/8/8/8/5q2/8/8 w - - 0 1", _BLACK, 2387511058326581416),
        new TestParams("Q7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, 18303478847064064385UL),
        new TestParams("8/8/8/8/8/8/6q1/8 w - - 0 1", _BLACK, 4702396038313459680),
        new TestParams("8/2p5/8/Q7/8/2P5/P7/8 w - - 0 1", _WHITE, 73469358428454912),
        new TestParams("8/8/5P1P/8/7q/8/5p2/8 w - - 0 1", _BLACK, 176748637487232)
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
    EXPECT_EQ(allRookAttacks(game->board, params->side), params->expectedPositions);
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetRookAttacksTest,
    ::testing::Values(
        new TestParams("8/8/1R6/8/8/8/8/8 w - - 0 1", _WHITE, 144956323094725122),
        new TestParams("8/8/8/8/8/8/6r1/8 w - - 0 1", _BLACK, 4629771061636939584),
        new TestParams("RP6/P7/8/8/8/8/8/8 w - - 0 1", _WHITE, 0),
        new TestParams("8/8/8/8/8/8/7p/6pr w - - 0 1", _BLACK, 0),
        new TestParams("8/8/1R1P4/8/1p6/8/8/8 w - - 0 1", _WHITE, 144683644210905088),
        new TestParams("8/8/8/8/6p1/8/4P1r1/8 w - - 0 1", _BLACK, 4239424)
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
    EXPECT_EQ(allBishopAttacks(game->board, params->side), params->expectedPositions);
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetBishopAttacksTest,
    ::testing::Values(
        new TestParams("8/8/8/3B4/8/8/8/8 w - - 0 1", _WHITE, 4693335752243822976),
        new TestParams("8/8/8/8/8/5b2/8/8 w - - 0 1", _BLACK, 72625527495610504),
        new TestParams("B7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, 567382630219904),
        new TestParams("8/8/8/8/8/8/6b1/8 w - - 0 1", _BLACK, 72624976676520096),
        new TestParams("8/2p5/8/B7/8/2P5/8/8 w - - 0 1", _WHITE, 1128098963652608),
        new TestParams("8/8/5P2/8/7b/8/5p2/8 w - - 0 1", _BLACK, 35459254190080)
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
    EXPECT_EQ(allKnightAttacks(game->board, params->side), params->expectedPositions);
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetKnightAttacksTest,
    ::testing::Values(
        new TestParams("8/8/2N5/8/8/8/8/8 w - - 0 1", _WHITE, 725361088165576704),
        new TestParams("8/8/8/8/3n4/8/8/8 w - - 0 1", _BLACK, 22136263676928),
        new TestParams("N7/8/8/8/8/8/8/8 w - - 0 1", _WHITE, 1128098930098176),
        new TestParams("8/8/8/8/8/8/8/n7 w - - 0 1", _BLACK, 132096),
        new TestParams("1P6/P7/2N5/8/1p1p4/8/8/8 w - - 0 1", _WHITE, 580964425113010176),
        new TestParams("8/8/8/8/3n4/1P3P2/2p1p3/8 w - - 0 1", _BLACK, 22136263671808)
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
    EXPECT_EQ(allPawnMoves(game->board, params->side), params->expectedPositions);
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetPawnMovesTest,
    ::testing::Values(
        new TestParams("8/p7/8/8/8/8/8/8 w - - 0 1", _BLACK, 1103806595072),
        new TestParams("8/8/p7/8/8/8/8/8 w - - 0 1", _BLACK, 4294967296),
        new TestParams("8/8/8/8/8/8/P7/8 w - - 0 1", _WHITE, 16842752),
        new TestParams("8/8/8/8/8/P7/8/8 w - - 0 1", _WHITE, 16777216),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", _BLACK, 0),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", _WHITE, 0),
        new TestParams("8/8/8/3p4/2P1b3/8/8/8 w - - 0 1", _BLACK, 201326592),
        new TestParams("8/8/8/2B1p3/3P4/8/8/8 w - - 0 1", _WHITE, 103079215104),
        new TestParams("8/8/8/pP6/8/8/8/8 w - a6 0 1", _WHITE, 3298534883328),
        new TestParams("8/8/8/pP5P/8/8/8/8 w - a6 0 1", _WHITE, 144036023238656),
        new TestParams("8/8/8/8/6Pp/8/8/8 w - g3 0 1", _BLACK, 12582912),
        new TestParams("8/8/8/8/p5Pp/8/8/8 w - g3 0 1", _BLACK, 12648448)
    )
);


class TestParams2 {
public:
    TestParams2(const string& fenGame, const Side side, const Position position, const Rawboard expectedPositions) {
        this->fenGame = fenGame;
        this->side = side;
        this->position = position;
        this->expectedPositions = expectedPositions;
    }

    
    string fenGame;
    Side side;
    Position position;
    Rawboard expectedPositions;
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
        EXPECT_EQ(blackPawnMoves(game->board, params->position, params->side), params->expectedPositions);
    } else {
        EXPECT_EQ(whitePawnMoves(game->board, params->position, params->side), params->expectedPositions);
    }

	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetSinglePawnMovesTest,
    ::testing::Values(
        new TestParams2("8/6bb/8/8/R1pP2k1/4P3/P7/K7 w - d3 0 1", _BLACK, 26, 786432),						// Following from https://www.chessprogramming.org/En_passant
        new TestParams2("2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K w - b3 0 1", _BLACK, 24, 131072),
        new TestParams2("2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K w - b3 0 1", _BLACK, 26, 131072),
        new TestParams2("rnbqkbnr/ppppppp1/8/7p/P7/8/1PPPPPPP/RNBQKBNR w - h6 0 1", _WHITE, 24, 4294967296)
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
    EXPECT_EQ(allPawnAttacks(game->board, params->side), params->expectedPositions);
	delete params;
	delete game;
}

INSTANTIATE_TEST_SUITE_P(
    MovesCalculationTest,
    GetPawnAttacksTest,
    ::testing::Values(
        new TestParams("8/p7/8/8/8/8/8/8 w - - 0 1", _BLACK, 2199023255552),
        new TestParams("8/8/p7/8/8/8/8/8 w - - 0 1", _BLACK, 8589934592),
        new TestParams("8/8/8/8/8/8/P7/8 w - - 0 1", _WHITE, 131072),
        new TestParams("8/8/8/8/8/P7/8/8 w - - 0 1", _WHITE, 33554432),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", _BLACK, 335544320),
        new TestParams("8/8/8/3p4/3P4/8/8/8 w - - 0 1", _WHITE, 85899345920),
        new TestParams("8/8/8/3p4/2P1b3/8/8/8 w - - 0 1", _BLACK, 67108864),
        new TestParams("8/8/8/2B1p3/3P4/8/8/8 w - - 0 1", _WHITE, 68719476736)
    )
);


TEST_F(MovesCalculationTest, getPawnMovesTest) {
    Board *board = new Board();
    reset(board);
    setPiece(board, 49, BPawn);
    setPiece(board, 40, WPawn);
    setPiece(board, 42, WPawn);
    EXPECT_EQ(blackPawnMoves(board, 49, _BLACK), 7705171329024);
    EXPECT_EQ(blackPawnMoves(board, 49, _BLACK), 0x70200000000LL);
    setPiece(board, 33, WPawn);   // disable double push
    EXPECT_EQ(blackPawnMoves(board, 49, _BLACK), 0x70000000000LL);
    setPiece(board, 41, WPawn);
    EXPECT_EQ(blackPawnMoves(board, 49, _BLACK), 0x50000000000LL);
    setPiece(board, 40, BPawn);
    EXPECT_EQ(blackPawnMoves(board, 49, _BLACK), 0x40000000000LL);

    setPiece(board, 9, WPawn);
    setPiece(board, 16, BPawn);
    setPiece(board, 18, BPawn);
    EXPECT_EQ(whitePawnMoves(board, 9, _WHITE), 0x2070000LL);
    setPiece(board, 25, BPawn);   // disable double push
    EXPECT_EQ(whitePawnMoves(board, 9, _WHITE), 0x70000LL);
    setPiece(board, 17, BPawn);
    EXPECT_EQ(whitePawnMoves(board, 9, _WHITE), 0x50000LL);
    setPiece(board, 16, WPawn);
    EXPECT_EQ(whitePawnMoves(board, 9, _WHITE), 0x40000LL);

    setPiece(board, 63, WPawn);
    EXPECT_EQ(whitePawnMoves(board, 63, _WHITE), 0x0LL);
    setPiece(board, 0, BPawn);
    EXPECT_EQ(blackPawnMoves(board, 0, _BLACK), 0x0LL);

    reset(board);
    // EnPassant
	board->enPassantPosition = 18;
    EXPECT_EQ(blackPawnMoves(board, 24, _BLACK), 65536);
	board->enPassantPosition = 17;
    EXPECT_EQ(blackPawnMoves(board, 24, _BLACK), 196608);
	board->enPassantPosition = 18;
    EXPECT_EQ(blackPawnMoves(board, 25, _BLACK), 393216);
	board->enPassantPosition = 16;
    EXPECT_EQ(blackPawnMoves(board, 25, _BLACK), 196608);
	board->enPassantPosition = 42;
    EXPECT_EQ(whitePawnMoves(board, 32, _WHITE), 1099511627776);
	board->enPassantPosition = 41;
    EXPECT_EQ(whitePawnMoves(board, 32, _WHITE), 3298534883328);
	board->enPassantPosition = 42;
    EXPECT_EQ(whitePawnMoves(board, 33, _WHITE), 6597069766656);
	board->enPassantPosition = 40;
    EXPECT_EQ(whitePawnMoves(board, 33, _WHITE), 3298534883328);
}

TEST_F(MovesCalculationTest, getKingMovesTest) {
    Board *board = new Board();
    reset(board);
	board->castlingInfo = 0b1111;
    EXPECT_EQ(blackKingMoves(board, 0, _BLACK), 0x302LL);
    EXPECT_EQ(blackKingMoves(board, 7, _BLACK), 0xc040LL);
    EXPECT_EQ(blackKingMoves(board, 63, _BLACK), 0x40c0000000000000LL);
    EXPECT_EQ(blackKingMoves(board, 56, _BLACK), 0x203000000000000LL);
    EXPECT_EQ(blackKingMoves(board, 54, _BLACK), 0xe0a0e00000000000LL);
    setPiece(board, 62, BPawn);
    EXPECT_EQ(blackKingMoves(board, 54, _BLACK), 0xa0a0e00000000000LL);
    EXPECT_EQ(whiteKingMoves(board, 54, _WHITE), 0xe0a0e00000000000LL);

    // castling
	board->castlingInfo = 0b1111;
    EXPECT_EQ(whiteKingMoves(board, 4, _WHITE), 0x386cLL);
	board->castlingInfo = 0b1011;
    EXPECT_EQ(whiteKingMoves(board, 4, _WHITE), 0x3868LL);
	board->castlingInfo = 0b0011;
    EXPECT_EQ(whiteKingMoves(board, 4, _WHITE), 0x3828LL);
	board->castlingInfo = 0b1111;
    EXPECT_EQ(blackKingMoves(board, 60, _BLACK), 0x2c38000000000000LL);
	board->castlingInfo = 0b1110;
    EXPECT_EQ(blackKingMoves(board, 60, _BLACK), 0x2838000000000000LL);
	board->castlingInfo = 0b1100;
    EXPECT_EQ(blackKingMoves(board, 60, _BLACK), 0x2838000000000000LL);
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