#include <gtest/gtest.h>
#include <chrono>

#include "../board/board.h"
#include "../utils/fen.h"
#include "../utils/boardUtils.h"
#include "../utils/positions.h"
#include "../utils/utils.h"
#include "../move/movesGenerator.h"

using namespace std;


TEST(BoardPerformanceTest, calculateLegalMovesPerformanceTest) {
	GTEST_SKIP();
	Game* boardInitial = FEN::fenToNewGame(Positions::INITIAL_FEN_POSITION);
	Game* boardPerft2 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_2);
	Game* boardPerft3 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_3);
	Game* boardPerft4 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_4);
	Game* boardPerft5 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_5);
	Game* boardPerft6 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 10000; ++i) {
		MovesGenerator::calculateLegalMoves(*boardInitial);
		boardInitial->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardInitial);
		boardInitial->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft2);
		boardPerft2->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft2);
		boardPerft2->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft3);
		boardPerft3->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft3);
		boardPerft3->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft4);
		boardPerft4->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft4);
		boardPerft4->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft5);
		boardPerft5->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft5);
		boardPerft5->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft6);
		boardPerft6->changeTurn();
		MovesGenerator::calculateLegalMoves(*boardPerft6);
		boardPerft6->changeTurn();
	}

	EXPECT_NE(1, 1)
		<< "time: " << Utils::getElapsedMillis(begin) << endl;

	// 1950
}

TEST(BoardPerformanceTest, getAttacksPerformanceTest) {
	GTEST_SKIP();
	Game* gameInitial = FEN::fenToNewGame(Positions::INITIAL_FEN_POSITION);
	Game* gamePerft2 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_2);
	Game* gamePerft3 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_3);
	Game* gamePerft4 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_4);
	Game* gamePerft5 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_5);
	Game* gamePerft6 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 1000000; ++i) {
		gameInitial->getBoard().getWhiteAttacks();
		gameInitial->getBoard().getBlackAttacks();
		gamePerft2->getBoard().getWhiteAttacks();
		gamePerft2->getBoard().getBlackAttacks();
		gamePerft3->getBoard().getWhiteAttacks();
		gamePerft3->getBoard().getBlackAttacks();
		gamePerft4->getBoard().getWhiteAttacks();
		gamePerft4->getBoard().getBlackAttacks();
		gamePerft5->getBoard().getWhiteAttacks();
		gamePerft5->getBoard().getBlackAttacks();
		gamePerft6->getBoard().getWhiteAttacks();
		gamePerft6->getBoard().getBlackAttacks();
	}

	EXPECT_NE(1, 1)
		<< "time: " << Utils::getElapsedMillis(begin) << endl;

	// 1750
}

TEST(BoardPerformanceTest, getQueenAttacksPerformanceTest) {
	GTEST_SKIP();
	Game* gameInitial = FEN::fenToNewGame(Positions::INITIAL_FEN_POSITION);
	Game* gamePerft2 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_2);
	Game* gamePerft3 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_3);
	Game* gamePerft4 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_4);
	Game* gamePerft5 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_5);
	Game* gamePerft6 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 10000000; ++i) {
		gameInitial->getBoard().getQueenAttacks(true);
		gameInitial->getBoard().getQueenAttacks(false);
		gamePerft2->getBoard().getQueenAttacks(true);
		gamePerft2->getBoard().getQueenAttacks(false);
		gamePerft3->getBoard().getQueenAttacks(true);
		gamePerft3->getBoard().getQueenAttacks(false);
		gamePerft4->getBoard().getQueenAttacks(true);
		gamePerft4->getBoard().getQueenAttacks(false);
		gamePerft5->getBoard().getQueenAttacks(true);
		gamePerft5->getBoard().getQueenAttacks(false);
		gamePerft6->getBoard().getQueenAttacks(true);
		gamePerft6->getBoard().getQueenAttacks(false);
	}

	EXPECT_NE(1, 1)
		<< "time: " << Utils::getElapsedMillis(begin) << endl;

	// 3800 (getQueenMoves)
	// 5600 (queenAttacks)
}