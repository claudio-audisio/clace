#include <gtest/gtest.h>
#include <chrono>

#include "../board/board.h"
#include "../utils/fen.h"
#include "../utils/boardUtils.h"
#include "../utils/positions.h"
#include "../utils/utils.h"
#include "../move/movesGenerator.h"
#include "../move/move.h"

using namespace std;


class BoardPerformanceTest : public testing::Test {
protected:
	BoardPerformanceTest() {
		BoardUtils::initAttacks();
	}
	~BoardPerformanceTest() {

	}
};

TEST_F(BoardPerformanceTest, calculateLegalMovesPerformanceTest) {
	GTEST_SKIP();
	vector<Move> moves;
    moves.reserve(5000);
	Game* boardInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* boardPerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* boardPerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* boardPerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* boardPerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* boardPerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 50000; ++i) {
		MovesGenerator::generateLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		MovesGenerator::generateLegalMoves(*boardInitial, moves);
		boardInitial->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft2, moves);
		boardPerft2->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft3, moves);
		boardPerft3->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft4, moves);
		boardPerft4->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft5, moves);
		boardPerft5->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
		MovesGenerator::generateLegalMoves(*boardPerft6, moves);
		boardPerft6->changeTurn();
		moves.clear();
	}

	cout << "time: " << Utils::getElapsedMillis(begin) << endl;

	// 1300
}

TEST_F(BoardPerformanceTest, getAttacksPerformanceTest) {
	GTEST_SKIP();
	Game* gameInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* gamePerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* gamePerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* gamePerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* gamePerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* gamePerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 10000000; ++i) {
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

	cout << "time: " << Utils::getElapsedMillis(begin) << endl;

	// 1020
}

TEST_F(BoardPerformanceTest, getQueenAttacksPerformanceTest) {
	GTEST_SKIP();
	Game* gameInitial = FEN::fenToNewGame(INITIAL_FEN_POSITION);
	Game* gamePerft2 = FEN::fenToNewGame(PERFT_FEN_POSITION_2);
	Game* gamePerft3 = FEN::fenToNewGame(PERFT_FEN_POSITION_3);
	Game* gamePerft4 = FEN::fenToNewGame(PERFT_FEN_POSITION_4);
	Game* gamePerft5 = FEN::fenToNewGame(PERFT_FEN_POSITION_5);
	Game* gamePerft6 = FEN::fenToNewGame(PERFT_FEN_POSITION_6);

	auto begin = chrono::steady_clock::now();

	for (int i = 1; i < 50000000; ++i) {
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

    cout << "time: " << Utils::getElapsedMillis(begin) << endl;

	// 5937
	// 5941     (attacks)
}

TEST_F(BoardPerformanceTest, castlingPerformanceTest) {
    GTEST_SKIP();

	Game* game = FEN::fenToNewGame("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	Move BQCastlingMove = MoveHelper::getMove(4, 2, BLACK);
	MoveHelper::decorate(BQCastlingMove, BKing);
	Move BKCastlingMove = MoveHelper::getMove(4, 6, BLACK);
	MoveHelper::decorate(BKCastlingMove, BKing);
	Move WQCastlingMove = MoveHelper::getMove(60, 58, WHITE);
	MoveHelper::decorate(WQCastlingMove, WKing);
	Move WKCastlingMove = MoveHelper::getMove(60, 62, WHITE);
	MoveHelper::decorate(WKCastlingMove, WKing);

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

	cout << "time: " << Utils::getElapsedMillis(begin) << endl;

	// 6600
	// 4354
	// 3000
}

TEST_F(BoardPerformanceTest, castlingMaskPerformanceTest) {
	GTEST_SKIP();

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

	cout << "time: " << Utils::getElapsedMillis(begin) << endl;

	// 3265 (debug)
	// 2635 (debug)
}

TEST_F(BoardPerformanceTest, stuffTest) {
	GTEST_SKIP();
	Position source = 4;
	Position destination = 2;

	Move move = MoveHelper::getMove(source, destination, WHITE);
	MoveHelper::decorate(move, BKing);

	Game* game = FEN::fenToNewGame(CASTLING_FEN_POSITION);
	Board board = game->getCopyBoard();

	game->simulateMove(move);
	game->undoSimulateMove(move);

	/*BoardUtils::printBoard(game->board.BOARD(WHITE));
	BoardUtils::printBoard(game->board.BOARD(BLACK));
	BoardUtils::printBoard(game->board.EMPTY);*/

	cout << (int)game->getWhiteKingPosition() << endl;
	cout << (int)game->getBlackKingPosition() << endl;

	GTEST_ASSERT_TRUE(game->board.equals(board));
}


