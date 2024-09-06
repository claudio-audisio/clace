#include <gtest/gtest.h>
#include <string>

#include "../move/rollback.h"
#include "../game/game.h"
#include "../utils/fen.h"

using namespace std;


TEST(RollbackkTest, rollbackTest1) {
	GTEST_SKIP();
	Game game;
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

/*
void rollbackInfo2Test() {
	Move move = new Move(57, 40, true);
	rollbackMoveInfo.save(board);

	assertThat(rollbackMoveInfo.getRollbackSize()).isEqualTo(1);

	board.applyMove(move);

	move = new Move(40, 57, true);
	rollbackMoveInfo.save(board);

	assertThat(rollbackMoveInfo.getRollbackSize()).isEqualTo(2);

	board.applyMove(move);

	move = new Move(57, 40, true);
	rollbackMoveInfo.save(board);

	assertThat(rollbackMoveInfo.getRollbackSize()).isEqualTo(3);

	board.applyMove(move);

	rollbackMoveInfo.rollback(board);
	assertThat(rollbackMoveInfo.getRollbackSize()).isEqualTo(2);
	rollbackMoveInfo.rollback(board);
	assertThat(rollbackMoveInfo.getRollbackSize()).isEqualTo(1);
	rollbackMoveInfo.rollback(board);
	assertThat(rollbackMoveInfo.getRollbackSize()).isEqualTo(0);

	String boardAfterRollback = FENConverter.boardToFEN(board);

	assertThat(boardBeforeRollback).isEqualTo(boardAfterRollback);
}

@Test
void rollbackInfoFailureTest() {
	Move move = new Move(48, 32, true);
	rollbackMoveInfo.save(board);

	assertThat(rollbackMoveInfo.getRollbackSize()).isEqualTo(1);

	board.applyMove(move);
	rollbackMoveInfo.rollback(board);
	String boardAfterRollback = FENConverter.boardToFEN(board);

	assertThat(rollbackMoveInfo.getRollbackSize()).isEqualTo(0);
	assertThat(boardBeforeRollback).isEqualTo(boardAfterRollback);

	Exception exception = assertThrows(RuntimeException.class, () -> {
		rollbackMoveInfo.rollback(board);
	});

	assertThat(exception.getMessage()).isEqualTo("rollback failed: empty stack");
}

@Disabled
@Test
void performanceTest() {
	List<Long> saveTimes = new ArrayList<>();
	List<Long> rollbackTimes = new ArrayList<>();
	Board board = FENConverter.fenToNewBoard(Positions.PERFT_FEN_POSITION_2);
	board.verifyChecks();

	for (int i = 0; i < 10000000; ++i) {
		long time = System.nanoTime();
		board.save();
		saveTimes.add(System.nanoTime() - time);

		board.applyMove(new Move("g2-h3", true));

		time = System.nanoTime();
		board.rollbackLastMove();
		rollbackTimes.add(System.nanoTime() - time);
	}

	System.out.println("save avg time: " + saveTimes.stream().collect(Collectors.averagingLong(Long::longValue)));
	System.out.println("rollback avg time: " + rollbackTimes.stream().collect(Collectors.averagingLong(Long::longValue)));

	// 650
	// 560
}
*/