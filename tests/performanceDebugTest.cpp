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
		initPawnAttacksProviders();
	}
	~PerformanceDebugTest() {

	}
	void static GTEST_ASSERT_NEAR(unsLL value, unsLL expected, unsLL delta) {
		GTEST_ASSERT_LT(value, expected + delta);
		GTEST_ASSERT_GT(value, expected - delta);
	}
};

TEST_F(PerformanceDebugTest, movePerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Move move;
	Position pos = 14;
	Piece piece = WQueen;
	Side side = _WHITE;
	bool bParam = false;
	MoveType movetype;

	auto begin = chrono::steady_clock::now();

	for (long i = 0; i < 20000000; i++) {
		setSourcePosition(move, pos);
		pos = getSourcePosition(move);
		setDestinationPosition(move, pos);
		pos = getDestinationPosition(move);
		setPiece(move, piece);
		piece = getPiece(move);
		setPromotion(move, piece);
		piece = getPromotion(move);
		setCaptured(move, piece);
		piece = getCaptured(move);
		setMoveSide(move, side);
		side = getMoveSide(move);
		setCastling(move, true);
		setCastling(move, bParam);
		bParam = isCastling(move);
		setEnPassant(move, true);
		setEnPassant(move, bParam);
		bParam = isEnPassant(move);
		setPawnPromotion(move, true);
		setPawnPromotion(move, bParam);
		bParam = isPawnPromotion(move);
		movetype = getType(move);
	}


	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 950, 50);

	cout << "time: " << time  << endl;
}

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
		allQueenAttacks(gameInitial->board, _WHITE);
		allQueenAttacks(gameInitial->board, _BLACK);
		allQueenAttacks(gamePerft2->board, _WHITE);
		allQueenAttacks(gamePerft2->board, _BLACK);
		allQueenAttacks(gamePerft3->board, _WHITE);
		allQueenAttacks(gamePerft3->board, _BLACK);
		allQueenAttacks(gamePerft4->board, _WHITE);
		allQueenAttacks(gamePerft4->board, _BLACK);
		allQueenAttacks(gamePerft5->board, _WHITE);
		allQueenAttacks(gamePerft5->board, _BLACK);
		allQueenAttacks(gamePerft6->board, _WHITE);
		allQueenAttacks(gamePerft6->board, _BLACK);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1400, 50);	// BOARD_STANDARD_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1580, 50);	// BOARD_BRANCHLESS_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1560, 50);	// BOARD_ONTHEFLY_RAY_ATTACKS

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceDebugTest, getQueenAttacks2PerformanceTest) {
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
		allQueenAttacks(gameInitial->board, _WHITE, 4);
		allQueenAttacks(gameInitial->board, _BLACK, 60);
		allQueenAttacks(gamePerft2->board, _WHITE, 4);
		allQueenAttacks(gamePerft2->board, _BLACK, 60);
		allQueenAttacks(gamePerft3->board, _WHITE, 39);
		allQueenAttacks(gamePerft3->board, _BLACK, 24);
		allQueenAttacks(gamePerft4->board, _WHITE, 4);
		allQueenAttacks(gamePerft4->board, _BLACK, 62);
		allQueenAttacks(gamePerft5->board, _WHITE, 5);
		allQueenAttacks(gamePerft5->board, _BLACK, 60);
		allQueenAttacks(gamePerft6->board, _WHITE, 6);
		allQueenAttacks(gamePerft6->board, _BLACK, 62);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1400, 50);	// BOARD_STANDARD_RAY_ATTACKS
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
		allKnightAttacks(gameInitial->board, _WHITE);
		allKnightAttacks(gameInitial->board, _BLACK);
		allKnightAttacks(gamePerft2->board, _WHITE);
		allKnightAttacks(gamePerft2->board, _BLACK);
		allKnightAttacks(gamePerft3->board, _WHITE);
		allKnightAttacks(gamePerft3->board, _BLACK);
		allKnightAttacks(gamePerft4->board, _WHITE);
		allKnightAttacks(gamePerft4->board, _BLACK);
		allKnightAttacks(gamePerft5->board, _WHITE);
		allKnightAttacks(gamePerft5->board, _BLACK);
		allKnightAttacks(gamePerft6->board, _WHITE);
		allKnightAttacks(gamePerft6->board, _BLACK);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1100, 50);	// BOARD_STANDARD_RAY_ATTACKS
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
		allAttacks(gameInitial->board, _WHITE);
		allAttacks(gameInitial->board, _BLACK);
		allAttacks(gamePerft2->board, _WHITE);
		allAttacks(gamePerft2->board, _BLACK);
		allAttacks(gamePerft3->board, _WHITE);
		allAttacks(gamePerft3->board, _BLACK);
		allAttacks(gamePerft4->board, _WHITE);
		allAttacks(gamePerft4->board, _BLACK);
		allAttacks(gamePerft5->board, _WHITE);
		allAttacks(gamePerft5->board, _BLACK);
		allAttacks(gamePerft6->board, _WHITE);
		allAttacks(gamePerft6->board, _BLACK);
	}

	unsLL time = getElapsedMillis(begin);

#ifdef BOARD_STANDARD_RAY_ATTACKS
#ifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(time, 2170, 50);
#else
	GTEST_ASSERT_NEAR(time, 1090, 50);
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
