#include <gtest/gtest.h>
#include <chrono>

#include "../board/board.h"
#include "../cache/zobrist.h"
#include "../utils/fen.h"
#include "../utils/boardUtils.h"
#include "../utils/positions.h"
#include "../utils/utils.h"
#include "../move/movesGenerator.h"
#include "../move/move.h"
#include "../perft/perft.h"
#include "../engine/bruteForceEngine.h"
#include "../common/defines.h"
#include "../move/rollback.h"
#include "../cache/transpositionTable.h"
#include "engine/alphaBetaEngine.h"

using namespace std;

#if RELEASE_MODE


class PerformanceEngineTest : public testing::Test {
protected:
    AbstractEngine *engine;

    PerformanceEngineTest() {
        initMovesGenerator();
    }

    ~PerformanceEngineTest() override {
        delete engine;
    }

    void static GTEST_ASSERT_NEAR(unsLL value, unsLL expected) {
        unsLL delta = expected * 5 / 100;

        if (delta < 10) {
            delta = 10;
        }

        GTEST_ASSERT_LE(value, expected + delta);
        GTEST_ASSERT_GE(value, expected - delta);
    }
};

#ifdef PERFORMANCE_TESTS

//******** BFEngine ****************************************

TEST_F(PerformanceEngineTest, BFEngineOpenGameDepth4Test) {
    Game game;
    game.initFromFEN(INITIAL_FEN_POSITION);
    engine = new BruteForceEngine(4);

    auto begin = chrono::steady_clock::now();

    engine->_calculateMove(game);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 165);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, BFEngineMidGameDepth3Test) {
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_2);
    engine = new BruteForceEngine(3);

    auto begin = chrono::steady_clock::now();

    engine->_calculateMove(game);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 125);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, BFEngineMidGame2Depth3Test) {
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_6);
    engine = new BruteForceEngine(3);

    auto begin = chrono::steady_clock::now();

    engine->_calculateMove(game);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 110);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, BFEngineEndGameDepth5Test) {
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_3);
    engine = new BruteForceEngine(5);

    auto begin = chrono::steady_clock::now();

    engine->_calculateMove(game);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 250);
    cout << "time: " << time  << endl;
}

//******** ABEngine ****************************************

TEST_F(PerformanceEngineTest, ABEngineOpenGameDepth5Test) {
    Game game;
    game.initFromFEN(INITIAL_FEN_POSITION);
    engine = new AlphaBetaEngine(5);

    auto begin = chrono::steady_clock::now();

    engine->_calculateMove(game);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 200);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, ABEngineMidGameDepth4Test) {
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_2);
    engine = new AlphaBetaEngine(4);

    auto begin = chrono::steady_clock::now();

    engine->_calculateMove(game);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 60);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, ABEngineMidGame2Depth4Test) {
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_6);
    engine = new AlphaBetaEngine(4);

    auto begin = chrono::steady_clock::now();

    engine->_calculateMove(game);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 210);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, ABEngineEndGameDepth6Test) {
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_3);
    engine = new AlphaBetaEngine(6);

    auto begin = chrono::steady_clock::now();

    engine->_calculateMove(game);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 35);
    cout << "time: " << time  << endl;
}

#endif
#endif
