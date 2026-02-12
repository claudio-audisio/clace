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
    auto *table = new TranspositionTable(1);

    Game game1, game2, game3;
    game1.init();
    game2.init();
    game3.init();

    Move *moves1;
    Move *moves2;
    Move *moves1g;
    Move *moves2g;

    const unsigned int amount1 = table->getMoves(game1, moves1);

    EXPECT_EQ(table->size(), 1);

    const unsigned int amount2 = table->getMoves(game2, moves2);

    EXPECT_EQ(table->size(), 2);

    const unsigned int amount1g = table->getMoves(game1, moves1g);

    EXPECT_EQ(table->size(), 2);
    EXPECT_EQ(*moves1, *moves1g);
    EXPECT_EQ(amount1, amount1g);
    EXPECT_EQ(amount1, amount1g);

    const unsigned int amount2g = table->getMoves(game2, moves2g);

    EXPECT_EQ(table->size(), 2);
    EXPECT_EQ(*moves2, *moves2g);
    EXPECT_EQ(amount2, amount2g);
    EXPECT_EQ(amount2, amount2g);

    table->remove(game1.key);

    EXPECT_EQ(table->size(), 1);

    table->remove(game3.key);

    EXPECT_EQ(table->size(), 1);

    table->clear();

    EXPECT_EQ(table->size(), 0);
}