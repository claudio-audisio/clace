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

#if RELEASE_MODE


class PerformanceReleaseTest : public testing::Test {
protected:
	PerformanceReleaseTest() {
		initAttacks();
	}
	~PerformanceReleaseTest() {

	}
	void static GTEST_ASSERT_NEAR(unsLL value, unsLL expected, unsLL delta) {
		GTEST_ASSERT_LT(value, expected + delta);
		GTEST_ASSERT_GT(value, expected - delta);
	}
};

TEST_F(PerformanceReleaseTest, calculateLegalMovesPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	vector<Move> moves;
    moves.reserve(MAX_MOVES);
	Game* boardInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* boardPerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* boardPerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* boardPerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* boardPerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* boardPerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 100000; ++i) {
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
		moves.clear();
		MovesGenerator::generateLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
	}

	unsLL time = Utils::getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1730, 50);
	//GTEST_ASSERT_NEAR(time, 1760, 50);	// USE_PRE_CALCULATED_BOARDS

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, calculateLegalMovesNewPerformanceTest) {
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
		MovesGenerator::generateLegalMoves_new(*boardInitial, moves);
		boardInitial->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardInitial, moves);
		boardInitial->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft2, moves);
		boardPerft2->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft2, moves);
		boardPerft2->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft3, moves);
		boardPerft3->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft3, moves);
		boardPerft3->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft4, moves);
		boardPerft4->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft4, moves);
		boardPerft4->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft5, moves);
		boardPerft5->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft5, moves);
		boardPerft5->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft6, moves);
		boardPerft6->changeTurn();
		memset(moves, 0, sizeof(moves));
		MovesGenerator::generateLegalMoves_new(*boardPerft6, moves);
		boardPerft6->changeTurn();
	}

	unsLL time = Utils::getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1730, 50);
	//GTEST_ASSERT_NEAR(time, 1800, 50);	// USE_PRE_CALCULATED_BOARDS

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

	unsLL time = Utils::getElapsedMillis(start);

	GTEST_ASSERT_NEAR(time, 1130, 50);
	//GTEST_ASSERT_NEAR(time, 1100, 50);		// USE_PRE_CALCULATED_BOARDS

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

	unsLL time = Utils::getElapsedMillis(begin);

	//**** BOARD_STANDARD_RAY_ATTACKS ****///
	GTEST_ASSERT_NEAR(time, 1280, 50);
	//GTEST_ASSERT_NEAR(time, 2170, 50);		// USE_PRE_CALCULATED_BOARDS

	//**** BOARD_BRANCHLESS_RAY_ATTACKS ****///
	//GTEST_ASSERT_NEAR(time, 1300, 50);
	//GTEST_ASSERT_NEAR(time, 2160, 50);		// USE_PRE_CALCULATED_BOARDS

	//**** BOARD_ONTHEFLY_RAY_ATTACKS ****/// TODO
	//GTEST_ASSERT_NEAR(time, 1280, 50);
	//GTEST_ASSERT_NEAR(time, 2170, 50);		// USE_PRE_CALCULATED_BOARDS

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, castlingPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif

	Game* game = FEN::fenToNewGame("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	Move BQCastlingMove = MoveHelper::getMove(4, 2, BLACK, BKing);
	Move BKCastlingMove = MoveHelper::getMove(4, 6, BLACK, BKing);
	Move WQCastlingMove = MoveHelper::getMove(60, 58, WHITE, WKing);
	Move WKCastlingMove = MoveHelper::getMove(60, 62, WHITE, WKing);

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

	unsLL time = Utils::getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1980, 50);

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, castlingMaskPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif

	Game* game = FEN::fenToNewGame(CASTLING_FEN_POSITION);
	auto moves = new Move[MAX_MOVES];
	pair<int, int> res = MovesGenerator::generateLegalMoves_new(*game, moves);

	auto begin = chrono::steady_clock::now();

	for (long count = 0; count < 100000000; count++) {
		for (int m = 0; m < res.first; m++) {
			if (moves[m]) {
				game->board.castlingInfo = 0b1111;
				game->board.updateCastlingInfo(MoveHelper::getSourcePosition(moves[m]), MoveHelper::getDestinationPosition(moves[m]));
			}
		}
	}

	unsLL time = Utils::getElapsedMillis(begin);

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

	auto saveTime = Utils::getElapsedMillis(start);
	GTEST_ASSERT_NEAR(saveTime, 1800, 50);
	cout << "save time: " << saveTime  << endl;

	start = chrono::steady_clock::now();
	for (int i = 0; i < size; ++i) {
		rollback->rollback(*game);
	}

	auto rollbackTime = Utils::getElapsedMillis(start);
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

	unsLL time = Utils::getElapsedMillis(begin);

	GTEST_ASSERT_NEAR(time, 1700, 50);	// TODO imbarazzante

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, Perft5Test) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	auto perft = new Perft(INITIAL_FEN_POSITION, 5);

	auto result = perft->runBulk();

	GTEST_ASSERT_NEAR(result->getElapsed(), 230, 10);
	//GTEST_ASSERT_NEAR(result->getElapsed(), 230, 10);		// BOARD_USE_PRE_CALCULATED
	//GTEST_ASSERT_NEAR(result->getElapsed(), 270, 10);		// PERFT_USE_CACHE

	cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformanceReleaseTest, Perft5NewTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	auto perft = new Perft(INITIAL_FEN_POSITION, 5);

	auto result = perft->runBulk_new();

	GTEST_ASSERT_NEAR(result->getElapsed(), 225, 10);
	//GTEST_ASSERT_NEAR(result->getElapsed(), 240, 10);		// BOARD_USE_PRE_CALCULATED
	//GTEST_ASSERT_NEAR(result->getElapsed(), 270, 10);		// PERFT_USE_CACHE	// TODO da fare per la versione new

	cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformanceReleaseTest, Perft6Test) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	auto perft = new Perft(INITIAL_FEN_POSITION, 6);

	auto result = perft->runBulk();

	GTEST_ASSERT_NEAR(result->getElapsed(), 6030, 50);
	//GTEST_ASSERT_NEAR(result->getElapsed(), 6100, 50);		// BOARD_USE_PRE_CALCULATED
	//GTEST_ASSERT_NEAR(result->getElapsed(), 5310, 50);		// PERFT_USE_CACHE

	cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformanceReleaseTest, Perft6NewTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	auto perft = new Perft(INITIAL_FEN_POSITION, 6);

	auto result = perft->runBulk_new();

	GTEST_ASSERT_NEAR(result->getElapsed(), 6050, 50);
	//GTEST_ASSERT_NEAR(result->getElapsed(), 6110, 50);		// BOARD_USE_PRE_CALCULATED
	//GTEST_ASSERT_NEAR(result->getElapsed(), 270, 10);		// PERFT_USE_CACHE	// TODO da fare per la versione new

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
	auto moves = new vector<Move>();

	auto begin = chrono::steady_clock::now();

	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	unsLL time = Utils::getElapsedMillis(begin);

	EXPECT_EQ(MoveHelper::toString(best.first), "e2e3");
	GTEST_ASSERT_NEAR(time, 340, 10);

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, BFEngineMidGameDepth4Test) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(PERFT_FEN_POSITION_6);
	auto engine = new BF_Engine(3);
	engine->setEvaluator(new BasicEvaluator());
	auto moves = new vector<Move>();

	auto begin = chrono::steady_clock::now();

	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	unsLL time = Utils::getElapsedMillis(begin);

	EXPECT_EQ(MoveHelper::toString(best.first), "c3d5");
	GTEST_ASSERT_NEAR(time, 240, 10);

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceReleaseTest, BFEngineEndGameDepth4Test) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	Game game;
	game.initFromFEN(PERFT_FEN_POSITION_3);
	auto engine = new BF_Engine(5);
	engine->setEvaluator(new BasicEvaluator());
	auto moves = new vector<Move>();

	auto begin = chrono::steady_clock::now();

	MovesGenerator::generateLegalMoves(game, *moves);
	Evaluation best = engine->calculateMove(game, *moves);

	unsLL time = Utils::getElapsedMillis(begin);

	EXPECT_EQ(MoveHelper::toString(best.first), "b4f4");
	GTEST_ASSERT_NEAR(time, 590, 10);

	cout << "time: " << time  << endl;
}


#endif
