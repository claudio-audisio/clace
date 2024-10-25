#include <gtest/gtest.h>
#include <chrono>
#include <numeric>

#include "../utils/positions.h"
#include "../utils/fen.h"
#include "../utils/utils.h"

using namespace std;


class IndexToCoordsTest : public ::testing::TestWithParam<tuple<Position, string>> {};

TEST_P(IndexToCoordsTest, indexToCoordsTest) {
    Position index = get<0>(GetParam());
    string expectedCoordinates = get<1>(GetParam());
    EXPECT_EQ(Positions::indexToCoords(index), expectedCoordinates);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IndexToCoordsTest,
    ::testing::Values(
        make_tuple(0, "a8"),
        make_tuple(56, "a1"),
        make_tuple(7, "h8"),
        make_tuple(63, "h1"),
        make_tuple(24, "a5"),
        make_tuple(31, "h5"),
        make_tuple(32, "a4"),
        make_tuple(39, "h4"),
        make_tuple(27, "d5"),
        make_tuple(35, "d4"),
        make_tuple(28, "e5"),
        make_tuple(36, "e4")
    )
);


class CoordsToIndexTest : public ::testing::TestWithParam<tuple<string, Position>> {};

TEST_P(CoordsToIndexTest, coordsToIndexTest) {   
    string coordinates = get<0>(GetParam());
    Position expectedResult = get<1>(GetParam());
    EXPECT_EQ(Positions::coordsToIndex(coordinates), expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    CoordsToIndexTest,
    ::testing::Values(
        make_tuple("a8", 0),
        make_tuple("a1", 56),
        make_tuple("h8", 7),
        make_tuple("h1", 63),
        make_tuple("a5", 24),
        make_tuple("h5", 31),
        make_tuple("a4", 32),
        make_tuple("h4", 39),
        make_tuple("d5", 27),
        make_tuple("d4", 35),
        make_tuple("e5", 28),
        make_tuple("e4", 36)
    )
);


class TestParams {
public:
    TestParams(Position position, bool white, bool expectedResult) {
        this->position = position;
        this->white = white;
        this->expectedResult = expectedResult;
    }

    Position position;
    bool white;
    bool expectedResult;
};

class IsFirstRowTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(IsFirstRowTest, isFirstRowTest) {
    TestParams* params = GetParam();
    EXPECT_EQ(Positions::isFirstRow(params->position, params->white), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsFirstRowTest,
    ::testing::Values(
        new TestParams(0, true, false),
        new TestParams(0, false, true),
        new TestParams(7, true, false),
        new TestParams(7, false, true),
        new TestParams(8, true, false),
        new TestParams(8, false, false),
        new TestParams(55, true, false),
        new TestParams(55, false, false),
        new TestParams(56, true, true),
        new TestParams(56, false, false),
        new TestParams(63, true, true),
        new TestParams(63, false, false)
    )
);


class IsSecondRowTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(IsSecondRowTest, isSecondRowTest) {
    TestParams* params = GetParam();
    EXPECT_EQ(Positions::isSecondRow(params->position, params->white), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsSecondRowTest,
    ::testing::Values(
        new TestParams(7, true, false),
        new TestParams(7, false, false),
        new TestParams(8, true, false),
        new TestParams(8, false, true),
        new TestParams(15, true, false),
        new TestParams(15, false, true),
        new TestParams(16, true, false),
        new TestParams(16, false, false),
        new TestParams(47, true, false),
        new TestParams(47, false, false),
        new TestParams(48, true, true),
        new TestParams(48, false, false),
        new TestParams(55, true, true),
        new TestParams(55, false, false),
        new TestParams(56, true, false),
        new TestParams(56, false, false)
    )
);


class IsFourthRowTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(IsFourthRowTest, isFourthRowTest) {
    TestParams* params = GetParam();
    EXPECT_EQ(Positions::isFourthRow(params->position, params->white), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsFourthRowTest,
    ::testing::Values(
        new TestParams(23, true, false),
        new TestParams(23, false, false),
        new TestParams(24, true, false),
        new TestParams(24, false, true),
        new TestParams(31, true, false),
        new TestParams(31, false, true),
        new TestParams(32, true, true),
        new TestParams(32, false, false),
        new TestParams(39, true, true),
        new TestParams(39, false, false),
        new TestParams(40, true, false),
        new TestParams(40, false, false)
    )
);


class IsEightRowTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(IsEightRowTest, isEightRowTest) {
    TestParams* params = GetParam();
    EXPECT_EQ(Positions::isEighthRow(params->position, params->white), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsEightRowTest,
    ::testing::Values(
        new TestParams(0, true, true),
        new TestParams(0, false, false),
        new TestParams(7, true, true),
        new TestParams(7, false, false),
        new TestParams(8, true, false),
        new TestParams(8, false, false),
        new TestParams(55, true, false),
        new TestParams(55, false, false),
        new TestParams(56, true, false),
        new TestParams(56, false, true),
        new TestParams(63, true, false),
        new TestParams(63, false, true)
    )
);


TEST(positionsTest, positionsTest) {
    EXPECT_EQ(INITIAL_FEN_POSITION, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(INITIAL_FEN_POSITION_SHORT, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    EXPECT_EQ(CASTLING_FEN_POSITION, "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(PERFT_FEN_POSITION_2, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(PERFT_FEN_POSITION_3, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(PERFT_FEN_POSITION_4, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(PERFT_FEN_POSITION_5, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(PERFT_FEN_POSITION_6, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
}


class TestParams2 {
public:
    TestParams2(Position firstPosition, Position secondPosition, bool expectedResult) {
        this->firstPosition = firstPosition;
        this->secondPosition = secondPosition;
        this->expectedResult = expectedResult;
    }

    Position firstPosition;
    Position secondPosition;
    bool expectedResult;
};

class AreOnSameRowOrColumnTest : public ::testing::TestWithParam<TestParams2*> {};

TEST_P(AreOnSameRowOrColumnTest, areOnSameRowOrColumnTest) {
    TestParams2* params = GetParam();
    EXPECT_EQ(Positions::areOnSameRowOrColumn(params->firstPosition, params->secondPosition), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    AreOnSameRowOrColumnTest,
    ::testing::Values(
        new TestParams2(0, 4, true),
        new TestParams2(0, 7 , true),
        new TestParams2(0, 32, true),
        new TestParams2(0, 56, true),
        new TestParams2(0, 36, false),
        new TestParams2(0, 63, false),
        new TestParams2(0, 50, false),
        new TestParams2(0, 21, false)
    )
);


class AreOnSameDiagonalTest : public ::testing::TestWithParam<TestParams2*> {};

TEST_P(AreOnSameDiagonalTest, areOnSameDiagonalTest) {
    TestParams2* params = GetParam();
    EXPECT_EQ(Positions::areOnSameDiagonal(params->firstPosition, params->secondPosition), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    AreOnSameDiagonalTest,
    ::testing::Values(
        new TestParams2(0, 36, true),
        new TestParams2(0, 63, true),
        new TestParams2(0, 56, false),
        new TestParams2(0, 7, false),
        new TestParams2(7, 35, true),
        new TestParams2(7, 56, true),
        new TestParams2(7, 4, false),
        new TestParams2(7, 39, false),
        new TestParams2(14, 5, true),
        new TestParams2(14, 23, true),
        new TestParams2(14, 7, true),
        new TestParams2(14, 21, true),
        new TestParams2(14, 24, false),
        new TestParams2(14, 60, false),
        new TestParams2(49, 40, true),
        new TestParams2(49, 58, true),
        new TestParams2(49, 42, true),
        new TestParams2(49, 56, true),
        new TestParams2(49, 3, false),
        new TestParams2(49, 39, false)
    )
);


class TestParams3 {
public:
    TestParams3(Position position, Position excludePosition, Side side, bool expectedResult) {
        this->position = position;
        this->excludePosition = excludePosition;
        this->side = side;
        this->expectedResult = expectedResult;
    }

    Position position;
    Position excludePosition;
    Side side;
    bool expectedResult;
};

class IsOnXRayTest : public ::testing::TestWithParam<TestParams3*> {
protected:
	IsOnXRayTest() {
		BoardUtils::initAttacks();
	}
};

TEST_P(IsOnXRayTest, isOnXRayTest) {
    TestParams3* params = GetParam();
    Game* game = FEN::fenToNewGame("3bqr2/8/8/7B/7Q/7R/8/8 w - - 0 1");
    game->sideToMove = params->side;

    EXPECT_EQ(Positions::isOnXRay(*game, params->position, params->excludePosition), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsOnXRayTest,
    ::testing::Values(
        new TestParams3(24, 3, WHITE, false),
        new TestParams3(25, 3, WHITE, true),
        new TestParams3(26, 3, WHITE, false),
        new TestParams3(27, 3, WHITE, false),
        new TestParams3(28, 3, WHITE, true),
        new TestParams3(29, 3, WHITE, true),
        new TestParams3(30, 3, WHITE, false),
        new TestParams3(31, 3, WHITE, true),
        new TestParams3(4, 31, BLACK, false),
        new TestParams3(12, 31, BLACK, true),
        new TestParams3(20, 31, BLACK, false),
        new TestParams3(28, 31, BLACK, false),
        new TestParams3(36, 31, BLACK, true),
        new TestParams3(44, 31, BLACK, true),
        new TestParams3(52, 31, BLACK, false),
        new TestParams3(60, 31, BLACK, true)
    )
);


class CalculateCheckPositionsPerformanceTest : public testing::Test {
protected:
    void runCheckPositionWithTime(const string& fenGame, bool white) {
        Game* game = FEN::fenToNewGame(fenGame);
        auto start = chrono::steady_clock::now();
        Positions::calculateCheckPositions(*game, white);
        times.push_back(Utils::getElapsedNanos(start));
    }

    long long getAverageTime() {
        return times.size() > 0 ?
            accumulate(times.begin(), times.end(), 0.0) / times.size() :
            0;
    }

    list<long long> times;
};



TEST_F(CalculateCheckPositionsPerformanceTest, calculateCheckPositionsPerformanceTest) {
    GTEST_SKIP();
    runCheckPositionWithTime(INITIAL_FEN_POSITION, true);
    runCheckPositionWithTime(INITIAL_FEN_POSITION, false);
    runCheckPositionWithTime(PERFT_FEN_POSITION_2, true);
    runCheckPositionWithTime(PERFT_FEN_POSITION_2, false);
    runCheckPositionWithTime(PERFT_FEN_POSITION_3, true);
    runCheckPositionWithTime(PERFT_FEN_POSITION_3, false);
    runCheckPositionWithTime(PERFT_FEN_POSITION_4, true);
    runCheckPositionWithTime(PERFT_FEN_POSITION_4, false);
    runCheckPositionWithTime(PERFT_FEN_POSITION_5, true);
    runCheckPositionWithTime(PERFT_FEN_POSITION_5, false);
    runCheckPositionWithTime(PERFT_FEN_POSITION_6, true);
    runCheckPositionWithTime(PERFT_FEN_POSITION_6, false);

    EXPECT_NE(1, 1)
        << "check positions time: " << getAverageTime() << endl;

    // 2500
}


