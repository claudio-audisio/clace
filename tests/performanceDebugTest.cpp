#include <gtest/gtest.h>
#include <chrono>

#include "../board/board.h"
#include "../utils/fen.h"
#include "../utils/boardUtils.h"
#include "../utils/positions.h"
#include "../utils/utils.h"
#include "../move/movesGenerator.h"
#include "../move/move.h"
#include "../perft/perft.h"
#include "../utils/vectorPool.h"
#include "../engine/bf_engine.h"
#include "../common/defines.h"

using namespace std;

#if DEBUG_MODE


class PerformanceDebugTest : public testing::Test {
protected:
	PerformanceDebugTest() {
		initAttacks();
	}
	~PerformanceDebugTest() {

	}
	void static GTEST_ASSERT_NEAR(unsLL value, unsLL expected, unsLL delta) {
		GTEST_ASSERT_LT(value, expected + delta);
		GTEST_ASSERT_GT(value, expected - delta);
	}
};


TEST_F(PerformanceDebugTest, getQueenAttacksPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game* gameInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* gamePerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* gamePerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* gamePerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* gamePerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* gamePerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (long i = 1; i < 2500000; ++i) {
		gameInitial->board.getQueenAttacks(_WHITE);
		gameInitial->board.getQueenAttacks(_BLACK);
		gamePerft2->board.getQueenAttacks(_WHITE);
		gamePerft2->board.getQueenAttacks(_BLACK);
		gamePerft3->board.getQueenAttacks(_WHITE);
		gamePerft3->board.getQueenAttacks(_BLACK);
		gamePerft4->board.getQueenAttacks(_WHITE);
		gamePerft4->board.getQueenAttacks(_BLACK);
		gamePerft5->board.getQueenAttacks(_WHITE);
		gamePerft5->board.getQueenAttacks(_BLACK);
		gamePerft6->board.getQueenAttacks(_WHITE);
		gamePerft6->board.getQueenAttacks(_BLACK);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1560, 50);	// BOARD_STANDARD_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1580, 50);	// BOARD_BRANCHLESS_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1560, 50);	// BOARD_ONTHEFLY_RAY_ATTACKS	// TODO

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceDebugTest, getKnightAttacksPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game* gameInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* gamePerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* gamePerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* gamePerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* gamePerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* gamePerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (long i = 1; i < 10000000; ++i) {
		gameInitial->board.getKnightAttacks(_WHITE);
		gameInitial->board.getKnightAttacks(_BLACK);
		gamePerft2->board.getKnightAttacks(_WHITE);
		gamePerft2->board.getKnightAttacks(_BLACK);
		gamePerft3->board.getKnightAttacks(_WHITE);
		gamePerft3->board.getKnightAttacks(_BLACK);
		gamePerft4->board.getKnightAttacks(_WHITE);
		gamePerft4->board.getKnightAttacks(_BLACK);
		gamePerft5->board.getKnightAttacks(_WHITE);
		gamePerft5->board.getKnightAttacks(_BLACK);
		gamePerft6->board.getKnightAttacks(_WHITE);
		gamePerft6->board.getKnightAttacks(_BLACK);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1190, 50);	// BOARD_STANDARD_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1200, 50);	// BOARD_BRANCHLESS_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1200, 50);	// BOARD_ONTHEFLY_RAY_ATTACKS	// TODO

	cout << "time: " << time  << endl;
}



TEST_F(PerformanceDebugTest, bugOfTheDayTest) {
	//GTEST_SKIP();
	Game game;
	game.initFromFEN("1nb1kbnQ/pppp1pp1/4p3/8/4P3/8/PPPP1PPP/RNB1KBNR b KQ - 0 1");
	auto engine = new BF_Engine(3);
	Evaluation best = engine->calculateMove(game);
	cout << "best: " << moveToString(best.move) << " --> " << best.value << endl;
}


#endif
