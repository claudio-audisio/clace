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

// *************************************************************** //
// IMPORTANT! Run these tests in DEBUG mode to avoid optimizations //
// *************************************************************** //

class PerformanceTest : public testing::Test {
protected:
	PerformanceTest() {
		initAttacks();
	}
	~PerformanceTest() {

	}
};

TEST_F(PerformanceTest, calculateLegalMovesPerformanceTest) {
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

	for (int i = 1; i < 10000; ++i) {
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
	GTEST_ASSERT_LT(time, 1450);

	cout << "time: " << time  << endl;
	// NOTE with USE_PRE_CALCULATED_BOARDS is slightly worst
}

TEST_F(PerformanceTest, getAttacksPerformanceTest) {
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
	GTEST_ASSERT_LT(time, 1200);	// standard version
	//GTEST_ASSERT_LT(time, 1250);	// branchless version
	//GTEST_ASSERT_LT(time, 2500);	// onthefly version
	// NOTE with USE_PRE_CALCULATED_BOARDS has same results

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceTest, getQueenAttacksPerformanceTest) {
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
	GTEST_ASSERT_LT(time, 1600);	// standard version
	//GTEST_ASSERT_LT(time, 1650);	// branchless version
	//GTEST_ASSERT_LT(time, 3500);	// onthefly version
	// NOTE with or without USE_PRE_CALCULATED_BOARDS on board has same results

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceTest, getKnightAttacksPerformanceTest) {
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
	GTEST_ASSERT_LT(time, 1250);
	// NOTE with USE_PRE_CALCULATED_BOARDS is 25% faster

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceTest, castlingPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif

	Game* game = FEN::fenToNewGame("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	Move BQCastlingMove = MoveHelper::getMove(4, 2, BLACK, BKing);
	Move BKCastlingMove = MoveHelper::getMove(4, 6, BLACK, BKing);
	Move WQCastlingMove = MoveHelper::getMove(60, 58, WHITE, WKing);
	Move WKCastlingMove = MoveHelper::getMove(60, 62, WHITE, WKing);

	auto begin = chrono::steady_clock::now();

	for (int count = 0; count < 10000000; count++) {
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
	GTEST_ASSERT_LT(time, 1050);

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceTest, castlingMaskPerformanceTest) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif

	Game* game = FEN::fenToNewGame(CASTLING_FEN_POSITION);
	vector<Move>* moves = new vector<Move>(MAX_MOVES);
	moves->clear();
	MovesGenerator::generateLegalMoves(*game, *moves);

	auto begin = chrono::steady_clock::now();

	for (long count = 0; count < 10000000; count++) {
		for (Move move : *moves) {
			game->board.castlingInfo = 0b1111;
			game->board.updateCastlingInfo(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move));
		}
	}

	unsLL time = Utils::getElapsedMillis(begin);
	GTEST_ASSERT_LT(time, 1550);

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceTest, gameToFENKeyTest) {
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

	for (long i = 1; i < 50000; ++i) {
		FEN::gameToFENKey(*gameInitial);
		FEN::gameToFENKey(*gamePerft2);
		FEN::gameToFENKey(*gamePerft3);
		FEN::gameToFENKey(*gamePerft4);
		FEN::gameToFENKey(*gamePerft5);
		FEN::gameToFENKey(*gamePerft6);
	}

	unsLL time = Utils::getElapsedMillis(begin);
	GTEST_ASSERT_LT(time, 1250);	// TODO qua e' una vera merda
	// NOTE with USE_PRE_CALCULATED_BOARDS is 25% faster

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceTest, Perft5Test) {
#ifndef PERFORMANCE_TESTS
	GTEST_SKIP();
#endif
	auto perft = new Perft(INITIAL_FEN_POSITION, 5);

	auto result = perft->runBulk();

	GTEST_ASSERT_LT(result->getElapsed(), 1800);
	//GTEST_ASSERT_LT(result->getElapsed(), 2050);	// with cache
	// NOTE with USE_PRE_CALCULATED_BOARDS is slightly slower

	cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformanceTest, BFEngineOpenGameDepth4Test) {
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
	GTEST_ASSERT_LT(time, 2250);
	// NOTE with USE_PRE_CALCULATED_BOARDS is slightly slower
	EXPECT_EQ(MoveHelper::toString(best.first), "e2e3");

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceTest, BFEngineMidGameDepth4Test) {
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
	GTEST_ASSERT_LT(time, 1650);
	// NOTE with USE_PRE_CALCULATED_BOARDS is slightly slower
	EXPECT_EQ(MoveHelper::toString(best.first), "c3d5");

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceTest, BFEngineEndGameDepth4Test) {
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
	GTEST_ASSERT_LT(time, 3600);
	// NOTE with USE_PRE_CALCULATED_BOARDS is slightly slower
	EXPECT_EQ(MoveHelper::toString(best.first), "b4f4");

	cout << "time: " << time  << endl;
}

TEST_F(PerformanceTest, stuffTest) {
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


