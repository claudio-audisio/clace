#include <gtest/gtest.h>

#include "../move/movesCache.h"

using namespace std;


TEST(MovesCacheTest, test) {
    MovesCache cache(10);
    vector<Move> moves;
    moves.push_back(0);
    moves.push_back(1);
    moves.push_back(2);
    moves.push_back(3);
    moves.push_back(4);
    cache.add("game", moves);

    EXPECT_EQ(cache.size(), 1);

    cache.add("game2", moves);

    EXPECT_EQ(cache.size(), 2);

    moves.clear();

    vector<Move> cacheMoves;
    vector<Move> cacheMoves2;

    EXPECT_EQ(cache.get("wrong", cacheMoves), false);
    EXPECT_EQ(cache.get("game", cacheMoves), true);
    EXPECT_EQ(cache.get("game2", cacheMoves2), true);
    EXPECT_EQ(cacheMoves.size(), 5);
    EXPECT_EQ(cacheMoves2.size(), 5);

    cacheMoves.clear();

    EXPECT_EQ(cacheMoves2.size(), 5);
}