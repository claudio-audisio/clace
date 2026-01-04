#include <gtest/gtest.h>
#include <chrono>

#include "../board/board.h"
#include "../cache/zobrist.h"
#include "../utils/fen.h"
#include "../utils/boardUtils.h"
#include "../utils/positions.h"
#include "../utils/utils.h"
#include "../move/movesGenerator.h"
#include "../move/move.h"
#include "../perft/perft.h"
#include "../engine/bf_engine.h"
#include "../common/defines.h"
#include "../move/rollback.h"
#include "../movesCalculation/staticAttacks.h"

using namespace std;

#if RELEASE_MODE


class PerformanceReleaseTest : public testing::Test {
protected:
	PerformanceReleaseTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}

	~PerformanceReleaseTest() override {

	}

	void static GTEST_ASSERT_NEAR(unsLL value, unsLL expected) {
		unsLL delta = expected * 5 / 100;

		if (delta < 10) {
			delta = 10;
		}

		GTEST_ASSERT_LE(value, expected + delta);
		GTEST_ASSERT_GE(value, expected - delta);
	}
};

TEST_F(PerformanceReleaseTest, generatePseudoLegalMovesPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Move moves[MAX_MOVES];
	MovesAmount movesAmount;
	Game* boardInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* boardPerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* boardPerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* boardPerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* boardPerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* boardPerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 1000000; ++i) {
		generatePseudoLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		generatePseudoLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		generatePseudoLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		generatePseudoLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		generatePseudoLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		generatePseudoLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		generatePseudoLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		generatePseudoLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		generatePseudoLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		generatePseudoLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		generatePseudoLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
		generatePseudoLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
	}

	unsLL time = getElapsedMillis(begin);
	GTEST_ASSERT_NEAR(time, 1820);
	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, simulatePerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Move moves[MAX_MOVES];
	MovesAmount movesAmount;
	Game* game = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	auto tot = generatePseudoLegalMoves(*game, moves);

	auto begin = chrono::steady_clock::now();

	for (int j = 1; j < 1000000; ++j) {
		for (int i = 0; i < tot; i++) {
			game->simulateMove(moves[i]);
			game->checkControl(moves[i]);
			game->undoSimulateMove(moves[i]);
		}
	}

	unsLL time = getElapsedMillis(begin);
	GTEST_ASSERT_NEAR(time, 1450);
	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, generateLegalMovesPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Move moves[MAX_MOVES];
	MovesAmount movesAmount;
	Game* boardInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* boardPerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* boardPerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* boardPerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* boardPerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* boardPerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 100000; ++i) {
		generateLegalMoves(*boardInitial, moves, &movesAmount);
		boardInitial->changeTurn();
		generateLegalMoves(*boardInitial, moves, &movesAmount);
		boardInitial->changeTurn();
		generateLegalMoves(*boardPerft2, moves, &movesAmount);
		boardPerft2->changeTurn();
		generateLegalMoves(*boardPerft2, moves, &movesAmount);
		boardPerft2->changeTurn();
		generateLegalMoves(*boardPerft3, moves, &movesAmount);
		boardPerft3->changeTurn();
		generateLegalMoves(*boardPerft3, moves, &movesAmount);
		boardPerft3->changeTurn();
		generateLegalMoves(*boardPerft4, moves, &movesAmount);
		boardPerft4->changeTurn();
		generateLegalMoves(*boardPerft4, moves, &movesAmount);
		boardPerft4->changeTurn();
		generateLegalMoves(*boardPerft5, moves, &movesAmount);
		boardPerft5->changeTurn();
		generateLegalMoves(*boardPerft5, moves, &movesAmount);
		boardPerft5->changeTurn();
		generateLegalMoves(*boardPerft6, moves, &movesAmount);
		boardPerft6->changeTurn();
		generateLegalMoves(*boardPerft6, moves, &movesAmount);
		boardPerft6->changeTurn();
	}

	unsLL time = getElapsedMillis(begin);
	GTEST_ASSERT_NEAR(time, 1270);
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
		game1->calculateCheckPositions(_WHITE, 4);
		game1->calculateCheckPositions(_BLACK, 60);
		game2->calculateCheckPositions(_WHITE, 4);
		game2->calculateCheckPositions(_BLACK, 60);
		game3->calculateCheckPositions(_WHITE, 39);
		game3->calculateCheckPositions(_BLACK, 24);
		game4->calculateCheckPositions(_WHITE, 4);
		game4->calculateCheckPositions(_BLACK, 62);
		game4m->calculateCheckPositions(_WHITE, 6);
		game4m->calculateCheckPositions(_BLACK, 60);
		game5->calculateCheckPositions(_WHITE, 5);
		game5->calculateCheckPositions(_BLACK, 60);
		game6->calculateCheckPositions(_WHITE, 6);
		game6->calculateCheckPositions(_BLACK, 62);
	}

	unsLL time = getElapsedMillis(start);
	GTEST_ASSERT_NEAR(time, 1080);
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

	MovesAmount mockAmount;
	MovesAmount amountPerft3;
	generateLegalMoves(*gameInitial, initialMoves, &mockAmount);
	generateLegalMoves(*gamePerft2, perft2Moves, &mockAmount);
	generateLegalMoves(*gamePerft3, perft3Moves, &amountPerft3);
	generateLegalMoves(*gamePerft5, perft5Moves, &mockAmount);
	generateLegalMoves(*gamePerft6, perft6Moves, &mockAmount);

	unsLL time = 0;

	for (int count = 0; count < 50000; count++) {
		for (int index = 0; index < amountPerft3.total; index++) {
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
	GTEST_ASSERT_NEAR(time, 980);
	cout << "time: " << time << endl;
}

TEST_F(PerformanceReleaseTest, castlingPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif

	Game* game = FEN::fenToNewGame("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	Move BQCastlingMove = createMove(60, 58, _BLACK, BKing);
	Move BKCastlingMove = createMove(60, 62, _BLACK, BKing);
	Move WQCastlingMove = createMove(4, 2, _WHITE, WKing);
	Move WKCastlingMove = createMove(4, 6, _WHITE, WKing);

	auto begin = chrono::steady_clock::now();

	for (int count = 0; count < 50000000; count++) {
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
	GTEST_ASSERT_NEAR(time, 1150);
	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, castlingMaskPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif

	Game* game = FEN::fenToNewGame(CASTLING_FEN_POSITION);
	auto moves = new Move[MAX_MOVES];
	MovesAmount amount;
	generateLegalMoves(*game, moves, &amount);

	auto begin = chrono::steady_clock::now();

	for (long count = 0; count < 100000000; count++) {
		for (int m = 0; m < amount.total; m++) {
			if (moves[m]) {
				game->board->castlingInfo = 0b1111;
				updateCastlingInfo(game->board, getSourcePosition(moves[m]), getDestinationPosition(moves[m]));
			}
		}
	}

	unsLL time = getElapsedMillis(begin);
	GTEST_ASSERT_NEAR(time, 1860);
	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, saveSnapshotTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game* game1 = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	game1->verifyChecks();
	Game* game2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	game2->verifyChecks();
	Game* game3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	game3->verifyChecks();
	Game* game4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	game4->verifyChecks();
	Game* game5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	game5->verifyChecks();
	Game* game6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);
	game6->verifyChecks();

	const unsigned int size = 5000000;
	const unsigned int testSize = size * 6;
	GameSnapshot **snapshots = allocateSnapshots(testSize);

	auto start = chrono::steady_clock::now();
	for (int i = 0; i < testSize;) {
		saveSnapshot(game1->board, game1->sideToMove, game1->fullMoves, game1->halfMoveClock, snapshots, i++);
		saveSnapshot(game2->board, game2->sideToMove, game2->fullMoves, game2->halfMoveClock, snapshots, i++);
		saveSnapshot(game3->board, game3->sideToMove, game3->fullMoves, game3->halfMoveClock, snapshots, i++);
		saveSnapshot(game4->board, game4->sideToMove, game4->fullMoves, game4->halfMoveClock, snapshots, i++);
		saveSnapshot(game5->board, game5->sideToMove, game5->fullMoves, game5->halfMoveClock, snapshots, i++);
		saveSnapshot(game6->board, game6->sideToMove, game6->fullMoves, game6->halfMoveClock, snapshots, i++);
	}

	auto saveTime = getElapsedMillis(start);
	GTEST_ASSERT_NEAR(saveTime, 1090);
	cout << "time: " << saveTime  << endl;

	deallocateSnapshots(snapshots, testSize);
}

TEST_F(PerformanceReleaseTest, loadSnapshotTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game* game1 = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	game1->verifyChecks();
	Game* game2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	game2->verifyChecks();
	Game* game3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	game3->verifyChecks();
	Game* game4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	game4->verifyChecks();
	Game* game5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	game5->verifyChecks();
	Game* game6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);
	game6->verifyChecks();

	const unsigned int size = 5000000;
	const unsigned int testSize = size * 6;
	GameSnapshot **snapshots = allocateSnapshots(testSize);

	for (int i = 0; i < testSize;) {
		saveSnapshot(game1->board, game1->sideToMove, game1->fullMoves, game1->halfMoveClock, snapshots, i++);
		saveSnapshot(game2->board, game2->sideToMove, game2->fullMoves, game2->halfMoveClock, snapshots, i++);
		saveSnapshot(game3->board, game3->sideToMove, game3->fullMoves, game3->halfMoveClock, snapshots, i++);
		saveSnapshot(game4->board, game4->sideToMove, game4->fullMoves, game4->halfMoveClock, snapshots, i++);
		saveSnapshot(game5->board, game5->sideToMove, game5->fullMoves, game5->halfMoveClock, snapshots, i++);
		saveSnapshot(game6->board, game6->sideToMove, game6->fullMoves, game6->halfMoveClock, snapshots, i++);
	}

	auto start = chrono::steady_clock::now();
	for (int i = testSize - 1; i >= 0;) {
		loadSnapshot(game1->board, game1->sideToMove, game1->fullMoves, game1->halfMoveClock, snapshots, i--);
		loadSnapshot(game2->board, game2->sideToMove, game2->fullMoves, game2->halfMoveClock, snapshots, i--);
		loadSnapshot(game3->board, game3->sideToMove, game3->fullMoves, game3->halfMoveClock, snapshots, i--);
		loadSnapshot(game4->board, game4->sideToMove, game4->fullMoves, game4->halfMoveClock, snapshots, i--);
		loadSnapshot(game5->board, game5->sideToMove, game5->fullMoves, game5->halfMoveClock, snapshots, i--);
		loadSnapshot(game6->board, game6->sideToMove, game6->fullMoves, game6->halfMoveClock, snapshots, i--);
	}

	auto rollbackTime = getElapsedMillis(start);
	GTEST_ASSERT_NEAR(rollbackTime, 1450);
	cout << "time: " << rollbackTime  << endl;

	deallocateSnapshots(snapshots, testSize);
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
	GTEST_ASSERT_NEAR(time, 1830);
	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, movesCacheTest) {
	// TODO
	GTEST_SKIP() << "First iteration takes double time compared with the next ones";
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
	GTEST_ASSERT_NEAR(time, 580);
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
#else
	GTEST_ASSERT_NEAR(result->getElapsed(), 180);
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
#else
	GTEST_ASSERT_NEAR(result->getElapsed(), 4400);
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
#else
	GTEST_ASSERT_NEAR(result->getElapsed(), 220);
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
#else
	GTEST_ASSERT_NEAR(result->getElapsed(), 5400);
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
	MovesAmount amount;

	auto begin = chrono::steady_clock::now();

	generateLegalMoves(game, moves, &amount);
	engine->_calculateMove(game, moves, amount);

	unsLL time = getElapsedMillis(begin);
	GTEST_ASSERT_NEAR(time, 280);
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
	MovesAmount amount;

	auto begin = chrono::steady_clock::now();

	generateLegalMoves(game, moves, &amount);
	engine->_calculateMove(game, moves, amount);

	unsLL time = getElapsedMillis(begin);
	GTEST_ASSERT_NEAR(time, 180);
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
	MovesAmount amount;

	auto begin = chrono::steady_clock::now();

	generateLegalMoves(game, moves, &amount);
	engine->_calculateMove(game, moves, amount);

	unsLL time = getElapsedMillis(begin);
	GTEST_ASSERT_NEAR(time, 570);
	cout << "time: " << time  << endl;
}


#endif
