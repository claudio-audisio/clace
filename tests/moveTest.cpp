#include <gtest/gtest.h>
#include <string>

#include "../move/move.h"
#include "../utils/positions.h"
#include "../utils/fen.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;


class MoveTest : public testing::Test {
protected:
	MoveTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}
	~MoveTest() {

	}
};


class TestParams {
public:
    TestParams(string stringMove, Side side, Position sourcePosition, Position destinationPosition) {
        this->stringMove = stringMove;
        this->side = side;
        this->sourcePosition = sourcePosition;
        this->destinationPosition = destinationPosition;
    }

    string stringMove;
    Side side;
    Position sourcePosition;
    Position destinationPosition;
};

class ConstructorTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(ConstructorTest, decorateTest) {
    TestParams* params = GetParam();

    if (params->sourcePosition != NO_POS) {
        const Move move = createMove(params->stringMove, params->side);

        EXPECT_EQ(getSourcePosition(move), params->sourcePosition);
        EXPECT_EQ(getDestinationPosition(move), params->destinationPosition);
        EXPECT_EQ(getMoveSide(move), params->side);

    	if (params->side == _WHITE) {
    		ASSERT_TRUE(isWhite(move));
    	} else {
    		ASSERT_FALSE(isWhite(move));
    	}
    }
    else {
    	EXPECT_ANY_THROW(createMove(params->stringMove, params->side));
    }
}

INSTANTIATE_TEST_SUITE_P(
    MoveTest,
    ConstructorTest,
    ::testing::Values(
        new TestParams("", _WHITE, NO_POS, NO_POS),
        new TestParams("dasfdfs", _WHITE, NO_POS, NO_POS),
        new TestParams("dasfd", _WHITE, NO_POS, NO_POS),
        new TestParams("a1b1", _WHITE, 56, 57),
        new TestParams("e3h6", _BLACK, 44, 23)
    )
);


class TestParams2 {
public:
    TestParams2(string fenGame, Position sourcePosition, Position destinationPosition, Piece piece, bool isCastling, bool isEnPassant, bool isPawnPromotion) {
        this->fenGame = fenGame;
        this->sourcePosition = sourcePosition;
        this->destinationPosition = destinationPosition;
        this->piece = piece;
        this->isCastling = isCastling;
        this->isEnPassant = isEnPassant;
        this->isPawnPromotion = isPawnPromotion;
    }

    string fenGame;
    Position sourcePosition;
    Position destinationPosition;
    Piece piece;
    bool isCastling;
    bool isEnPassant;
    bool isPawnPromotion;
};

class DecorateTest : public ::testing::TestWithParam<TestParams2*> {};

TEST_P(DecorateTest, decorateTest) {
    TestParams2* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    Move move = createMove(params->sourcePosition, params->destinationPosition, game->getSide(params->sourcePosition), params->piece, game->board->enPassantPosition);

    EXPECT_EQ(getPiece(move), params->piece);
    EXPECT_EQ(isCastling(move), params->isCastling);
    EXPECT_EQ(isEnPassant(move), params->isEnPassant);
    EXPECT_EQ(isPawnPromotion(move), params->isPawnPromotion);
}

INSTANTIATE_TEST_SUITE_P(
    MoveTest,
    DecorateTest,
    ::testing::Values(
        new TestParams2(INITIAL_FEN_POSITION, 8, 16, BPawn, false, false, false),
        new TestParams2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 6, BKing, true, false, false),
        new TestParams2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 2, BKing, true, false, false),
        new TestParams2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 60, 62, WKing, true, false, false),
        new TestParams2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 60, 58, WKing, true, false, false),
        new TestParams2("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 11, 2, WPawn, false, false, true),
        new TestParams2("8/6bb/8/8/R1pP2k1/4P3/P7/K7 b - d3 0 1", 34, 43, BPawn, false, true, false)
    )
);

TEST_F(MoveTest, capturedTest) {
	Move move = createMove(60, 52, _WHITE, WKing, NO_POS);

	ASSERT_FALSE(isCaptured(move));

	setCaptured(move, BPawn);

	ASSERT_TRUE(isCaptured(move));
	EXPECT_EQ(BPawn, getCaptured(move));

	setCaptured(move, Empty);

	ASSERT_FALSE(isCaptured(move));
	EXPECT_EQ(Empty, getCaptured(move));
}


TEST_F(MoveTest, getTypeTest) {
	Move move = createMove(60, 52, _WHITE, WKing, NO_POS);

	EXPECT_EQ(NORMAL, getType(move));

	setCastling(move, true);

	EXPECT_EQ(CASTLING, getType(move));

	setCastling(move, false);
	setEnPassant(move, true);

	EXPECT_EQ(EN_PASSANT, getType(move));

	setEnPassant(move, false);
	setPawnPromotion(move, true);

	EXPECT_EQ(PROMOTION, getType(move));
}

