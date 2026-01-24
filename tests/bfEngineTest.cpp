#include <gtest/gtest.h>

#include "../engine/bruteForceEngine.h"
#include "../ui/userInterface.h"
#include "../common/defines.h"

using namespace std;


class BFEngineTest : public testing::Test {
protected:
	BFEngineTest() {
		initMovesGenerator();
	}
	~BFEngineTest() {

	}
};

TEST_F(BFEngineTest, ConstructorTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	auto engine = new BruteForceEngine(3);

	EXPECT_EQ(engine->depth, 3);
	EXPECT_EQ(engine->pool->size, 4);
}

TEST_F(BFEngineTest, enPassantBugTest) {
	//GTEST_SKIP_("");
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(FEN_EN_PASSANT_BUG_TEST);
	auto engine = new BruteForceEngine(4);
	Evaluation best = engine->calculateMove(game);

	EXPECT_NE(moveToString(best.move), "c4b3");
}

TEST_F(BFEngineTest, Depth4Test) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BruteForceEngine(4);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2e4");		// move based on LCZero 0.31.2 - https://nextchessmove.com/
}

TEST_F(BFEngineTest, Depth3TestOnFenPosition2) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(PERFT_FEN_POSITION_2);
	auto engine = new BruteForceEngine(3);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2a6");		// move based on LCZero 0.31.2 - https://nextchessmove.com/
}

TEST_F(BFEngineTest, Depth5Test) {
#ifndef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BruteForceEngine(5);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2e4");
}

