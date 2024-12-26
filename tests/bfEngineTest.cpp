#include <gtest/gtest.h>

#include "../engine/bf_engine.h"
#include "../ui/userInterface.h"

using namespace std;

#define VTUNE_PROFILER


class BFEngineTest : public testing::Test {
protected:
	BFEngineTest() {
		BoardUtils::initAttacks();
	}
	~BFEngineTest() {

	}
};

TEST_F(BFEngineTest, ConstructorTest) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	auto engine = new BF_Engine(WHITE, 3);

	EXPECT_EQ(engine->depth, 5);
	EXPECT_EQ(engine->side, WHITE);
	EXPECT_EQ(engine->pool->size, 5);
}

TEST_F(BFEngineTest, Depth1Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(WHITE, 1);

	auto moves = new vector<Move>();
	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	EXPECT_EQ(MoveHelper::toString(best.first), "e2-e4");
}

TEST_F(BFEngineTest, Depth2Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(WHITE, 2);

	auto moves = new vector<Move>();
	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	EXPECT_EQ(MoveHelper::toString(best.first), "e2-e3");
}

TEST_F(BFEngineTest, Depth3Test) {
#ifndef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(WHITE, 3);
	auto moves = new vector<Move>();

	auto begin = chrono::steady_clock::now();

	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	cout << "time: " << Utils::getElapsedMillis(begin) << endl;
	// 10747

	EXPECT_EQ(MoveHelper::toString(best.first), "e2-e4");
}

