#include <gtest/gtest.h>
#include <string>

#include "../move/movesGenerator.h"
#include "../utils/positions.h"
#include "../utils/fen.h"
#include "../utils/testUtils.h"

using namespace std;


class MovesGeneratorTest : public testing::Test {
protected:
	MovesGeneratorTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}
	~MovesGeneratorTest() {

	}
};


TEST_F(MovesGeneratorTest, calculateLegalMoves) {
	Game* game = FEN::fenToNewGame("rk6/8/8/8/8/1n6/1PPPPPPP/K7 w - - 0 1");
	game->setLastMove(createMove("a5b3", _BLACK));
	game->verifyChecks();

	Move* moves = new Move[MAX_MOVES];
	MovesAmount amount;
	generateLegalMoves(*game, moves, &amount);

	EXPECT_EQ(amount.total, 2);
	EXPECT_EQ(amount.legal, 1);
	EXPECT_EQ(moveToString(moves[0]), "a1b1");
	EXPECT_EQ(moves[1], 0);
}


class TestParams {
public:
	TestParams(string fenGame, unsigned int expectedPossibleMoves) {
		this->fenGame = fenGame;
		this->expectedPossibleMoves = expectedPossibleMoves;
	}

	string fenGame;
	unsigned int expectedPossibleMoves;
};


class GetLegalMovesTest : public ::testing::TestWithParam<TestParams*> {
protected:
	GetLegalMovesTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}
};

TEST_P(GetLegalMovesTest, getLegalMovesTest) {
	TestParams* params = GetParam();
	Game* game = FEN::fenToNewGame(params->fenGame);
	Move* moves = new Move[MAX_MOVES];
	MovesAmount amount;
	generateLegalMoves(*game, moves, &amount);

	EXPECT_EQ(amount.legal, params->expectedPossibleMoves);
}

INSTANTIATE_TEST_SUITE_P(
	MovesGeneratorTest,
	GetLegalMovesTest,
	::testing::Values(
		new TestParams(INITIAL_FEN_POSITION, 20),
		new TestParams("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 48),
		new TestParams(PERFT_FEN_POSITION_3, 14),
		new TestParams("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6),
		new TestParams("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 44),
		new TestParams("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 46),
		new TestParams("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1", 218),
		new TestParams("3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1", 218)
	)
);
