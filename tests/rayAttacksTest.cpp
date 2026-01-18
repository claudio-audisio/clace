#include <gtest/gtest.h>

#include "../utils/fen.h"
#include "testUtils.h"
#include "../move/movesCalculation.h"
#include "move/movesGenerator.h"

using namespace std;


class RayAttacksTest : public testing::Test
{
protected:
    RayAttacksTest() {
        initMovesGenerator();
    }
    ~RayAttacksTest() {

    }
};

TEST_F(RayAttacksTest, attackTest1) {
    Game* game = FEN::fenToNewGame("4k3/8/8/8/3Q4/8/8/4K3 w - - 0 1");
    const Rawboard occupied = ~game->board->pieceBoards[Empty];

    GTEST_ASSERT_TRUE(checkBoard(southAttack(occupied, 27), 3, 11, 19));
    GTEST_ASSERT_TRUE(checkBoard(noEastAttack(occupied, 27), 36, 45, 54, 63));
    GTEST_ASSERT_TRUE(checkBoard(eastAttack(occupied, 27), 28, 29, 30, 31));
    GTEST_ASSERT_TRUE(checkBoard(soEastAttack(occupied, 27), 20, 13, 6));
    GTEST_ASSERT_TRUE(checkBoard(northAttack(occupied, 27), 35, 43, 51, 59));
    GTEST_ASSERT_TRUE(checkBoard(soWestAttack(occupied, 27), 0, 9, 18));
    GTEST_ASSERT_TRUE(checkBoard(westAttack(occupied, 27), 24, 25, 26));
    GTEST_ASSERT_TRUE(checkBoard(noWestAttack(occupied, 27), 34, 41, 48));

	delete game;
}

TEST_F(RayAttacksTest, attackTest2) {
    Game* game = FEN::fenToNewGame("4k3/8/1P1p1p2/8/1p1Q1P2/8/1P1P1p2/4K3 w - - 0 1");
    const Rawboard occupied = ~game->board->pieceBoards[Empty];

    GTEST_ASSERT_TRUE(checkBoard(northAttack(occupied, 27), 35, 43));
    GTEST_ASSERT_TRUE(checkBoard(noEastAttack(occupied, 27), 36, 45));
    GTEST_ASSERT_TRUE(checkBoard(eastAttack(occupied, 27), 28, 29));
    GTEST_ASSERT_TRUE(checkBoard(soEastAttack(occupied, 27), 20, 13));
    GTEST_ASSERT_TRUE(checkBoard(southAttack(occupied, 27), 19, 11));
    GTEST_ASSERT_TRUE(checkBoard(soWestAttack(occupied, 27), 18, 9));
    GTEST_ASSERT_TRUE(checkBoard(westAttack(occupied, 27), 25, 26));
    GTEST_ASSERT_TRUE(checkBoard(noWestAttack(occupied, 27), 34, 41));

	delete game;
}

TEST_F(RayAttacksTest,  attackTest3) {
    Game* game = FEN::fenToNewGame("4k3/p2P2P1/1P1p1p2/8/Pp1Q1Pp1/8/1P1P1p2/p2pK1P1 w - - 0 1");
    const Rawboard occupied = ~game->board->pieceBoards[Empty];

    GTEST_ASSERT_TRUE(checkBoard(northAttack(occupied, 27), 35, 43));
    GTEST_ASSERT_TRUE(checkBoard(noEastAttack(occupied, 27), 36, 45));
    GTEST_ASSERT_TRUE(checkBoard(eastAttack(occupied, 27), 28, 29));
    GTEST_ASSERT_TRUE(checkBoard(soEastAttack(occupied, 27), 20, 13));
    GTEST_ASSERT_TRUE(checkBoard(southAttack(occupied, 27), 19, 11));
    GTEST_ASSERT_TRUE(checkBoard(soWestAttack(occupied, 27), 18, 9));
    GTEST_ASSERT_TRUE(checkBoard(westAttack(occupied, 27), 25, 26));
    GTEST_ASSERT_TRUE(checkBoard(noWestAttack(occupied, 27), 34, 41));

	delete game;
}

TEST_F(RayAttacksTest, attackTest4) {
    Game* game = FEN::fenToNewGame("4k3/p2P2P1/8/2Ppp3/P1pQP1p1/2PPp3/8/p2pK1P1 w - - 0 1");
    const Rawboard occupied = ~game->board->pieceBoards[Empty];

    GTEST_ASSERT_TRUE(checkBoard(northAttack(occupied, 27), 35));
    GTEST_ASSERT_TRUE(checkBoard(noEastAttack(occupied, 27), 36));
    GTEST_ASSERT_TRUE(checkBoard(eastAttack(occupied, 27), 28));
    GTEST_ASSERT_TRUE(checkBoard(soEastAttack(occupied, 27), 20));
    GTEST_ASSERT_TRUE(checkBoard(southAttack(occupied, 27), 19));
    GTEST_ASSERT_TRUE(checkBoard(soWestAttack(occupied, 27), 18));
    GTEST_ASSERT_TRUE(checkBoard(westAttack(occupied, 27), 26));
    GTEST_ASSERT_TRUE(checkBoard(noWestAttack(occupied, 27), 34));

	delete game;
}