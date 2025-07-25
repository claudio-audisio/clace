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
		gameInitial->board.getQueenAttacks(WHITE);
		gameInitial->board.getQueenAttacks(BLACK);
		gamePerft2->board.getQueenAttacks(WHITE);
		gamePerft2->board.getQueenAttacks(BLACK);
		gamePerft3->board.getQueenAttacks(WHITE);
		gamePerft3->board.getQueenAttacks(BLACK);
		gamePerft4->board.getQueenAttacks(WHITE);
		gamePerft4->board.getQueenAttacks(BLACK);
		gamePerft5->board.getQueenAttacks(WHITE);
		gamePerft5->board.getQueenAttacks(BLACK);
		gamePerft6->board.getQueenAttacks(WHITE);
		gamePerft6->board.getQueenAttacks(BLACK);
	}

	unsLL time = Utils::getElapsedMillis(begin);

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
		gameInitial->board.getKnightAttacks(WHITE);
		gameInitial->board.getKnightAttacks(BLACK);
		gamePerft2->board.getKnightAttacks(WHITE);
		gamePerft2->board.getKnightAttacks(BLACK);
		gamePerft3->board.getKnightAttacks(WHITE);
		gamePerft3->board.getKnightAttacks(BLACK);
		gamePerft4->board.getKnightAttacks(WHITE);
		gamePerft4->board.getKnightAttacks(BLACK);
		gamePerft5->board.getKnightAttacks(WHITE);
		gamePerft5->board.getKnightAttacks(BLACK);
		gamePerft6->board.getKnightAttacks(WHITE);
		gamePerft6->board.getKnightAttacks(BLACK);
	}

	unsLL time = Utils::getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1190, 50);	// BOARD_STANDARD_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1200, 50);	// BOARD_BRANCHLESS_RAY_ATTACKS
	//GTEST_ASSERT_NEAR(time, 1200, 50);	// BOARD_ONTHEFLY_RAY_ATTACKS	// TODO

	cout << "time: " << time  << endl;
}



TEST_F(PerformanceDebugTest, stuffTest) {
//#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
//#endif
	Position source = 4;
	Position destination = 2;

	Move move = MoveHelper::getMove(source, destination, WHITE, BKing);

	Game* game = FEN::fenToNewGame(CASTLING_FEN_POSITION);
	Board board = game->getCopyBoard();

	game->simulateMove(move);
	game->undoSimulateMove(move);

	/*printBoard(game->board.BOARD(WHITE));
	printBoard(game->board.PIECES(BLACK));
	printBoard(game->board.EMPTY);*/

	GTEST_ASSERT_TRUE(game->board.equals(board));
}


#endif
