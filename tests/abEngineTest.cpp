#include <gtest/gtest.h>

#include "../engine/bruteForceEngine.h"
#include "../ui/userInterface.h"
#include "../common/defines.h"
#include "engine/alphaBetaEngine.h"

using namespace std;


class ABEngineTest : public testing::Test {
protected:
	AlphaBetaEngine *engine;
	unsigned int DEPTH;

	ABEngineTest() {
		DEPTH = 5;
		initMovesGenerator();
	}
	~ABEngineTest() {
		delete engine;
	}
};

TEST_F(ABEngineTest, ConstructorTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	engine = new AlphaBetaEngine(DEPTH);

	EXPECT_EQ(engine->depth, DEPTH);
}

TEST_F(ABEngineTest, enPassantBugTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(FEN_EN_PASSANT_BUG_TEST);
	engine = new AlphaBetaEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_NE(moveToString(best.move), "c4b3");
}

TEST_F(ABEngineTest, avoidStalemateTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(FEN_AVOID_STALEMATE);
	engine = new AlphaBetaEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "b6b5");
}

TEST_F(ABEngineTest, mateInOneTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(FEN_MATE_IN_ONE);
	engine = new AlphaBetaEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "b1h1");
}

TEST_F(ABEngineTest, mateInOneBlackTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(FEN_MATE_IN_ONE_BLACK);
	engine = new AlphaBetaEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "b1h1");
}

TEST_F(ABEngineTest, mateInTwoTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(FEN_MATE_IN_TWO);
	engine = new AlphaBetaEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e7g7");
}

TEST_F(ABEngineTest, mateInThreeTest) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(FEN_MATE_IN_THREE);
	engine = new AlphaBetaEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "c3c2");
}

TEST_F(ABEngineTest, Depth5Test) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	engine = new AlphaBetaEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2e3");
}

TEST_F(ABEngineTest, Depth4TestOnFenPosition2) {
#ifdef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(PERFT_FEN_POSITION_2);
	engine = new AlphaBetaEngine(DEPTH - 1);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2a6");
}

TEST_F(ABEngineTest, Depth6Test) {
#ifndef PROFILER
	GTEST_SKIP();
#endif
	Game game;
	game.init();
	engine = new AlphaBetaEngine(DEPTH + 1);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2e3");
}

TEST_F(ABEngineTest, dailyBugTest) {
	GTEST_SKIP();
	Game game;
	game.initFromFEN(FEN_MATE_IN_THREE);
	engine = new AlphaBetaEngine(5);
	Evaluation best = engine->calculateMove(game);

	// TODO
	// A depth 4 mi da c3c2 matto a depth 3 e c3f3 a 17.80 (tutto corretto)
	// A depth 5 mi da c3c2 matto a depth 5 (scorretto dovvrebe essere 3 come prima)
	// e c3f3 matto a depth 3 (vero pero' il bianco non fara' mai quella mossa, dovrebbe essere 5)

	cout << moveToString(best.move) << " --> " << evalValueToString(best) << endl;
}