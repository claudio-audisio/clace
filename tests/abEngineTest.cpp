#include <gtest/gtest.h>

#include "../engine/bf_engine.h"
#include "../ui/userInterface.h"
#include "../common/defines.h"
#include "engine/ab_engine.h"

using namespace std;


class ABEngineTest : public testing::Test {
protected:
	ABEngineTest() {
		initMovesGenerator();
	}
	~ABEngineTest() {

	}
};

TEST_F(ABEngineTest, ConstructorTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	auto engine = new AB_Engine(3);

	EXPECT_EQ(engine->depth, 3);
	EXPECT_EQ(engine->pool->size, 4);
}

TEST_F(ABEngineTest, enPassantBugTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(FEN_EN_PASSANT_BUG_TEST);
	auto engine = new AB_Engine(5);
	Evaluation best = engine->calculateMove(game);

	EXPECT_NE(moveToString(best.move), "c4b3");
}

TEST_F(ABEngineTest, Depth5Test) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new AB_Engine(5);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2e4");		// move based on LCZero 0.31.2 - https://nextchessmove.com/
}

TEST_F(ABEngineTest, Depth4TestOnFenPosition2) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(PERFT_FEN_POSITION_2);
	auto engine = new AB_Engine(4);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2a6");		// move based on LCZero 0.31.2 - https://nextchessmove.com/
}

TEST_F(ABEngineTest, Depth6Test) {
#ifndef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new AB_Engine(6);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2e3");
}