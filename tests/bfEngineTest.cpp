#include <gtest/gtest.h>

#include "../engine/bruteForceEngine.h"
#include "../ui/userInterface.h"
#include "../common/defines.h"

using namespace std;


class BFEngineTest : public testing::Test {
protected:
	unsigned int DEPTH;
	BruteForceEngine *engine;

	BFEngineTest() {
		DEPTH = 4;
		initMovesGenerator();
	}
	~BFEngineTest() {
		delete engine;
	}
};

#ifdef PROFILER

TEST_F(BFEngineTest, Depth5Test) {
	Game game;
	game.init();
	engine = new BruteForceEngine(DEPTH + 1);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2e4");
}

#elifndef PROFILER

TEST_F(BFEngineTest, ConstructorTest) {
	engine = new BruteForceEngine(DEPTH);

	EXPECT_EQ(engine->depth, DEPTH);
}

TEST_F(BFEngineTest, enPassantBugTest) {
	Game game;
	game.initFromFEN(FEN_EN_PASSANT_BUG_TEST);
	engine = new BruteForceEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_NE(moveToString(best.move), "c4b3");
}

TEST_F(BFEngineTest, mateInOneTest) {
	Game game;
	game.initFromFEN(FEN_MATE_IN_ONE);
	engine = new BruteForceEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "b1h1");
}

TEST_F(BFEngineTest, mateInOneBlackTest) {
	Game game;
	game.initFromFEN(FEN_MATE_IN_ONE_BLACK);
	engine = new BruteForceEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "b1h1");
}

TEST_F(BFEngineTest, mateInTwoTest) {
	Game game;
	game.initFromFEN(FEN_MATE_IN_TWO);
	engine = new BruteForceEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e7g7");
}

TEST_F(BFEngineTest, mateInThreeTest) {
	Game game;
	game.initFromFEN(FEN_MATE_IN_THREE);
	engine = new BruteForceEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "c3c2");
}

TEST_F(BFEngineTest, Depth4Test) {
	Game game;
	game.init();
	engine = new BruteForceEngine(DEPTH);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2e3");
}

TEST_F(BFEngineTest, Depth3TestOnFenPosition2) {
	Game game;
	game.initFromFEN(PERFT_FEN_POSITION_2);
	engine = new BruteForceEngine(DEPTH - 1);
	Evaluation best = engine->calculateMove(game);

	EXPECT_EQ(moveToString(best.move), "e2a6");
}

TEST_F(BFEngineTest, dailyBugTest) {
	GTEST_SKIP();
	Game game;
	game.initFromFEN(FEN_MATE_IN_THREE);
	engine = new BruteForceEngine(4);
	Evaluation best = engine->calculateMove(game);

	// TODO
	// A depth 4 mi da c3c2 matto a depth 3 e c3f3 a 17.80 (tutto corretto)
	// A depth 5 mi da c3c2 matto a depth 5 (scorretto dovvrebe essere 3 come prima)
	// e c3f3 matto a depth 3 (vero pero' il bianco non fara' mai quella mossa, dovrebbe essere depth 5)

	cout << moveToString(best.move) << " --> " << evalValueToString(best) << endl;
}

#endif
