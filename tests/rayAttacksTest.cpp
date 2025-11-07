#include <gtest/gtest.h>

#include "../utils/fen.h"
#include "testUtils.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;


class RayAttacksTest : public testing::Test
{
protected:
    RayAttacksTest() {
        initAttacks();
        initDestPosProviders();
        initPawnAttacksProviders();
    }
    ~RayAttacksTest() {

    }
};

TEST_F(RayAttacksTest, attackTest1) {
    Game* game = FEN::fenToNewGame("4k3/8/8/8/3Q4/8/8/4K3 w - - 0 1");
    const Rawboard occupied = ~game->board->pieceBoards[Empty];

    GTEST_ASSERT_TRUE(checkBoard(northAttack(occupied, 35), 3, 11, 19, 27));
    GTEST_ASSERT_TRUE(checkBoard(noEastAttack(occupied, 35), 7, 14, 21, 28));
    GTEST_ASSERT_TRUE(checkBoard(eastAttack(occupied, 35), 36, 37, 38, 39));
    GTEST_ASSERT_TRUE(checkBoard(soEastAttack(occupied, 35), 44, 53, 62));
    GTEST_ASSERT_TRUE(checkBoard(southAttack(occupied, 35), 43, 51, 59));
    GTEST_ASSERT_TRUE(checkBoard(soWestAttack(occupied, 35), 42, 49, 56));
    GTEST_ASSERT_TRUE(checkBoard(westAttack(occupied, 35), 34, 33, 32));
    GTEST_ASSERT_TRUE(checkBoard(noWestAttack(occupied, 35), 8, 17, 26));

	delete game;
}

TEST_F(RayAttacksTest, attackTest2) {
    Game* game = FEN::fenToNewGame("4k3/8/1P1p1p2/8/1p1Q1P2/8/1P1P1p2/4K3 w - - 0 1");
    const Rawboard occupied = ~game->board->pieceBoards[Empty];

    GTEST_ASSERT_TRUE(checkBoard(northAttack(occupied, 35), 19, 27));
    GTEST_ASSERT_TRUE(checkBoard(noEastAttack(occupied, 35), 21, 28));
    GTEST_ASSERT_TRUE(checkBoard(eastAttack(occupied, 35), 36, 37));
    GTEST_ASSERT_TRUE(checkBoard(soEastAttack(occupied, 35), 44, 53));
    GTEST_ASSERT_TRUE(checkBoard(southAttack(occupied, 35), 43, 51));
    GTEST_ASSERT_TRUE(checkBoard(soWestAttack(occupied, 35), 42, 49));
    GTEST_ASSERT_TRUE(checkBoard(westAttack(occupied, 35), 34, 33));
    GTEST_ASSERT_TRUE(checkBoard(noWestAttack(occupied, 35), 17, 26));

	delete game;
}

TEST_F(RayAttacksTest,  attackTest3) {
    Game* game = FEN::fenToNewGame("4k3/p2P2P1/1P1p1p2/8/Pp1Q1Pp1/8/1P1P1p2/p2pK1P1 w - - 0 1");
    const Rawboard occupied = ~game->board->pieceBoards[Empty];

    GTEST_ASSERT_TRUE(checkBoard(northAttack(occupied, 35), 19, 27));
    GTEST_ASSERT_TRUE(checkBoard(noEastAttack(occupied, 35), 21, 28));
    GTEST_ASSERT_TRUE(checkBoard(eastAttack(occupied, 35), 36, 37));
    GTEST_ASSERT_TRUE(checkBoard(soEastAttack(occupied, 35), 44, 53));
    GTEST_ASSERT_TRUE(checkBoard(southAttack(occupied, 35), 43, 51));
    GTEST_ASSERT_TRUE(checkBoard(soWestAttack(occupied, 35), 42, 49));
    GTEST_ASSERT_TRUE(checkBoard(westAttack(occupied, 35), 34, 33));
    GTEST_ASSERT_TRUE(checkBoard(noWestAttack(occupied, 35), 17, 26));

	delete game;
}

TEST_F(RayAttacksTest, attackTest4) {
    Game* game = FEN::fenToNewGame("4k3/p2P2P1/8/2Ppp3/P1pQP1p1/2PPp3/8/p2pK1P1 w - - 0 1");
    const Rawboard occupied = ~game->board->pieceBoards[Empty];

    GTEST_ASSERT_TRUE(checkBoard(northAttack(occupied, 35), 27));
    GTEST_ASSERT_TRUE(checkBoard(noEastAttack(occupied, 35), 28));
    GTEST_ASSERT_TRUE(checkBoard(eastAttack(occupied, 35), 36));
    GTEST_ASSERT_TRUE(checkBoard(soEastAttack(occupied, 35), 44));
    GTEST_ASSERT_TRUE(checkBoard(southAttack(occupied, 35), 43));
    GTEST_ASSERT_TRUE(checkBoard(soWestAttack(occupied, 35), 42));
    GTEST_ASSERT_TRUE(checkBoard(westAttack(occupied, 35), 34));
    GTEST_ASSERT_TRUE(checkBoard(noWestAttack(occupied, 35), 26));

	delete game;
}