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
#include "../engine/bf_engine.h"
#include "../common/defines.h"

using namespace std;

#if RELEASE_MODE


class PerformanceReleaseTest : public testing::Test {
protected:
	PerformanceReleaseTest() {
		initAttacks();
	}
	~PerformanceReleaseTest() {

	}
	void static GTEST_ASSERT_NEAR(unsLL value, unsLL expected, unsLL delta) {
		GTEST_ASSERT_LE(value, expected + delta);
		GTEST_ASSERT_GE(value, expected - delta);
	}
};

TEST_F(PerformanceReleaseTest, calculateLegalMovesPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Move moves[MAX_MOVES];
	Game* boardInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* boardPerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* boardPerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* boardPerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* boardPerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* boardPerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 100000; ++i) {
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
		memset(moves, 0, sizeof(moves));
		generateLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
	}

	unsLL time = getElapsedMillis(begin);

#ifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(time, 2040, 50);	// TODO
#else
	GTEST_ASSERT_NEAR(time, 1730, 50);
#endif

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, calculateCheckPositionsPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game* game1 = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* game2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* game3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* game4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* game4m = FEN::fenToNewGame(PERFT_FEN_POSITION_4_MIRRORED);
	Game* game5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* game6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto start = chrono::steady_clock::now();

	for (int i = 1; i < 500000; ++i) {
		game1->calculateCheckPositions(WHITE);
		game1->calculateCheckPositions(BLACK);
		game2->calculateCheckPositions(WHITE);
		game2->calculateCheckPositions(BLACK);
		game3->calculateCheckPositions(WHITE);
		game3->calculateCheckPositions(BLACK);
		game4->calculateCheckPositions(WHITE);
		game4->calculateCheckPositions(BLACK);
		game4m->calculateCheckPositions(WHITE);
		game4m->calculateCheckPositions(BLACK);
		game5->calculateCheckPositions(WHITE);
		game5->calculateCheckPositions(BLACK);
		game6->calculateCheckPositions(WHITE);
		game6->calculateCheckPositions(BLACK);
	}

	unsLL time = getElapsedMillis(start);

#ifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(time, 1100, 50);
#else
	GTEST_ASSERT_NEAR(time, 1130, 50);
