#include <gtest/gtest.h>

#include "../move/movesCache.h"

using namespace std;


class MovesCacheTest : public testing::Test {
protected:
    MovesCacheTest() {

    }
    ~MovesCacheTest() {

    }
};

TEST(MovesCacheTest, cacheTest) {
    MovesCache cache(10);
    Move* moves = new Move[5];
    moves[0]= 0;
    moves[1]= 1;
    moves[2]= 2;
    moves[3]= 3;
    moves[4]= 4;

    cache.add("game", moves, 5, 1);

    EXPECT_EQ(cache.sizeNew(), 1);

    cache.add("game2", moves, 5, 1);

    EXPECT_EQ(cache.sizeNew(), 2);

    delete[] moves;

    Move* cacheMoves = new Move[5];
    Move* cacheMoves2 = new Move[5];
    MovesAmount amount;
    MovesAmount amount2;

    EXPECT_EQ(cache.get("wrong", cacheMoves, amount), false);
    EXPECT_EQ(cache.get("game", cacheMoves, amount), true);
    EXPECT_EQ(cache.get("game2", cacheMoves2, amount2), true);
    EXPECT_NE(cacheMoves, cacheMoves2);
    EXPECT_EQ(cacheMoves[4], 4);
    EXPECT_EQ(amount.first, 5);
    EXPECT_EQ(amount.second, 1);
    EXPECT_EQ(cacheMoves2[4], 4);
    EXPECT_EQ(amount2.first, 5);
    EXPECT_EQ(amount2.second, 1);
}