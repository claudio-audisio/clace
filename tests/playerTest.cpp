#include <gtest/gtest.h>
#include <chrono>
#include <thread>

#include "../game/player.h"

using namespace std;

TEST(PlayerTest, moveTimeTest) {
	Player* player = new Player(true);
	player->startMoveTime();
	this_thread::sleep_for(chrono::milliseconds(100));
	player->stopMoveTime();

	EXPECT_LT(player->gameTime, 150);
	EXPECT_GT(player->gameTime, 50);
}