#endif

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, getAttacksPerformanceTest) {
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

	for (int i = 1; i < 50000000; ++i) {
		gameInitial->board.getAttacks(WHITE);
		gameInitial->board.getAttacks(BLACK);
		gamePerft2->board.getAttacks(WHITE);
		gamePerft2->board.getAttacks(BLACK);
		gamePerft3->board.getAttacks(WHITE);
		gamePerft3->board.getAttacks(BLACK);
		gamePerft4->board.getAttacks(WHITE);
		gamePerft4->board.getAttacks(BLACK);
		gamePerft5->board.getAttacks(WHITE);
		gamePerft5->board.getAttacks(BLACK);
		gamePerft6->board.getAttacks(WHITE);
		gamePerft6->board.getAttacks(BLACK);
	}

	unsLL time = getElapsedMillis(begin);

#ifdef BOARD_STANDARD_RAY_ATTACKS
	#ifdef BOARD_USE_PRE_CALCULATED
		GTEST_ASSERT_NEAR(time, 2170, 50);
	#else
		GTEST_ASSERT_NEAR(time, 1280, 50);
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

TEST_F(PerformanceReleaseTest, finalizeMovePerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif

	Game* gameInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* gamePerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* gamePerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* gamePerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* gamePerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	Move initialMoves[MAX_MOVES];
	Move perft2Moves[MAX_MOVES];
	Move perft3Moves[MAX_MOVES];
	Move perft5Moves[MAX_MOVES];
	Move perft6Moves[MAX_MOVES];
	generateLegalMoves(*gameInitial, initialMoves);
	generateLegalMoves(*gamePerft2, perft2Moves);
	MovesAmount amountPerft3 = generateLegalMoves(*gamePerft3, perft3Moves);
	generateLegalMoves(*gamePerft5, perft5Moves);
	generateLegalMoves(*gamePerft6, perft6Moves);

	unsLL time = 0;

	for (int count = 0; count < 50000; count++) {
		for (int index = 0; index < amountPerft3.first; index++) {
			auto begin = chrono::steady_clock::now();

			if (initialMoves[index]) { gameInitial->finalizeMove(initialMoves[index]); }
			if (perft2Moves[index]) { gamePerft2->finalizeMove(perft2Moves[index]); }
			if (perft3Moves[index]) { gamePerft3->finalizeMove(perft3Moves[index]); }
			if (perft5Moves[index]) { gamePerft5->finalizeMove(perft5Moves[index]); }
			if (perft6Moves[index]) { gamePerft6->finalizeMove(perft6Moves[index]); }

			time += getElapsedNanos(begin);

			FEN::fenToGame(INITIAL_FEN_POSITION, *gameInitial);
			FEN::fenToGame(PERFT_FEN_POSITION_2, *gamePerft2);
			FEN::fenToGame(PERFT_FEN_POSITION_3, *gamePerft3);
			FEN::fenToGame(PERFT_FEN_POSITION_5, *gamePerft5);
			FEN::fenToGame(PERFT_FEN_POSITION_6, *gamePerft6);
		}
	}

	time /= 100000;

	GTEST_ASSERT_NEAR(time, 920, 20);

	cout << "time: " << time << endl;
}

TEST_F(PerformanceReleaseTest, castlingPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif

	Game* game = FEN::fenToNewGame("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	Move BQCastlingMove = createMove(4, 2, BLACK, BKing);
	Move BKCastlingMove = createMove(4, 6, BLACK, BKing);
	Move WQCastlingMove = createMove(60, 58, WHITE, WKing);
	Move WKCastlingMove = createMove(60, 62, WHITE, WKing);

	auto begin = chrono::steady_clock::now();

	for (int count = 0; count < 100000000; count++) {
		game->simulateMove(BQCastlingMove);
		game->undoSimulateMove(BQCastlingMove);
		game->simulateMove(BKCastlingMove);
		game->undoSimulateMove(BKCastlingMove);
		game->simulateMove(WQCastlingMove);
		game->undoSimulateMove(WQCastlingMove);
		game->simulateMove(WKCastlingMove);
		game->undoSimulateMove(WKCastlingMove);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1980, 50);

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, castlingMaskPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif

	Game* game = FEN::fenToNewGame(CASTLING_FEN_POSITION);
	auto moves = new Move[MAX_MOVES];
	pair<int, int> res = generateLegalMoves(*game, moves);

	auto begin = chrono::steady_clock::now();

	for (long count = 0; count < 100000000; count++) {
		for (int m = 0; m < res.first; m++) {
			if (moves[m]) {
				game->board.castlingInfo = 0b1111;
				game->board.updateCastlingInfo(getSourcePosition(moves[m]), getDestinationPosition(moves[m]));
			}
		}
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1730, 50);

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, rollbackTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game* game = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	game->verifyChecks();
	const unsigned int size = 100000000;
	Rollback* rollback = new Rollback(size);

	auto start = chrono::steady_clock::now();
	for (int i = 0; i < size; ++i) {
		rollback->save(*game);
	}

	auto saveTime = getElapsedMillis(start);
	GTEST_ASSERT_NEAR(saveTime, 1750, 50);
	cout << "save time: " << saveTime  << endl;

	start = chrono::steady_clock::now();
	for (int i = 0; i < size; ++i) {
		rollback->rollback(*game);
	}

	auto rollbackTime = getElapsedMillis(start);
	GTEST_ASSERT_NEAR(rollbackTime, 1500, 50);
	cout << "rollback time: " << rollbackTime  << endl;
}

TEST_F(PerformanceReleaseTest, gameToFENKeyTest) {
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

	for (long i = 1; i < 1000000; ++i) {
		FEN::gameToFENKey(*gameInitial);
		FEN::gameToFENKey(*gamePerft2);
		FEN::gameToFENKey(*gamePerft3);
		FEN::gameToFENKey(*gamePerft4);
		FEN::gameToFENKey(*gamePerft5);
		FEN::gameToFENKey(*gamePerft6);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1650, 50);	// TODO imbarazzante

	cout << "time: " << time  << endl;
}

// TODO questo test e il prossimo alla prima iterazione il tempo e' doppio, capire il perche'
// (non e' ottimizzazione release, forse ottimizzazione cpu ?)
TEST_F(PerformanceReleaseTest, movesCacheNewTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Move moves[MAX_MOVES];
	Move* moves2;
	MovesAmount amount;

	for (int i = 0; i < MAX_MOVES; ++i) {
		moves[i] = i;
	}

	const unsigned int testSize = 1000000;
	MovesCache cache(testSize);

	auto begin = chrono::steady_clock::now();

	for (int m = 0; m < testSize; ++m) {
		const string key = to_string(m);
		cache.add(key, moves, MAX_MOVES, MAX_MOVES - 1);
		cache.get(key, moves2, amount);
	}

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1100, 50);	// since second iteration 480

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, Perft5BulkTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	auto perft = new Perft(INITIAL_FEN_POSITION, 5);

	auto result = perft->runBulk();

#ifdef PERFT_USE_CACHE
	GTEST_ASSERT_NEAR(result->getElapsed(), 265, 10);
#elifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(result->getElapsed(), 240, 10);
#else
	GTEST_ASSERT_NEAR(result->getElapsed(), 225, 10);
#endif

	cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformanceReleaseTest, Perft6BulkTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	auto perft = new Perft(INITIAL_FEN_POSITION, 6);

	auto result = perft->runBulk();

#ifdef PERFT_USE_CACHE
	GTEST_ASSERT_NEAR(result->getElapsed(), 5450, 50);
#elifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(result->getElapsed(), 6110, 50);
#else
	GTEST_ASSERT_NEAR(result->getElapsed(), 5980, 50);
#endif

	cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformanceReleaseTest, Perft4CompleteTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	auto perft = new Perft(INITIAL_FEN_POSITION, 4);

	auto result = perft->runComplete();

#ifdef PERFT_USE_CACHE
	GTEST_ASSERT_NEAR(result->getElapsed(), 320, 10);
#elifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(result->getElapsed(), 6110, 50);		// TODO
#else
	GTEST_ASSERT_NEAR(result->getElapsed(), 280, 10);
#endif

	cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformanceReleaseTest, Perft5CompleteTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	auto perft = new Perft(INITIAL_FEN_POSITION, 5);

	auto result = perft->runComplete();

#ifdef PERFT_USE_CACHE
	GTEST_ASSERT_NEAR(result->getElapsed(), 6610, 50);
#elifdef BOARD_USE_PRE_CALCULATED
	GTEST_ASSERT_NEAR(result->getElapsed(), 6110, 50);		// TODO
#else
	GTEST_ASSERT_NEAR(result->getElapsed(), 7300, 50);
#endif

	cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformanceReleaseTest, BFEngineOpenGameDepth4Test) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(INITIAL_FEN_POSITION);
	auto engine = new BF_Engine(4);
	engine->setEvaluator(new BasicEvaluator());
	auto moves = new Move[MAX_MOVES];

	auto begin = chrono::steady_clock::now();

	MovesAmount amount = generateLegalMoves(game, moves);
	engine->_calculateMove(game, moves, amount);

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 340, 10);

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, BFEngineMidGameDepth3Test) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(PERFT_FEN_POSITION_6);
	auto engine = new BF_Engine(3);
	engine->setEvaluator(new BasicEvaluator());
	auto moves = new Move[MAX_MOVES];

	auto begin = chrono::steady_clock::now();

	MovesAmount amount = generateLegalMoves(game, moves);
	engine->_calculateMove(game, moves, amount);

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 240, 10);

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, BFEngineEndGameDepth5Test) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(PERFT_FEN_POSITION_3);
	auto engine = new BF_Engine(5);
	auto moves = new Move[MAX_MOVES];

	auto begin = chrono::steady_clock::now();

	MovesAmount amount = generateLegalMoves(game, moves);
	engine->_calculateMove(game, moves, amount);

	unsLL time = getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 590, 10);

	cout << "time: " << time  << endl;
}


#endif
