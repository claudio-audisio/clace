#include <gtest/gtest.h>

#include "../engine/bf_engine.h"
#include "../ui/userInterface.h"
#include "../common/defines.h"

using namespace std;


class BFEngineTest : public testing::Test {
protected:
	BFEngineTest() {
		initAttacks();
	}
	~BFEngineTest() {

	}
};

TEST_F(BFEngineTest, ConstructorTest) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	auto engine = new BF_Engine(3);

	EXPECT_EQ(engine->depth, 3);
	EXPECT_EQ(engine->pool->size, 4);
}

TEST_F(BFEngineTest, Depth1Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(1);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(MoveHelper::toString(best.move), "e2e4");
}

TEST_F(BFEngineTest, Depth2Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(2);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(MoveHelper::toString(best.move), "e2e4");
}

TEST_F(BFEngineTest, Depth3Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(3);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(MoveHelper::toString(best.move), "e2e3");
}

TEST_F(BFEngineTest, Depth4Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(4);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(MoveHelper::toString(best.move), "e2e3");
}

TEST_F(BFEngineTest, Depth5Test) {
#ifndef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(5);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(MoveHelper::toString(best.move), "e2e4");
}

