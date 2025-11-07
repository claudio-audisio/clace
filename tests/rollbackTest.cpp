#include <gtest/gtest.h>
#include <string>
#include <numeric>

#include "../move/rollback.h"
#include "../game/game.h"
#include "../utils/fen.h"
#include "../utils/utils.h"
#include "../move/move.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;

class RollbackTest : public testing::Test {
protected:
	RollbackTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}
	~RollbackTest() {

	}
};

TEST_F(RollbackTest, rollbackTest1) {
	Game game;
	game.init();

	GameSnapshot** snapshots = allocateSnapshots(10);
	const string boardBeforeRollback = FEN::gameToFEN(game);
	Move move = createMove(48, 32, _WHITE, WPawn);

	saveSnapshot(game.board, game.sideToMove, game.fullMoves, game.halfMoveClock, snapshots, 0);

	game.applyMove(move);

	loadSnapshot(game.board, game.sideToMove, game.fullMoves, game.halfMoveClock, snapshots, 0);

	const string boardAfterRollback = FEN::gameToFEN(game);

	EXPECT_EQ(boardBeforeRollback, boardAfterRollback);
}

TEST_F(RollbackTest, rollbackInfo2Test) {
	Game game;
	game.init();
	GameSnapshot** snapshots = allocateSnapshots(10);
	const string boardBeforeRollback = FEN::gameToFEN(game);
	Move move = createMove(57, 40, _WHITE, WKnight);

	saveSnapshot(game.board, game.sideToMove, game.fullMoves, game.halfMoveClock, snapshots, 0);

	game.applyMove(move);
	move = createMove(40, 57, _WHITE, WKnight);

	saveSnapshot(game.board, game.sideToMove, game.fullMoves, game.halfMoveClock, snapshots, 1);

	game.applyMove(move);
	move = createMove(57, 40, _WHITE, WKnight);

	saveSnapshot(game.board, game.sideToMove, game.fullMoves, game.halfMoveClock, snapshots, 2);

	game.applyMove(move);

	loadSnapshot(game.board, game.sideToMove, game.fullMoves, game.halfMoveClock, snapshots, 2);
	loadSnapshot(game.board, game.sideToMove, game.fullMoves, game.halfMoveClock, snapshots, 1);
	loadSnapshot(game.board, game.sideToMove, game.fullMoves, game.halfMoveClock, snapshots, 0);

	string boardAfterRollback = FEN::gameToFEN(game);

	EXPECT_EQ(boardBeforeRollback, boardAfterRollback);
}

