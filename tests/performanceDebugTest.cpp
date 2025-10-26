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
		initDestPosProviders();
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
		s_allQueenAttacks(gameInitial->board, _WHITE);
		s_allQueenAttacks(gameInitial->board, _BLACK);
		s_allQueenAttacks(gamePerft2->board, _WHITE);
		s_allQueenAttacks(gamePerft2->board, _BLACK);
		s_allQueenAttacks(gamePerft3->board, _WHITE);
		s_allQueenAttacks(gamePerft3->board, _BLACK);
		s_allQueenAttacks(gamePerft4->board, _WHITE);
		s_allQueenAttacks(gamePerft4->board, _BLACK);
		s_allQueenAttacks(gamePerft5->board, _WHITE);
		s_allQueenAttacks(gamePerft5->board, _BLACK);
		s_allQueenAttacks(gamePerft6->board, _WHITE);
		s_allQueenAttacks(gamePerft6->board, _BLACK);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1560, 50);	// BOARD_STANDARD_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1580, 50);	// BOARD_BRANCHLESS_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1560, 50);	// BOARD_ONTHEFLY_RAY_ATTACKS

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
		s_allKnightAttacks(gameInitial->board, _WHITE);
		s_allKnightAttacks(gameInitial->board, _BLACK);
		s_allKnightAttacks(gamePerft2->board, _WHITE);
		s_allKnightAttacks(gamePerft2->board, _BLACK);
		s_allKnightAttacks(gamePerft3->board, _WHITE);
		s_allKnightAttacks(gamePerft3->board, _BLACK);
		s_allKnightAttacks(gamePerft4->board, _WHITE);
		s_allKnightAttacks(gamePerft4->board, _BLACK);
		s_allKnightAttacks(gamePerft5->board, _WHITE);
		s_allKnightAttacks(gamePerft5->board, _BLACK);
		s_allKnightAttacks(gamePerft6->board, _WHITE);
		s_allKnightAttacks(gamePerft6->board, _BLACK);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1220, 50);	// BOARD_STANDARD_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1200, 50);	// BOARD_BRANCHLESS_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1200, 50);	// BOARD_ONTHEFLY_RAY_ATTACKS

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceDebugTest, getAttacksPerformanceTest) {
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

	for (int i = 1; i < 500000; ++i) {
		s_allAttacks(gameInitial->board, _WHITE);
		s_allAttacks(gameInitial->board, _BLACK);
		s_allAttacks(gamePerft2->board, _WHITE);
		s_allAttacks(gamePerft2->board, _BLACK);
		s_allAttacks(gamePerft3->board, _WHITE);
		s_allAttacks(gamePerft3->board, _BLACK);
		s_allAttacks(gamePerft4->board, _WHITE);
		s_allAttacks(gamePerft4->board, _BLACK);
		s_allAttacks(gamePerft5->board, _WHITE);
		s_allAttacks(gamePerft5->board, _BLACK);
		s_allAttacks(gamePerft6->board, _WHITE);
		s_allAttacks(gamePerft6->board, _BLACK);
	}

	unsLL time = getElapsedMillis(begin);

#ifdef BOARD_STANDARD_RAY_ATTACKS
#ifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(time, 2170, 50);
#else
	GTEST_ASSERT_NEAR(time, 1150, 50);
#endif
#endif

#ifdef BOARD_BRANCHLESS_RAY_ATTACKS
#ifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(time, 2160, 50);
#else
	GTEST_ASSERT_NEAR(time, 1300, 50);
#endif
#endif

#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
#ifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(time, 2085, 50);
#else
	GTEST_ASSERT_NEAR(time, 1345, 50);
#endif
#endif

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
