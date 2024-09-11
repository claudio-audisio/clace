#include <gtest/gtest.h>
#include <string>

#include "../move/move.h"
#include "../utils/positions.h"
#include "../utils/fen.h"

using namespace std;


class TestParams {
public:
    TestParams(string stringMove, bool white, Position sourcePosition, Position destinationPosition) {
        this->stringMove = stringMove;
        this->white = white;
        this->sourcePosition = sourcePosition;
        this->destinationPosition = destinationPosition;
    }

    string stringMove;
    bool white;
    Position sourcePosition;
    Position destinationPosition;
};


class ConstructorTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(ConstructorTest, decorateTest) {
    TestParams* params = GetParam();

    if (params->sourcePosition != NO_POS) {
        Move* move = new Move(params->stringMove, params->white);

        EXPECT_EQ(move->getSourcePosition(), params->sourcePosition);
        EXPECT_EQ(move->getDestinationPosition(), params->destinationPosition);
        EXPECT_EQ(move->isWhite(), params->white);
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
        new TestParams("a1-b1", true, 56, 57),
        new TestParams("e3-h6", false, 44, 23)
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
    Move* move = new Move(params->sourcePosition, params->destinationPosition, game->isWhite(params->sourcePosition));
    move->decorate(params->piece, game->getEnPassantPosition(), game->isComputerToMove());

    EXPECT_EQ(move->getPiece(), params->piece);
    EXPECT_EQ(move->isCastling(), params->isCastling);
    EXPECT_EQ(move->isEnPassant(), params->isEnPassant);
    EXPECT_EQ(move->isPawnPromotion(), params->isPawnPromotion);
    GTEST_ASSERT_TRUE(move->isComputer());
}

INSTANTIATE_TEST_SUITE_P(
    MoveTest,
    DecorateTest,
    ::testing::Values(
        new TestParams2(Positions::INITIAL_FEN_POSITION, 8, 16, BPawn, false, false, false),
        new TestParams2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 6, BKing, true, false, false),
        new TestParams2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 2, BKing, true, false, false),
        new TestParams2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 60, 62, WKing, true, false, false),
        new TestParams2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 60, 58, WKing, true, false, false),
        new TestParams2("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 11, 2, WPawn, false, false, true),
        new TestParams2("8/6bb/8/8/R1pP2k1/4P3/P7/K7 b - d3 0 1", 34, 43, BPawn, false, true, false)
    )
);