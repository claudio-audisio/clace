#include <gtest/gtest.h>
#include <string>
#include <numeric>

#include "../move/rollback.h"
#include "../game/game.h"
#include "../utils/fen.h"
#include "../utils/utils.h"
#include "../move/move.h"

using namespace std;

class RollbackTest : public testing::Test {
protected:
	RollbackTest() {
		initAttacks();
	}
	~RollbackTest() {

	}
};

TEST_F(RollbackTest, rollbackTest1) {
	Game game;
	game.init();
	Rollback* rollback = new Rollback(10);
	const string boardBeforeRollback = FEN::gameToFEN(game);

	Move move = MoveHelper::getMove(48, 32, WHITE, WPawn);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 1);

	game.applyMove(move);
	rollback->rollback(game);
	const string boardAfterRollback = FEN::gameToFEN(game);

	EXPECT_EQ(rollback->getRollbackSize(), 0);
	EXPECT_EQ(boardBeforeRollback, boardAfterRollback);
}

TEST_F(RollbackTest, rollbackInfo2Test) {
	Game game;
	game.init();
	Rollback* rollback = new Rollback(10);
	const string boardBeforeRollback = FEN::gameToFEN(game);

	Move move = MoveHelper::getMove(57, 40, WHITE, WKnight);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 1);

	game.applyMove(move);

	move = MoveHelper::getMove(40, 57, WHITE, WKnight);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 2);

	game.applyMove(move);

	move = MoveHelper::getMove(57, 40, WHITE, WKnight);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 3);

	game.applyMove(move);

	rollback->rollback(game);
	EXPECT_EQ(rollback->getRollbackSize(), 2);
	rollback->rollback(game);
	EXPECT_EQ(rollback->getRollbackSize(), 1);
	rollback->rollback(game);
	EXPECT_EQ(rollback->getRollbackSize(), 0);

	string boardAfterRollback = FEN::gameToFEN(game);

	EXPECT_EQ(boardBeforeRollback, boardAfterRollback);
}

TEST_F(RollbackTest, rollbackInfoFailureTest) {
	Game game;
	game.init();
	Rollback* rollback = new Rollback(10);
	const string boardBeforeRollback = FEN::gameToFEN(game);

	Move move = MoveHelper::getMove(48, 32, true);
	rollback->save(game);

	EXPECT_EQ(rollback->getRollbackSize(), 1);

	game.applyMove(move);
	rollback->rollback(game);
	string boardAfterRollback = FEN::gameToFEN(game);

	EXPECT_EQ(rollback->getRollbackSize(), 0);
	EXPECT_EQ(boardBeforeRollback, boardAfterRollback);

	EXPECT_THROW(rollback->rollback(game), runtime_error);
}
