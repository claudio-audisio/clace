#include <gtest/gtest.h>
#include <string>
#include <numeric>

#include "../move/rollback.h"
#include "../game/game.h"
#include "../utils/fen.h"
#include "../utils/utils.h"

using namespace std;


TEST(RollbackTest, rollbackTest1) {
	Game game;
	game.init();
	Rollback* rollback = new Rollback();
	const string boardBeforeRollback = FEN::gameToFEN(game);

	Move* move = new Move(48, 32, true);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 1);

	game.applyMove(*move);
	rollback->rollback(game);
	const string boardAfterRollback = FEN::gameToFEN(game);

	EXPECT_EQ(rollback->getRollbackSize(), 0);
	EXPECT_EQ(boardBeforeRollback, boardAfterRollback);
}

TEST(RollbackTest, rollbackInfo2Test) {
	Game game;
	game.init();
	Rollback* rollback = new Rollback();
	const string boardBeforeRollback = FEN::gameToFEN(game);

	Move* move = new Move(57, 40, true);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 1);

	game.applyMove(*move);

	move = new Move(40, 57, true);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 2);

	game.applyMove(*move);

	move = new Move(57, 40, true);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 3);

	game.applyMove(*move);

	rollback->rollback(game);
	EXPECT_EQ(rollback->getRollbackSize(), 2);
	rollback->rollback(game);
	EXPECT_EQ(rollback->getRollbackSize(), 1);
	rollback->rollback(game);
	EXPECT_EQ(rollback->getRollbackSize(), 0);

	string boardAfterRollback = FEN::gameToFEN(game);

	EXPECT_EQ(boardBeforeRollback, boardAfterRollback);
}


TEST(RollbackTest, rollbackInfoFailureTest) {
	GTEST_SKIP();
	Game game;
	game.init();
	Rollback* rollback = new Rollback();
	const string boardBeforeRollback = FEN::gameToFEN(game);

	Move* move = new Move(48, 32, true);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 1);

	game.applyMove(*move);
	rollback->rollback(game);
	string boardAfterRollback = FEN::gameToFEN(game);

	EXPECT_EQ(rollback->getRollbackSize(), 0);
	EXPECT_EQ(boardBeforeRollback, boardAfterRollback);

	/*	TODO da finire quando gestiamo le eccezioni
	Exception exception = assertThrows(RuntimeException.class, () -> {
		rollback->rollback(board);
	});

	EXPECT_EQ(exception.getMessage(), "rollback failed: empty stack");

	EXPECT_THROW(statement,exception_type)
	*/
}

TEST(RollbackTest, performanceTest) {
	GTEST_SKIP();
	list<long long> saveTimes;
	list<long long> rollbackTimes;
	Game* game = FEN::fenToNewGame(Positions::PERFT_FEN_POSITION_2);
	game->verifyChecks();

	for (int i = 0; i < 10000; ++i) {
		auto start = chrono::steady_clock::now();
		game->save();
		saveTimes.push_back(Utils::getElapsedNanos(start));

		game->applyMove(*(new Move("g2-h3", true)));

		start = chrono::steady_clock::now();
		game->rollbackLastMove();
		rollbackTimes.push_back(Utils::getElapsedNanos(start));
	}

	EXPECT_NE(1, 1)
		<< "save avg time: " << accumulate(saveTimes.begin(), saveTimes.end(), 0.0) / saveTimes.size() << endl
		<< "rollback avg time: " << accumulate(rollbackTimes.begin(), rollbackTimes.end(), 0.0) / rollbackTimes.size() << endl;
	

	// 200
	// 60
}