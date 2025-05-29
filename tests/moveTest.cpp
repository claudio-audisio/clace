#include <gtest/gtest.h>
#include <string>

#include "../move/move.h"
#include "../utils/positions.h"
#include "../utils/fen.h"

using namespace std;


class MoveTest : public testing::Test {
protected:
	MoveTest() {
		BoardUtils::initAttacks();
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
        Move move = MoveHelper::getMove(params->stringMove, params->side);

        EXPECT_EQ(MoveHelper::getSourcePosition(move), params->sourcePosition);
        EXPECT_EQ(MoveHelper::getDestinationPosition(move), params->destinationPosition);
        EXPECT_EQ(MoveHelper::getSide(move), params->side);
    }
    /* TODO da fare quandoo gestiamo le eccezioni
    else {
        Exception exception = assertThrows(RuntimeException.class, () -> {
            new Move(stringMove, white);
        });

        if (destinationPosition == -1) {
            EXPECT_EQ(exception.getMessage(), "move malformed");
        }
        else {
            EXPECT_EQ(exception.getMessage(), "piece coordinates malformed");
        }
    }
    */
}

INSTANTIATE_TEST_SUITE_P(
    MoveTest,
    ConstructorTest,
    ::testing::Values(
        new TestParams("", true, NO_POS, NO_POS),
        new TestParams("dasfdfs", true, NO_POS, NO_POS),
        new TestParams("dasfd", true, NO_POS, NO_POS),
        new TestParams("a1b1", true, 56, 57),
        new TestParams("e3h6", false, 44, 23)
    )
);


/*class TestParams2 {
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
    Move move = MoveHelper::getMove(params->sourcePosition, params->destinationPosition, game->getSide(params->sourcePosition), params->piece, game->board.enPassantPosition, game->isComputerToMove());

    EXPECT_EQ(MoveHelper::getPiece(move), params->piece);
    EXPECT_EQ(MoveHelper::isCastling(move), params->isCastling);
    EXPECT_EQ(MoveHelper::isEnPassant(move), params->isEnPassant);
    EXPECT_EQ(MoveHelper::isPawnPromotion(move), params->isPawnPromotion);
    GTEST_ASSERT_TRUE(MoveHelper::isComputer(move));
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
);*/


class PositionsTest : public ::testing::TestWithParam<Position> {};

TEST_P(PositionsTest, positionsTest) {
    Position position = GetParam();
    Move move = 0;
    MoveHelper::setSourcePosition(move, position);
    MoveHelper::setDestinationPosition(move, position);
    EXPECT_EQ(position, MoveHelper::getSourcePosition(move));
    EXPECT_EQ(position, MoveHelper::getDestinationPosition(move));
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
    MoveHelper::setPiece(move, piece);
    EXPECT_EQ(piece, MoveHelper::getPiece(move));
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
	Move move = MoveHelper::parseUciMove(params->uciMove);

	EXPECT_EQ(MoveHelper::getSourcePosition(move), params->sourcePosition);
	EXPECT_EQ(MoveHelper::getDestinationPosition(move), params->destinationPosition);
	EXPECT_EQ(MoveHelper::getPromotion(move), params->promotion);
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
	Move move = MoveHelper::getMove(params->sourcePosition, params->destinationPosition, WHITE);
	MoveHelper::setPromotion(move, params->promotion);

	if (params->promotion != Empty) {
		MoveHelper::setPawnPromotion(move, true);
	}

	EXPECT_EQ(MoveHelper::toUciMove(move), params->uciMove);
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



