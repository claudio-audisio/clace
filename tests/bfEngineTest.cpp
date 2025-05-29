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
	auto engine = new BF_Engine(3);

	EXPECT_EQ(engine->depth, 3);
	EXPECT_EQ(engine->pool->size, 3);
}

TEST_F(BFEngineTest, Depth1Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(1);
	engine->setEvaluator(new BasicEvaluator());

	auto moves = new vector<Move>();
	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	EXPECT_EQ(MoveHelper::toString(best.first), "e2e4");
}

TEST_F(BFEngineTest, Depth2Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(2);
	engine->setEvaluator(new BasicEvaluator());

	auto moves = new vector<Move>();
	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	EXPECT_EQ(MoveHelper::toString(best.first), "e2e4");
}

TEST_F(BFEngineTest, Depth3Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(3);
	engine->setEvaluator(new BasicEvaluator());

	auto moves = new vector<Move>();
	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	EXPECT_EQ(MoveHelper::toString(best.first), "e2e3");
}

TEST_F(BFEngineTest, Depth4Test) {
#ifdef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(4);
	engine->setEvaluator(new BasicEvaluator());

	auto moves = new vector<Move>();
	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	EXPECT_EQ(MoveHelper::toString(best.first), "e2e3");
}

TEST_F(BFEngineTest, Depth5Test) {
#ifndef VTUNE_PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	auto engine = new BF_Engine(5);
	engine->setEvaluator(new BasicEvaluator());
	auto moves = new vector<Move>();

	auto begin = chrono::steady_clock::now();

	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	cout << "time: " << Utils::getElapsedMillis(begin) << endl;
	// 8676

	EXPECT_EQ(MoveHelper::toString(best.first), "e2e4");
}

