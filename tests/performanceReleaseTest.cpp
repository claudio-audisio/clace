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
#include "../engine/bruteForceEngine.h"
#include "../common/defines.h"
#include "../move/rollback.h"
#include "../cache/transpositionTable.h"

using namespace std;

#if RELEASE_MODE


class PerformanceReleaseTest : public testing::Test {
protected:
	PerformanceReleaseTest() {
		initMovesGenerator();
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

#ifdef PERFORMANCE_TESTS

TEST_F(PerformanceReleaseTest, generatePseudoLegalMovesPerformanceTest) {
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
	GTEST_ASSERT_NEAR(time, 1500);
	cout << "time: " << time  << endl;

	delete boardInitial;
	delete boardPerft2;
	delete boardPerft3;
	delete boardPerft4;
	delete boardPerft5;
	delete boardPerft6;
}

TEST_F(PerformanceReleaseTest, simulatePerformanceTest) {
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
	GTEST_ASSERT_NEAR(time, 1350);
	cout << "time: " << time  << endl;

	delete game;
}

TEST_F(PerformanceReleaseTest, generateLegalMovesPerformanceTest) {
	Move moves[MAX_MOVES];
	Game* boardInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* boardPerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* boardPerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* boardPerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* boardPerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* boardPerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 100000; ++i) {
		generateLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		generateLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		generateLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		generateLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		generateLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		generateLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		generateLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		generateLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		generateLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		generateLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		generateLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
		generateLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
	}

	unsLL time = getElapsedMillis(begin);
	GTEST_ASSERT_NEAR(time, 1270);
	cout << "time: " << time  << endl;

	delete boardInitial;
	delete boardPerft2;
	delete boardPerft3;
	delete boardPerft4;
	delete boardPerft5;
	delete boardPerft6;
}

TEST_F(PerformanceReleaseTest, calculateCheckPositionsPerformanceTest) {
	Game* game1 = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* game2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* game3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* game4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* game4m = FEN::fenToNewGame(PERFT_FEN_POSITION_4_BLACK);
	Game* game5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* game6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto start = chrono::steady_clock::now();

	for (int i = 1; i < 500000; ++i) {
		game1->calculateCheckPositions(_WHITE, 60);
		game1->calculateCheckPositions(_BLACK, 4);
		game2->calculateCheckPositions(_WHITE, 60);
		game2->calculateCheckPositions(_BLACK, 4);
		game3->calculateCheckPositions(_WHITE, 31);
		game3->calculateCheckPositions(_BLACK, 32);
		game4->calculateCheckPositions(_WHITE, 60);
		game4->calculateCheckPositions(_BLACK, 6);
		game4m->calculateCheckPositions(_WHITE, 62);
		game4m->calculateCheckPositions(_BLACK, 4);
		game5->calculateCheckPositions(_WHITE, 61);
		game5->calculateCheckPositions(_BLACK, 4);
		game6->calculateCheckPositions(_WHITE, 62);
		game6->calculateCheckPositions(_BLACK, 6);
	}

	unsLL time = getElapsedMillis(start);
	GTEST_ASSERT_NEAR(time, 420);
	cout << "time: " << time  << endl;

	delete game1;
	delete game2;
	delete game3;
	delete game4;
	delete game4m;
	delete game5;
	delete game6;
}

TEST_F(PerformanceReleaseTest, allAttacksTest) {
	Game* game1 = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* game2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* game3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* game4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* game4m = FEN::fenToNewGame(PERFT_FEN_POSITION_4_BLACK);
	Game* game5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* game6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto start = chrono::steady_clock::now();

	for (int i = 1; i < 5000000; ++i) {
		allAttacks(game1->board,_WHITE, 60);
		allAttacks(game1->board,_BLACK, 4);
		allAttacks(game2->board,_WHITE, 60);
		allAttacks(game2->board,_BLACK, 4);
		allAttacks(game3->board,_WHITE, 31);
		allAttacks(game3->board,_BLACK, 32);
		allAttacks(game4->board,_WHITE, 60);
		allAttacks(game4->board,_BLACK, 6);
		allAttacks(game4m->board,_WHITE, 62);
		allAttacks(game4m->board,_BLACK, 4);
		allAttacks(game5->board,_WHITE, 61);
		allAttacks(game5->board,_BLACK, 4);
		allAttacks(game6->board,_WHITE, 62);
		allAttacks(game6->board,_BLACK, 6);
	}

	unsLL time = getElapsedMillis(start);
	GTEST_ASSERT_NEAR(time, 510);
	cout << "time: " << time  << endl;

	delete game1;
	delete game2;
	delete game3;
	delete game4;
	delete game4m;
	delete game5;
	delete game6;
}

TEST_F(PerformanceReleaseTest, finalizeMovePerformanceTest) {
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
	const unsigned int amount = generateLegalMoves(*gamePerft3, perft3Moves);
	generateLegalMoves(*gamePerft5, perft5Moves);
	generateLegalMoves(*gamePerft6, perft6Moves);

	unsLL time = 0;

	for (int count = 0; count < 50000; count++) {
		for (int index = 0; index < amount; index++) {
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
	GTEST_ASSERT_NEAR(time, 800);
	cout << "time: " << time << endl;

	delete gameInitial;
	delete gamePerft2;
	delete gamePerft3;
	delete gamePerft5;
	delete gamePerft6;
}

TEST_F(PerformanceReleaseTest, castlingPerformanceTest) {
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
	GTEST_ASSERT_NEAR(time, 1100);
	cout << "time: " << time  << endl;

	delete game;
}

TEST_F(PerformanceReleaseTest, castlingMaskPerformanceTest) {
	Game* game = FEN::fenToNewGame(CASTLING_FEN_POSITION);
	auto moves = new Move[MAX_MOVES];
	const unsigned int amount = generateLegalMoves(*game, moves);

	auto begin = chrono::steady_clock::now();

	for (long count = 0; count < 100000000; count++) {
		for (int m = 0; m < amount; m++) {
			if (moves[m]) {
				game->board->castlingInfo = 0b1111;
				updateCastlingInfo(game->board, getSourcePosition(moves[m]), getDestinationPosition(moves[m]));
			}
		}
	}

	unsLL time = getElapsedMillis(begin);
	GTEST_ASSERT_NEAR(time, 2500);		// TODO era 1900 poi e' passata a 2500, xche' ?
	cout << "time: " << time  << endl;

	delete game;
}

TEST_F(PerformanceReleaseTest, saveSnapshotTest) {
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
		saveSnapshot(game1->board, game1->sideToMove, game1->fullMoves, game1->halfMoveClock, game1->key, snapshots, i++);
		saveSnapshot(game2->board, game2->sideToMove, game2->fullMoves, game2->halfMoveClock, game2->key, snapshots, i++);
		saveSnapshot(game3->board, game3->sideToMove, game3->fullMoves, game3->halfMoveClock, game3->key, snapshots, i++);
		saveSnapshot(game4->board, game4->sideToMove, game4->fullMoves, game4->halfMoveClock, game4->key, snapshots, i++);
		saveSnapshot(game5->board, game5->sideToMove, game5->fullMoves, game5->halfMoveClock, game5->key, snapshots, i++);
		saveSnapshot(game6->board, game6->sideToMove, game6->fullMoves, game6->halfMoveClock, game6->key, snapshots, i++);
	}

	auto saveTime = getElapsedMillis(start);
	GTEST_ASSERT_NEAR(saveTime, 1090);
	cout << "time: " << saveTime  << endl;

	deallocateSnapshots(snapshots, testSize);

	delete game1;
	delete game2;
	delete game3;
	delete game4;
	delete game5;
	delete game6;
}

TEST_F(PerformanceReleaseTest, loadSnapshotTest) {
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
		saveSnapshot(game1->board, game1->sideToMove, game1->fullMoves, game1->halfMoveClock, game1->key, snapshots, i++);
		saveSnapshot(game2->board, game2->sideToMove, game2->fullMoves, game2->halfMoveClock, game2->key, snapshots, i++);
		saveSnapshot(game3->board, game3->sideToMove, game3->fullMoves, game3->halfMoveClock, game3->key, snapshots, i++);
		saveSnapshot(game4->board, game4->sideToMove, game4->fullMoves, game4->halfMoveClock, game4->key, snapshots, i++);
		saveSnapshot(game5->board, game5->sideToMove, game5->fullMoves, game5->halfMoveClock, game5->key, snapshots, i++);
		saveSnapshot(game6->board, game6->sideToMove, game6->fullMoves, game6->halfMoveClock, game6->key, snapshots, i++);
	}

	auto start = chrono::steady_clock::now();
	for (int i = testSize - 1; i >= 0;) {
		loadSnapshot(game1->board, game1->sideToMove, game1->fullMoves, game1->halfMoveClock, game1->key, snapshots, i--);
		loadSnapshot(game2->board, game2->sideToMove, game2->fullMoves, game2->halfMoveClock, game2->key, snapshots, i--);
		loadSnapshot(game3->board, game3->sideToMove, game3->fullMoves, game3->halfMoveClock, game3->key, snapshots, i--);
		loadSnapshot(game4->board, game4->sideToMove, game4->fullMoves, game4->halfMoveClock, game4->key, snapshots, i--);
		loadSnapshot(game5->board, game5->sideToMove, game5->fullMoves, game5->halfMoveClock, game5->key, snapshots, i--);
		loadSnapshot(game6->board, game6->sideToMove, game6->fullMoves, game6->halfMoveClock, game6->key, snapshots, i--);
	}

	auto rollbackTime = getElapsedMillis(start);
	GTEST_ASSERT_NEAR(rollbackTime, 1100);
	cout << "time: " << rollbackTime  << endl;

	deallocateSnapshots(snapshots, testSize);

	delete game1;
	delete game2;
	delete game3;
	delete game4;
	delete game5;
	delete game6;
}

#endif
#endif
