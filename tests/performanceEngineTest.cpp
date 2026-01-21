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
#include "../engine/bf_engine.h"
#include "../common/defines.h"
#include "../move/rollback.h"
#include "../cache/transpositionTable.h"
#include "engine/ab_engine.h"

using namespace std;

#if RELEASE_MODE


class PerformanceEngineTest : public testing::Test {
protected:
    PerformanceEngineTest() {
        initMovesGenerator();
    }

    ~PerformanceEngineTest() override {

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

//******** BFEngine ****************************************

TEST_F(PerformanceEngineTest, BFEngineOpenGameDepth4Test) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    Game game;
    game.initFromFEN(INITIAL_FEN_POSITION);
    auto engine = new BF_Engine(4);
    engine->setEvaluator(new BasicEvaluator());
    auto moves = new Move[MAX_MOVES];
    MovesAmount amount;

    auto begin = chrono::steady_clock::now();

    generateLegalMoves(game, moves, &amount);
    engine->_calculateMove(game, moves, amount);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 280);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, BFEngineMidGameDepth3Test) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_2);
    auto engine = new BF_Engine(3);
    engine->setEvaluator(new BasicEvaluator());
    auto moves = new Move[MAX_MOVES];
    MovesAmount amount;

    auto begin = chrono::steady_clock::now();

    generateLegalMoves(game, moves, &amount);
    engine->_calculateMove(game, moves, amount);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 200);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, BFEngineMidGame2Depth3Test) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_6);
    auto engine = new BF_Engine(3);
    engine->setEvaluator(new BasicEvaluator());
    auto moves = new Move[MAX_MOVES];
    MovesAmount amount;

    auto begin = chrono::steady_clock::now();

    generateLegalMoves(game, moves, &amount);
    engine->_calculateMove(game, moves, amount);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 180);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, BFEngineEndGameDepth5Test) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_3);
    auto engine = new BF_Engine(5);
    auto moves = new Move[MAX_MOVES];
    MovesAmount amount;

    auto begin = chrono::steady_clock::now();

    generateLegalMoves(game, moves, &amount);
    engine->_calculateMove(game, moves, amount);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 570);
    cout << "time: " << time  << endl;
}

//******** ABEngine ****************************************

TEST_F(PerformanceEngineTest, ABEngineOpenGameDepth4Test) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    Game game;
    game.initFromFEN(INITIAL_FEN_POSITION);
    auto engine = new AB_Engine(4);
    engine->setEvaluator(new BasicEvaluator());
    auto moves = new Move[MAX_MOVES];
    MovesAmount amount;

    auto begin = chrono::steady_clock::now();

    generateLegalMoves(game, moves, &amount);
    engine->_calculateMove(game, moves, amount);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 70);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, ABEngineMidGameDepth3Test) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_2);
    auto engine = new AB_Engine(3);
    engine->setEvaluator(new BasicEvaluator());
    auto moves = new Move[MAX_MOVES];
    MovesAmount amount;

    auto begin = chrono::steady_clock::now();

    generateLegalMoves(game, moves, &amount);
    engine->_calculateMove(game, moves, amount);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 80);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, ABEngineMidGame2Depth3Test) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_6);
    auto engine = new AB_Engine(3);
    engine->setEvaluator(new BasicEvaluator());
    auto moves = new Move[MAX_MOVES];
    MovesAmount amount;

    auto begin = chrono::steady_clock::now();

    generateLegalMoves(game, moves, &amount);
    engine->_calculateMove(game, moves, amount);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 100);
    cout << "time: " << time  << endl;
}

TEST_F(PerformanceEngineTest, ABEngineEndGameDepth5Test) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    Game game;
    game.initFromFEN(PERFT_FEN_POSITION_3);
    auto engine = new AB_Engine(5);
    auto moves = new Move[MAX_MOVES];
    MovesAmount amount;

    auto begin = chrono::steady_clock::now();

    generateLegalMoves(game, moves, &amount);
    engine->_calculateMove(game, moves, amount);

    unsLL time = getElapsedMillis(begin);
    GTEST_ASSERT_NEAR(time, 40);
    cout << "time: " << time  << endl;
}


#endif
