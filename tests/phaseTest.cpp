#include <gtest/gtest.h>

#include "testUtils.h"
#include "../game/game.h"
#include "../board/board.h"
#include "../evaluation/phase.h"
#include "../utils/fen.h"

using namespace std;

#define EPS 0.01

TEST(phaseTest, phaseBoundsTest) {
    Game game;
    game.init();

    EXPECT_EQ(Phase::calculate(game.board), 0.5);

    FEN::fenToGame(END_FEN_POSITION, game);

    EXPECT_EQ(Phase::calculate(game.board), 256.5);
}

TEST(phaseTest, taperedEvalTest) {
    Game game;
    game.init();

    EXPECT_EQ(Phase::taperedEval(game.board, 0, 0), 0.0);
    EXPECT_NEAR(Phase::taperedEval(game.board, 0, 1), 0.0, EPS);
    EXPECT_NEAR(Phase::taperedEval(game.board, 1, 0), 1.0, EPS);

    FEN::fenToGame(END_FEN_POSITION, game);

    EXPECT_EQ(Phase::taperedEval(game.board, 0, 0), 0.0);
    EXPECT_NEAR(Phase::taperedEval(game.board, 0, 1), 1.0, EPS);
    EXPECT_NEAR(Phase::taperedEval(game.board, 1, 0), 0.0, EPS);

    FEN::fenToGame(PERFT_FEN_POSITION_3, game);
    cout << Phase::taperedEval(game.board, 0, 1) << endl;
}