#include <gtest/gtest.h>
#include <chrono>

#include "../board/board.h"
#include "../utils/fen.h"
#include "../utils/boardUtils.h"
#include "../utils/positions.h"
#include "../utils/utils.h"
#include "../move/movesGenerator.h"

using namespace std;

// NOTE La slidingAttack va molto bene quando abbiamo una scacchiera densa, perche' e' iterativo e si ferma all'ostacolo
// Il RayAttacks fa sempre gli stessi calcoli, quindi in ambienti spogli va molto meglio
// Scacchiera densa: Ray e' 1/3 piu' lento di Sliding
// Scacchiera spoglia: Sliding e' 4/5 piu' lento di Ray
// TODO test aggiuntivi con scacchiere casuali (a diminuire di numero di pezzi)




TEST(BoardPerformanceTest, calculateLegalMovesPerformanceTest) {
	GTEST_SKIP();
	vector<Move> moves;
    moves.reserve(5000);
	Game* boardInitial = FEN::fenToNewGame(Positions::INITIAL_FEN_POSITION);
	Game* boardPerft2 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_2);
	Game* boardPerft3 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_3);
	Game* boardPerft4 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_4);
	Game* boardPerft5 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_5);
	Game* boardPerft6 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_6);

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

	// 1800
	// 2250     (attacks)
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

	for (int i = 1; i < 10000000; ++i) {
		gameInitial->getBoard().getAttacks(WHITE);
		gameInitial->getBoard().getAttacks(BLACK);
		gamePerft2->getBoard().getAttacks(WHITE);
		gamePerft2->getBoard().getAttacks(BLACK);
		gamePerft3->getBoard().getAttacks(WHITE);
		gamePerft3->getBoard().getAttacks(BLACK);
		gamePerft4->getBoard().getAttacks(WHITE);
		gamePerft4->getBoard().getAttacks(BLACK);
		gamePerft5->getBoard().getAttacks(WHITE);
		gamePerft5->getBoard().getAttacks(BLACK);
		gamePerft6->getBoard().getAttacks(WHITE);
		gamePerft6->getBoard().getAttacks(BLACK);
	}

	cout << "time: " << Utils::getElapsedMillis(begin) << endl;

	// 5050
	// 6950     (attacks)
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

	for (int i = 1; i < 100000000; ++i) {
		//gameInitial->getBoard().getQueenAttacks(WHITE);
		//gameInitial->getBoard().getQueenAttacks(BLACK);
		gamePerft2->getBoard().getQueenAttacks(WHITE);
		gamePerft2->getBoard().getQueenAttacks(BLACK);
		gamePerft3->getBoard().getQueenAttacks(WHITE);
		gamePerft3->getBoard().getQueenAttacks(BLACK);
		gamePerft4->getBoard().getQueenAttacks(WHITE);
		gamePerft4->getBoard().getQueenAttacks(BLACK);
		/*gamePerft5->getBoard().getQueenAttacks(WHITE);
		gamePerft5->getBoard().getQueenAttacks(BLACK);*/
		gamePerft6->getBoard().getQueenAttacks(WHITE);
		gamePerft6->getBoard().getQueenAttacks(BLACK);
	}

    cout << "time: " << Utils::getElapsedMillis(begin) << endl;

	// 1200
	// 1636     (attacks)

	// 990
	// 10300
}

TEST(BoardPerformanceTest, stuffTest) {
    GTEST_SKIP();
    //Game* boardInitial = FEN::fenToNewGame(Positions::INITIAL_FEN_POSITION);
    /*Game* boardPerft2 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_2);
    Game* boardPerft3 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_3);
    Game* boardPerft4 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_4);
    Game* boardPerft5 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_5);
    Game* boardPerft6 = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_6);*/
	Game* game = FEN::fenToNewGame("3k4/8/8/8/1Q6/8/8/4K3 w - - 0 1");

    auto begin = chrono::steady_clock::now();

    for (int i = 1; i < 100000000; ++i) {
		/*const Rawboard posIndex = BoardUtils::posInd(33);
		const Rawboard oppositeBoard = game->getBoard().OPPOSITE(WHITE);
		Rawboard attack = game->getBoard().slidingAttack(Board::soWestOne, posIndex, oppositeBoard);*/
		//BoardUtils::printBoard(attack);

		/*const Rawboard occupied = ~game->getBoard().EMPTY;
		const Rawboard notSide = ~game->getBoard().BOARD(WHITE);
		Rawboard attack = Board::soWestAttack(occupied, 33)  & notSide;*/
		//BoardUtils::printBoard(attack);

		game->getBoard().getQueenAttacks(WHITE);
    }

    cout << "time: " << Utils::getElapsedMillis(begin) << endl;

	// sliding	4900
	// blocker	1250

	// queen sliding	3056	1080
	// queen blocker	2138	1289
}