TEST_F(MoveTest, getMoveResultTest) {
	Move move = createMove(60, 52, _WHITE, WKing, NO_POS);

	EXPECT_EQ(NORMAL, getType(move));

	MoveResult moveResult = getMoveResult(false, getType(move));

	GTEST_ASSERT_FALSE(isCapturedMR(moveResult));
	GTEST_ASSERT_FALSE(isCastlingMR(moveResult));
	GTEST_ASSERT_FALSE(isEnPassantMR(moveResult));
	GTEST_ASSERT_FALSE(isPromotedMR(moveResult));

	moveResult = getMoveResult(true, getType(move));

	GTEST_ASSERT_TRUE(isCapturedMR(moveResult));
	GTEST_ASSERT_FALSE(isCastlingMR(moveResult));
	GTEST_ASSERT_FALSE(isEnPassantMR(moveResult));
	GTEST_ASSERT_FALSE(isPromotedMR(moveResult));

	setCastling(move, true);

	EXPECT_EQ(CASTLING, getType(move));

	moveResult = getMoveResult(false, getType(move));

	GTEST_ASSERT_FALSE(isCapturedMR(moveResult));
	GTEST_ASSERT_TRUE(isCastlingMR(moveResult));
	GTEST_ASSERT_FALSE(isEnPassantMR(moveResult));
	GTEST_ASSERT_FALSE(isPromotedMR(moveResult));

	moveResult = getMoveResult(true, getType(move));

	GTEST_ASSERT_TRUE(isCapturedMR(moveResult));
	GTEST_ASSERT_TRUE(isCastlingMR(moveResult));
	GTEST_ASSERT_FALSE(isEnPassantMR(moveResult));
	GTEST_ASSERT_FALSE(isPromotedMR(moveResult));

	setCastling(move, false);
	setEnPassant(move, true);

	EXPECT_EQ(EN_PASSANT, getType(move));

	moveResult = getMoveResult(false, getType(move));

	GTEST_ASSERT_FALSE(isCapturedMR(moveResult));
	GTEST_ASSERT_FALSE(isCastlingMR(moveResult));
	GTEST_ASSERT_TRUE(isEnPassantMR(moveResult));
	GTEST_ASSERT_FALSE(isPromotedMR(moveResult));

	moveResult = getMoveResult(true, getType(move));

	GTEST_ASSERT_TRUE(isCapturedMR(moveResult));
	GTEST_ASSERT_FALSE(isCastlingMR(moveResult));
	GTEST_ASSERT_TRUE(isEnPassantMR(moveResult));
	GTEST_ASSERT_FALSE(isPromotedMR(moveResult));

	setEnPassant(move, false);
	setPawnPromotion(move, true);

	EXPECT_EQ(PROMOTION, getType(move));

	moveResult = getMoveResult(false, getType(move));

	GTEST_ASSERT_FALSE(isCapturedMR(moveResult));
	GTEST_ASSERT_FALSE(isCastlingMR(moveResult));
	GTEST_ASSERT_FALSE(isEnPassantMR(moveResult));
	GTEST_ASSERT_TRUE(isPromotedMR(moveResult));

	moveResult = getMoveResult(true, getType(move));

	GTEST_ASSERT_TRUE(isCapturedMR(moveResult));
	GTEST_ASSERT_FALSE(isCastlingMR(moveResult));
	GTEST_ASSERT_FALSE(isEnPassantMR(moveResult));
	GTEST_ASSERT_TRUE(isPromotedMR(moveResult));
}


class PositionsTest : public ::testing::TestWithParam<Position> {};

TEST_P(PositionsTest, positionsTest) {
    Position position = GetParam();
    Move move = 0;
    setSourcePosition(move, position);
    setDestinationPosition(move, position);
    EXPECT_EQ(position, getSourcePosition(move));
    EXPECT_EQ(position, getDestinationPosition(move));
}

INSTANTIATE_TEST_SUITE_P(
    MoveTest,
    PositionsTest,
    ::testing::Values(0, 12, 45, 63)
);


class PiecesTest : public ::testing::TestWithParam<Piece> {};

TEST_P(PiecesTest, piecesTest) {
    Piece piece = GetParam();
    Move move = 0;
    setPiece(move, piece);
    EXPECT_EQ(piece, getPiece(move));
}

INSTANTIATE_TEST_SUITE_P(
    MoveTest,
    PiecesTest,
    ::testing::Values(WPawn, BKing, WQueen, BRook, Empty)
);


class TestParams3 {
public:
	TestParams3(string uciMove, Position sourcePosition, Position destinationPosition, Piece promotion) {
		this->uciMove = uciMove;
		this->sourcePosition = sourcePosition;
		this->destinationPosition = destinationPosition;
		this->promotion = promotion;
	}

	string uciMove;
	Position sourcePosition;
	Position destinationPosition;
	Piece promotion;
};

class ParseUciMoveTest : public ::testing::TestWithParam<TestParams3*> {};

TEST_P(ParseUciMoveTest, parseUciMoveTest) {
	TestParams3* params = GetParam();
	Move move = parseUciMove(params->uciMove);

	EXPECT_EQ(getSourcePosition(move), params->sourcePosition);
	EXPECT_EQ(getDestinationPosition(move), params->destinationPosition);
	EXPECT_EQ(getPromotion(move), params->promotion);
}

INSTANTIATE_TEST_SUITE_P(
		MoveTest,
		ParseUciMoveTest,
		::testing::Values(
			new TestParams3("d2d4", 51, 35, Empty),
			new TestParams3("b8c6", 1, 18, Empty),
			new TestParams3("b7b8q", 9, 1, WQueen)
		)
);


class ToUciMoveTest : public ::testing::TestWithParam<TestParams3*> {};

TEST_P(ToUciMoveTest, toUciMoveTest) {
	TestParams3* params = GetParam();
	Move move = createMove(params->sourcePosition, params->destinationPosition, _WHITE);
	setPromotion(move, params->promotion);

	if (params->promotion != Empty) {
		setPawnPromotion(move, true);
	}

	EXPECT_EQ(toUciMove(move), params->uciMove);
}

INSTANTIATE_TEST_SUITE_P(
		MoveTest,
		ToUciMoveTest,
		::testing::Values(
				new TestParams3("d2d4", 51, 35, Empty),
				new TestParams3("b8c6", 1, 18, Empty),
				new TestParams3("b7b8q", 9, 1, WQueen)
		)
);



