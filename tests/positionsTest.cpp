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
    EXPECT_EQ(indexToCoords(index), expectedCoordinates);
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
    EXPECT_EQ(coordsToIndex(coordinates), expectedResult);
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
    TestParams(Position position, Side side, bool expectedResult) {
        this->position = position;
        this->side = side;
        this->expectedResult = expectedResult;
    }

    Position position;
    Side side;
    bool expectedResult;
};

class IsFirstRowTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(IsFirstRowTest, isFirstRowTest) {
    TestParams* params = GetParam();
    EXPECT_EQ(isFirstRow(params->position, params->side), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsFirstRowTest,
    ::testing::Values(
        new TestParams(0, _WHITE, false),
        new TestParams(0, _BLACK, true),
        new TestParams(7, _WHITE, false),
        new TestParams(7, _BLACK, true),
        new TestParams(8, _WHITE, false),
        new TestParams(8, _BLACK, false),
        new TestParams(55, _WHITE, false),
        new TestParams(55, _BLACK, false),
        new TestParams(56, _WHITE, true),
        new TestParams(56, _BLACK, false),
        new TestParams(63, _WHITE, true),
        new TestParams(63, _BLACK, false)
    )
);


class IsSecondRowTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(IsSecondRowTest, isSecondRowTest) {
    TestParams* params = GetParam();
    EXPECT_EQ(isSecondRow(params->position, params->side), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsSecondRowTest,
    ::testing::Values(
        new TestParams(7, _WHITE, false),
        new TestParams(7, _BLACK, false),
        new TestParams(8, _WHITE, false),
        new TestParams(8, _BLACK, true),
        new TestParams(15, _WHITE, false),
        new TestParams(15, _BLACK, true),
        new TestParams(16, _WHITE, false),
        new TestParams(16, _BLACK, false),
        new TestParams(47, _WHITE, false),
        new TestParams(47, _BLACK, false),
        new TestParams(48, _WHITE, true),
        new TestParams(48, _BLACK, false),
        new TestParams(55, _WHITE, true),
        new TestParams(55, _BLACK, false),
        new TestParams(56, _WHITE, false),
        new TestParams(56, _BLACK, false)
    )
);


class IsFourthRowTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(IsFourthRowTest, isFourthRowTest) {
    TestParams* params = GetParam();
    EXPECT_EQ(isFourthRow(params->position, params->side), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsFourthRowTest,
    ::testing::Values(
        new TestParams(23, _WHITE, false),
        new TestParams(23, _BLACK, false),
        new TestParams(24, _WHITE, false),
        new TestParams(24, _BLACK, true),
        new TestParams(31, _WHITE, false),
        new TestParams(31, _BLACK, true),
        new TestParams(32, _WHITE, true),
        new TestParams(32, _BLACK, false),
        new TestParams(39, _WHITE, true),
        new TestParams(39, _BLACK, false),
        new TestParams(40, _WHITE, false),
        new TestParams(40, _BLACK, false)
    )
);


class TestParams12 {
public:
	TestParams12(Position position, Side side, bool expectedResult) {
		this->position = position;
		this->side = side;
		this->expectedResult = expectedResult;
	}

	Position position;
	Side side;
	bool expectedResult;
};

class IsEightRowTest : public ::testing::TestWithParam<TestParams12*> {};

TEST_P(IsEightRowTest, isEightRowTest) {
	TestParams12* params = GetParam();
    EXPECT_EQ(isEighthRow(params->position, params->side), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    IsEightRowTest,
    ::testing::Values(
        new TestParams12(0, _WHITE, true),
        new TestParams12(0, _BLACK, false),
        new TestParams12(7, _WHITE, true),
        new TestParams12(7, _BLACK, false),
        new TestParams12(8, _WHITE, false),
        new TestParams12(8, _BLACK, false),
        new TestParams12(55, _WHITE, false),
        new TestParams12(55, _BLACK, false),
        new TestParams12(56, _WHITE, false),
        new TestParams12(56, _BLACK, true),
        new TestParams12(63, _WHITE, false),
        new TestParams12(63, _BLACK, true)
    )
);


TEST(positionsTest, positionsTest) {
    EXPECT_EQ(INITIAL_FEN_POSITION, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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
    EXPECT_EQ(areOnSameRowOrColumn(params->firstPosition, params->secondPosition), params->expectedResult);
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
    EXPECT_EQ(areOnSameDiagonal(params->firstPosition, params->secondPosition), params->expectedResult);
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

class AreOnSameRowOrColumnOrDiagonalTest : public ::testing::TestWithParam<TestParams2*> {};

TEST_P(AreOnSameRowOrColumnOrDiagonalTest, areOnSameRowOrColumnOrDiagonalTest) {
    TestParams2* params = GetParam();
    EXPECT_EQ(areOnSameRowOrColumnOrDiagonal(params->firstPosition, params->secondPosition), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    positionsTest,
    AreOnSameRowOrColumnOrDiagonalTest,
    ::testing::Values(
        new TestParams2(0, 4, true),
        new TestParams2(0, 7 , true),
        new TestParams2(0, 32, true),
        new TestParams2(0, 56, true),
        new TestParams2(0, 36, true),
        new TestParams2(0, 63, true),
        new TestParams2(0, 50, false),
        new TestParams2(0, 21, false),
        new TestParams2(0, 36, true),
        new TestParams2(0, 63, true),
        new TestParams2(0, 56, true),
        new TestParams2(0, 7, true),
        new TestParams2(7, 35, true),
        new TestParams2(7, 56, true),
        new TestParams2(7, 4, true),
        new TestParams2(7, 39, true),
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

