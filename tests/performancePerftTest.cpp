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
    Perft *perft;

    PerformancePerftTest() {
        initMovesGenerator();
    }

    ~PerformancePerftTest() override {
        delete perft;
    }

    void static GTEST_ASSERT_NEAR(unsLL value, unsLL expected) {
        unsLL delta = expected * 10 / 100;

        if (delta < 10) {
            delta = 10;
        }

        GTEST_ASSERT_LE(value, expected + delta);
        GTEST_ASSERT_GE(value, expected - delta);
    }
};

#ifdef PERFORMANCE_TESTS

TEST_F(PerformancePerftTest, InitPerft5BulkTest) {
    perft = new Perft(INITIAL_FEN_POSITION, 5);

    auto result = perft->runBulk();

    GTEST_ASSERT_NEAR(result->getElapsed(), 120);

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, InitPerft6BulkTest) {
    perft = new Perft(INITIAL_FEN_POSITION, 6);

    auto result = perft->runBulk();

    GTEST_ASSERT_NEAR(result->getElapsed(), 2700);

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, Pos2Perft5BulkTest) {
    perft = new Perft(PERFT_FEN_POSITION_2, 5);

    auto result = perft->runBulk();

    GTEST_ASSERT_NEAR(result->getElapsed(), 3610);

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, Pos4Perft5BulkTest) {
    perft = new Perft(PERFT_FEN_POSITION_4, 5);

    auto result = perft->runBulk();

    GTEST_ASSERT_NEAR(result->getElapsed(), 460);

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, InitPerft4CompleteTest) {
    perft = new Perft(INITIAL_FEN_POSITION, 4);

    auto result = perft->runComplete();

    GTEST_ASSERT_NEAR(result->getElapsed(), 150);

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, InitPerft5CompleteTest) {
    perft = new Perft(INITIAL_FEN_POSITION, 5);

    auto result = perft->runComplete();

    GTEST_ASSERT_NEAR(result->getElapsed(), 3170);

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, Pos2Perft4CompleteTest) {
    perft = new Perft(PERFT_FEN_POSITION_2, 4);

    auto result = perft->runComplete();

    GTEST_ASSERT_NEAR(result->getElapsed(), 4200);

    cout << "time: " << result->getElapsed()  << endl;
}

TEST_F(PerformancePerftTest, Pos4Perft4CompleteTest) {
    perft = new Perft(PERFT_FEN_POSITION_4, 4);

    auto result = perft->runComplete();

    GTEST_ASSERT_NEAR(result->getElapsed(), 550);

    cout << "time: " << result->getElapsed()  << endl;
}

#endif
#endif
