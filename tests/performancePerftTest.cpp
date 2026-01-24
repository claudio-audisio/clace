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

using namespace std;

#if RELEASE_MODE


class PerformancePerftTest : public testing::Test {
protected:
    PerformancePerftTest() {
        initMovesGenerator();
    }

    ~PerformancePerftTest() override {

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

TEST_F(PerformancePerftTest, InitPerft5BulkTest) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    auto perft = new Perft(INITIAL_FEN_POSITION, 5);

    auto result = perft->runBulk();

#ifdef USE_CACHE
    GTEST_ASSERT_NEAR(result->getElapsed(), 160);
#else
    GTEST_ASSERT_NEAR(result->getElapsed(), 180);
#endif

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, InitPerft6BulkTest) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    auto perft = new Perft(INITIAL_FEN_POSITION, 6);

    auto result = perft->runBulk();

#ifdef USE_CACHE
    GTEST_ASSERT_NEAR(result->getElapsed(), 3300);
#else
    GTEST_ASSERT_NEAR(result->getElapsed(), 4400);
#endif

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, Pos2Perft5BulkTest) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    auto perft = new Perft(PERFT_FEN_POSITION_2, 5);

    auto result = perft->runBulk();

#ifdef USE_CACHE
    GTEST_ASSERT_NEAR(result->getElapsed(), 4800);
#else
    GTEST_ASSERT_NEAR(result->getElapsed(), 7200);
#endif

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, Pos4Perft5BulkTest) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    auto perft = new Perft(PERFT_FEN_POSITION_4, 5);

    auto result = perft->runBulk();

#ifdef USE_CACHE
    GTEST_ASSERT_NEAR(result->getElapsed(), 640);
#else
    GTEST_ASSERT_NEAR(result->getElapsed(), 660);
#endif

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, InitPerft4CompleteTest) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    auto perft = new Perft(INITIAL_FEN_POSITION, 4);

    auto result = perft->runComplete();

#ifdef USE_CACHE
    GTEST_ASSERT_NEAR(result->getElapsed(), 215);
#else
    GTEST_ASSERT_NEAR(result->getElapsed(), 220);
#endif

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, InitPerft5CompleteTest) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    auto perft = new Perft(INITIAL_FEN_POSITION, 5);

    auto result = perft->runComplete();

#ifdef USE_CACHE
    GTEST_ASSERT_NEAR(result->getElapsed(), 4300);
#else
    GTEST_ASSERT_NEAR(result->getElapsed(), 5400);
#endif

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, Pos2Perft4CompleteTest) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    auto perft = new Perft(PERFT_FEN_POSITION_2, 4);

    auto result = perft->runComplete();

#ifdef USE_CACHE
    GTEST_ASSERT_NEAR(result->getElapsed(), 5700);
#else
    GTEST_ASSERT_NEAR(result->getElapsed(), 8200);
#endif

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, Pos4Perft4CompleteTest) {
#ifndef PERFORMANCE_TESTS
    GTEST_SKIP();
#endif
    auto perft = new Perft(PERFT_FEN_POSITION_4, 4);

    auto result = perft->runComplete();

#ifdef USE_CACHE
    GTEST_ASSERT_NEAR(result->getElapsed(), 720);
#else
    GTEST_ASSERT_NEAR(result->getElapsed(), 760);
#endif

    cout << "time: " << result->getElapsed()  << endl;
}

#endif
