#include <gtest/gtest.h>

#include "../cache/transpositionTable.h"

using namespace std;


class TranspositionTableTest : public testing::Test {
protected:
    TranspositionTableTest() {
        initMovesGenerator();
    }
    ~TranspositionTableTest() {

    }
};

TEST_F(TranspositionTableTest, tableTest) {
#ifndef USE_CACHE
    GTEST_SKIP();
#endif
    TranspositionTable table;

    Game game1, game2, game3;
    game1.init();
    game2.init();
    game3.init();

    Move *moves1 = new Move[216];
    Move *moves2 = new Move[216];
    Move *moves1g;
    Move *moves2g;
    MovesAmount amount1, amount2, amount1g, amount2g;

    table.getMoves(game1, moves1, amount1);

    EXPECT_EQ(table.size(), 1);

    table.getMoves(game2, moves2, amount2);

    EXPECT_EQ(table.size(), 2);

    table.getMoves(game1, moves1g, amount1g);

    EXPECT_EQ(table.size(), 2);
    EXPECT_EQ(*moves1, *moves1g);
    EXPECT_EQ(amount1.total, amount1g.total);
    EXPECT_EQ(amount1.legal, amount1g.legal);

    table.getMoves(game2, moves2g, amount2g);

    EXPECT_EQ(table.size(), 2);
    EXPECT_EQ(*moves2, *moves2g);
    EXPECT_EQ(amount2.total, amount2g.total);
    EXPECT_EQ(amount2.legal, amount2g.legal);

    table.remove(game1.key);

    EXPECT_EQ(table.size(), 1);

    table.remove(game3.key);

    EXPECT_EQ(table.size(), 1);

    table.clear();

    EXPECT_EQ(table.size(), 0);
}