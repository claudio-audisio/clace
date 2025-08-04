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

TEST(MovesCacheTest, cacheNewTest) {
    MovesCache cache(10);
    Move* moves = new Move[5];
    moves[0]= 0;
    moves[1]= 1;
    moves[2]= 2;
    moves[3]= 3;
    moves[4]= 4;
    cache.addNew("game", moves, 5, 1);

    EXPECT_EQ(cache.sizeNew(), 1);

    cache.addNew("game2", moves, 5, 1);

    EXPECT_EQ(cache.sizeNew(), 2);

    delete moves;

    Move* cacheMoves = new Move[5];
    Move* cacheMoves2 = new Move[5];
    pair<unsigned int, unsigned int> res;
    pair<unsigned int, unsigned int> res2;

    EXPECT_EQ(cache.getNew("wrong", cacheMoves, res), false);
    EXPECT_EQ(cache.getNew("game", cacheMoves, res), true);
    EXPECT_EQ(cache.getNew("game2", cacheMoves2, res2), true);
    EXPECT_EQ(cacheMoves[4], 4);
    EXPECT_EQ(res.first, 5);
    EXPECT_EQ(res.second, 1);
    EXPECT_EQ(cacheMoves2[4], 4);
    EXPECT_EQ(res2.first, 5);
    EXPECT_EQ(res2.second, 1);

    delete cacheMoves;

    EXPECT_EQ(cacheMoves2[4], 4);
    EXPECT_EQ(res2.first, 5);
    EXPECT_EQ(res2.second, 1);
}