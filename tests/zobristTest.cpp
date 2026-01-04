#include "gtest/gtest.h"

#include "../cache/zobrist.h"
#include "../movesCalculation/staticAttacks.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;

class ZobristTest : public testing::Test {
protected:
    ZobristTest() {
        initAttacks();
        initDestPosProviders();
        initPawnAttacksProviders();
    }
    ~ZobristTest() {

    }
};

TEST_F(ZobristTest, getZobristKeyTest) {
    Game game;
    game.init();

    const unsLL index1 = getZobristKey(game);
    const unsLL index2 = getZobristKey(game);

    ASSERT_EQ(index1, index2);
}

