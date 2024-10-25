#include <gtest/gtest.h>
#include <chrono>
#include <thread>

#include "../game/player.h"
#include "../board/piece.h"

using namespace std;


TEST(PlayerTest, onCapturedWhiteTest) {
	Player* player = new Player(true);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 2);
	EXPECT_EQ(player->pieces[WBishop], 2);
	EXPECT_EQ(player->pieces[WKnight], 2);
	EXPECT_EQ(player->pieces[WPawn], 8);
	GTEST_ASSERT_TRUE(player->getCapturedList().empty());

	player->onCaptured(WPawn);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 2);
	EXPECT_EQ(player->pieces[WBishop], 2);
	EXPECT_EQ(player->pieces[WKnight], 2);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "P ");

	player->onCaptured(WKnight);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 2);
	EXPECT_EQ(player->pieces[WBishop], 2);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "N P ");

	player->onCaptured(WBishop);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 2);
	EXPECT_EQ(player->pieces[WBishop], 1);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "B N P ");

	player->onCaptured(WRook);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 1);
	EXPECT_EQ(player->pieces[WBishop], 1);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "R B N P ");

	player->onCaptured(WQueen);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 0);
	EXPECT_EQ(player->pieces[WRook], 1);
	EXPECT_EQ(player->pieces[WBishop], 1);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "Q R B N P ");

	player->onCaptured(WKing);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 0);
	EXPECT_EQ(player->pieces[WQueen], 0);
	EXPECT_EQ(player->pieces[WRook], 1);
	EXPECT_EQ(player->pieces[WBishop], 1);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "K Q R B N P ");

	player->onCaptured(WPawn);

	EXPECT_EQ(player->getCapturedList(), "K Q R B N P P ");

	player->onCaptured(WPawn);

	EXPECT_EQ(player->getCapturedList(), "K Q R B N P P P ");

	player->onCaptured(WPawn);

	EXPECT_EQ(player->getCapturedList(), "K Q R B N P P P P ");

	player->onCaptured(WPawn);

	EXPECT_EQ(player->getCapturedList(), "K Q R B N P P P P P ");

	player->onCaptured(WPawn);

	EXPECT_EQ(player->getCapturedList(), "K Q R B N P P P P P P ");

	player->onCaptured(WPawn);

	EXPECT_EQ(player->getCapturedList(), "K Q R B N P P P P P P P ");

	player->onCaptured(WPawn);

	EXPECT_EQ(player->getCapturedList(), "K Q R B N P P P P P P P P ");

	player->onCaptured(WRook);
	player->onCaptured(WBishop);
	player->onCaptured(WKnight);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 0);
	EXPECT_EQ(player->pieces[WQueen], 0);
	EXPECT_EQ(player->pieces[WRook], 0);
	EXPECT_EQ(player->pieces[WBishop], 0);
	EXPECT_EQ(player->pieces[WKnight], 0);
	EXPECT_EQ(player->pieces[WPawn], 0);
	EXPECT_EQ(player->getCapturedList(), "K Q R R B B N N P P P P P P P P ");
}

TEST(PlayerTest, onCapturedBlackTest) {
	Player* player = new Player(false);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 2);
	EXPECT_EQ(player->pieces[WBishop], 2);
	EXPECT_EQ(player->pieces[WKnight], 2);
	EXPECT_EQ(player->pieces[WPawn], 8);
	GTEST_ASSERT_TRUE(player->getCapturedList().empty());

	player->onCaptured(BPawn);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 2);
	EXPECT_EQ(player->pieces[WBishop], 2);
	EXPECT_EQ(player->pieces[WKnight], 2);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "p ");

	player->onCaptured(BKnight);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 2);
	EXPECT_EQ(player->pieces[WBishop], 2);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "n p ");

	player->onCaptured(BBishop);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 2);
	EXPECT_EQ(player->pieces[WBishop], 1);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "b n p ");

	player->onCaptured(BRook);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 1);
	EXPECT_EQ(player->pieces[WRook], 1);
	EXPECT_EQ(player->pieces[WBishop], 1);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "r b n p ");

	player->onCaptured(BQueen);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 1);
	EXPECT_EQ(player->pieces[WQueen], 0);
	EXPECT_EQ(player->pieces[WRook], 1);
	EXPECT_EQ(player->pieces[WBishop], 1);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "q r b n p ");

	player->onCaptured(BKing);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 0);
	EXPECT_EQ(player->pieces[WQueen], 0);
	EXPECT_EQ(player->pieces[WRook], 1);
	EXPECT_EQ(player->pieces[WBishop], 1);
	EXPECT_EQ(player->pieces[WKnight], 1);
	EXPECT_EQ(player->pieces[WPawn], 7);
	EXPECT_EQ(player->getCapturedList(), "k q r b n p ");

	player->onCaptured(BPawn);

	EXPECT_EQ(player->getCapturedList(), "k q r b n p p ");

	player->onCaptured(BPawn);

	EXPECT_EQ(player->getCapturedList(), "k q r b n p p p ");

	player->onCaptured(BPawn);

	EXPECT_EQ(player->getCapturedList(), "k q r b n p p p p ");

	player->onCaptured(BPawn);

	EXPECT_EQ(player->getCapturedList(), "k q r b n p p p p p ");

	player->onCaptured(BPawn);

	EXPECT_EQ(player->getCapturedList(), "k q r b n p p p p p p ");

	player->onCaptured(BPawn);

	EXPECT_EQ(player->getCapturedList(), "k q r b n p p p p p p p ");

	player->onCaptured(BPawn);

	EXPECT_EQ(player->getCapturedList(), "k q r b n p p p p p p p p ");

	player->onCaptured(BRook);
	player->onCaptured(BBishop);
	player->onCaptured(BKnight);

	EXPECT_EQ(player->pieces[Empty], 0);
	EXPECT_EQ(player->pieces[WKing], 0);
	EXPECT_EQ(player->pieces[WQueen], 0);
	EXPECT_EQ(player->pieces[WRook], 0);
	EXPECT_EQ(player->pieces[WBishop], 0);
	EXPECT_EQ(player->pieces[WKnight], 0);
	EXPECT_EQ(player->pieces[WPawn], 0);
	EXPECT_EQ(player->getCapturedList(), "k q r r b b n n p p p p p p p p ");
}

TEST(PlayerTest, moveTimeTest) {
	Player* player = new Player(true);
	player->startMoveTime();
	this_thread::sleep_for(chrono::milliseconds(100));
	player->stopMoveTime();

	EXPECT_LT(player->gameTime, 150);
	EXPECT_GT(player->gameTime, 50);
}