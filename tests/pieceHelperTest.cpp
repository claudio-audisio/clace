#include <gtest/gtest.h>
#include <string>

#include "../utils/pieceHelper.h"
#include "../game/game.h"

using namespace std;


class TestParams {
public:
	TestParams(Piece piece, bool opponentWhite, bool expectedResult) {
		this->piece = piece;
		this->opponentWhite = opponentWhite;
		this->expectedResult = expectedResult;
	}

	Piece piece;
	bool opponentWhite;
	bool expectedResult;
};

class IsOpponentTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(IsOpponentTest, isOpponentTest) {
	TestParams* params = GetParam();
	Game game;
	
	EXPECT_EQ(_isOpponent(params->piece, params->opponentWhite), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
	PieceHelperTest,
	IsOpponentTest,
	::testing::Values(
		new TestParams(Empty, true, false),
		new TestParams(Empty, false, false),
		new TestParams(WPawn, true, false),
		new TestParams(WPawn, false, true),
		new TestParams(BPawn, true, true),
		new TestParams(BPawn, false, false)
	)
);


class IsEmptyOrOpponentTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(IsEmptyOrOpponentTest, isOpponentTest) {
	TestParams* params = GetParam();
	Game game;

	EXPECT_EQ(_isEmptyOrOpponent(params->piece, params->opponentWhite), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
	PieceHelperTest,
	IsEmptyOrOpponentTest,
	::testing::Values(
		new TestParams(Empty, true, true),
		new TestParams(Empty, false, true),
		new TestParams(WPawn, true, false),
		new TestParams(WPawn, false, true),
		new TestParams(BPawn, true, true),
		new TestParams(BPawn, false, false)
	)
);


class ValueOfTest : public ::testing::TestWithParam<tuple<string, Piece>> {};

TEST_P(ValueOfTest, valueOfTest) {
	string stringPiece = get<0>(GetParam());
	Piece piece = get<1>(GetParam());
	Game game;

	EXPECT_EQ(valueOf(stringPiece), piece);
}

INSTANTIATE_TEST_SUITE_P(
	PieceHelperTest,
	ValueOfTest,
	::testing::Values(
		make_tuple("Empty", Empty),
		make_tuple("WPawn", WPawn),
		make_tuple("WKnight", WKnight),
		make_tuple("WBishop", WBishop),
		make_tuple("WRook", WRook),
		make_tuple("WQueen", WQueen),
		make_tuple("WKing", WKing),
		make_tuple("BPawn", BPawn),
		make_tuple("BKnight", BKnight),
		make_tuple("BBishop", BBishop),
		make_tuple("BRook", BRook),
		make_tuple("BQueen", BQueen),
		make_tuple("BKing", BKing)
	)
);
