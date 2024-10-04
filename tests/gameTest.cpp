#include <gtest/gtest.h>
#include <string>

#include "../game/game.h"
#include "../utils/castlingHelper.h"
#include "../utils/fen.h"
#include "../move/movesGenerator.h"

using namespace std;


TEST(GameTest, ConstructorTest) {
	Game game;

	for (Position i = 0; i < 64; i++) {
		GTEST_ASSERT_TRUE(game.isEmpty(i));
	}
}

TEST(GameTest, initTest) {
	Game game;
	game.init();

	EXPECT_EQ(game.getFullMoves(), 1);
	EXPECT_EQ(game.getHalfMoveClock(), 0);
	EXPECT_EQ(game.getBlackKingPosition(), 4);
	EXPECT_EQ(game.getWhiteKingPosition(), 60);
	GTEST_ASSERT_TRUE(game.isWhiteToMove());
	EXPECT_EQ(game.getEnPassantPosition(), NO_POS);
	EXPECT_EQ(game.getLastMove(), 0);
	GTEST_ASSERT_TRUE(CastlingHelper::isWhiteQueenCastling(game.getCastlingInfo()));
	GTEST_ASSERT_TRUE(CastlingHelper::isWhiteKingCastling(game.getCastlingInfo()));
	GTEST_ASSERT_TRUE(CastlingHelper::isBlackQueenCastling(game.getCastlingInfo()));
	GTEST_ASSERT_TRUE(CastlingHelper::isBlackKingCastling(game.getCastlingInfo()));
	GTEST_ASSERT_TRUE(game.getMovesHistory().empty());
	EXPECT_EQ(FEN::gameToFEN(game), Positions::INITIAL_FEN_POSITION);
}

TEST(GameTest, setKingPositionsTest) {
	Game game;
	game.setPiece(17, WKing);
	game.setPiece(42, BKing);
	game.setKingPositions();

	EXPECT_EQ(game.getWhiteKingPosition(), 17);
	EXPECT_EQ(game.getBlackKingPosition(), 42);
}


TEST(GameTest, setPieceTest) {
	Game game;
	Piece piece = game.setPiece(0, WPawn);

	EXPECT_EQ(piece, Empty);

	piece = game.setPiece(0, BPawn);

	EXPECT_EQ(piece, WPawn);
}

TEST(GameTest, rawMoveTest) {
	Game game;
	game.setPiece(17, WPawn);
	Piece piece = game.rawMove(17, 18, WPawn);

	EXPECT_EQ(piece, Empty);
	EXPECT_EQ(game.getPiece(17), Empty);
	EXPECT_EQ(game.getPiece(18), WPawn);
}

TEST(GameTest, setEmptyTest) {
	Game game;
	game.setPiece(17, WPawn);
	game.setEmptyPiece(17);

	EXPECT_EQ(game.getPiece(17), Empty);
}


class CheckFiftyMoveRuleTest : public ::testing::TestWithParam<tuple<unsigned int, EndGameType>> {};

TEST_P(CheckFiftyMoveRuleTest, checkFiftyMoveRuleTest) {
	Game game;
	unsigned int halfMoveClock = get<0>(GetParam());
	EndGameType expectedEndGame = get<1>(GetParam());
	game.setHalfMoveClock(halfMoveClock);

	EXPECT_EQ(game.checkFiftyMoveRule(), expectedEndGame);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	CheckFiftyMoveRuleTest,
	::testing::Values(
		make_tuple(0, EndGameType::NONE),
		make_tuple(49, EndGameType::NONE),
		make_tuple(50, EndGameType::FIFTY_MOVE_RULE),
		make_tuple(51, EndGameType::FIFTY_MOVE_RULE)
	)
);


TEST(GameTest, isWhiteTest) {
	Game game;
	game.setPiece(0, WPawn);
	game.setPiece(1, BPawn);

	GTEST_ASSERT_TRUE(game.getSide(0) == WHITE);
	GTEST_ASSERT_TRUE(game.getSide(1) == BLACK);
}


/*class TestParams {
public:
	TestParams(Position position, bool checkWhite, bool expectedWhite) {
		this->position = position;
		this->checkWhite = checkWhite;
		this->expectedWhite = expectedWhite;
	}

	Position position;
	bool checkWhite;
	bool expectedWhite;
};

class CheckWhiteTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(CheckWhiteTest, checkWhiteTest) {
	TestParams* params = GetParam();
	Game game;
	game.setPiece(0, WPawn);
	game.setPiece(1, BPawn);
	EXPECT_EQ(game.checkColor(params->position, params->checkWhite), params->expectedWhite);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	CheckWhiteTest,
	::testing::Values(
		new TestParams(0, true, true),
		new TestParams(0, false, false),
		new TestParams(1, true, false),
		new TestParams(1, false, true),
		new TestParams(2, true, false),
		new TestParams(2, false, false)
	)
);*/


class TestParams2 {
public:
	TestParams2(string fenBoard, Position sourcePos, Position destinationPos, string expectedFenBoardAfterMove, bool expectedCaptured, bool expectedPromotion, string expectedLastMove) {
		this->fenBoard = fenBoard;
		this->sourcePos = sourcePos;
		this->destinationPos = destinationPos;
		this->expectedFenBoardAfterMove = expectedFenBoardAfterMove;
		this->expectedCaptured = expectedCaptured;
		this->expectedPromotion = expectedPromotion;
		this->expectedLastMove = expectedLastMove;
	}

	string fenBoard;
	Position sourcePos;
	Position destinationPos;
	string expectedFenBoardAfterMove;
	bool expectedCaptured;
	bool expectedPromotion;
	string expectedLastMove;
};

class ApplyMoveTest : public ::testing::TestWithParam<TestParams2*> {};

TEST_P(ApplyMoveTest, applyMoveTest) {
	TestParams2* testParams = GetParam();
	Game* game = FEN::fenToNewGame(testParams->fenBoard);
	Side side = game->getSide(testParams->sourcePos);
	game->setSideToMove(side);
	Move move = MoveHelper::getMove(testParams->sourcePos, testParams->destinationPos, side);
	MoveHelper::decorate(move, game->getPiece(testParams->sourcePos), game->getEnPassantPosition(), game->isComputerToMove());
	MoveHelper::setPromotion(move, side == WHITE ? WQueen : BQueen);
	const MoveResult moveResult = game->applyMove(move);
	string fenBoardAfterMove = FEN::gameToFEN(*game);

	EXPECT_EQ(fenBoardAfterMove, testParams->expectedFenBoardAfterMove);
	EXPECT_EQ(MoveHelper::isCapturedMR(moveResult), testParams->expectedCaptured);
	EXPECT_EQ(MoveHelper::isPromotedMR(moveResult), testParams->expectedPromotion);
	EXPECT_EQ(MoveHelper::toString(game->getLastMove()), testParams->expectedLastMove);
	EXPECT_EQ(game->getMovesHistory().size(), 1);
	EXPECT_EQ(MoveHelper::toString(game->getMovesHistory().front()), testParams->expectedLastMove);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	ApplyMoveTest,
	::testing::Values(
		new TestParams2(Positions::INITIAL_FEN_POSITION, 48, 40, "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1", false, false, "a2-a3"),
		new TestParams2(Positions::INITIAL_FEN_POSITION, 48, 32, "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1", false, false, "a2-a4"),
		new TestParams2(Positions::CASTLING_FEN_POSITION, 4, 2, "2kr3r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQ - 1 2", false, false, "e8-c8"),
		new TestParams2(Positions::CASTLING_FEN_POSITION, 4, 6, "r4rk1/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQ - 1 2", false, false, "e8-g8"),
		new TestParams2(Positions::CASTLING_FEN_POSITION, 60, 58, "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq - 1 1", false, false, "e1-c1"),
		new TestParams2(Positions::CASTLING_FEN_POSITION, 60, 62, "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1", false, false, "e1-g1"),
		new TestParams2("8/8/8/8/Pp6/8/8/8 b - a3 0 1", 33, 40, "8/8/8/8/8/p7/8/8 w - - 0 2", true, false, "b4-a3"),
		new TestParams2("8/P7/8/8/8/8/8/8 w - - 0 1", 8, 0, "Q7/8/8/8/8/8/8/8 b - - 0 1", false, true, "a7-a8"),
		new TestParams2("1b6/P7/8/8/8/8/8/8 w - - 0 1", 8, 1, "1Q6/8/8/8/8/8/8/8 b - - 0 1", true, true, "a7-b8"),
		new TestParams2("q7/8/8/8/8/8/8/7Q b - - 0 1", 0, 63, "8/8/8/8/8/8/8/7q w - - 0 2", true, false, "a8-h1")
	)
);


class TestParams3 {
public:
	TestParams3(string fenBoard, string lastMove, bool check, bool discoveryCheck, bool doubleCheck) {
		this->fenBoard = fenBoard;
		this->lastMove = lastMove;
		this->check = check;
		this->discoveryCheck = discoveryCheck;
		this->doubleCheck = doubleCheck;
	}

	string fenBoard;
	string lastMove;
	bool check;
	bool discoveryCheck;
	bool doubleCheck;
};

class VerifyChecksTest : public ::testing::TestWithParam<TestParams3*> {};

TEST_P(VerifyChecksTest, verifyChecksTest) {
	TestParams3* testParams = GetParam();
	Game* game = FEN::fenToNewGame(testParams->fenBoard);
	
	if (!testParams->lastMove.empty()) {
		game->setLastMove(MoveHelper::getMove(testParams->lastMove, BLACK));
	}

	game->verifyChecks();

	EXPECT_EQ(game->getCheckStatus().isCheck(), testParams->check);
	EXPECT_EQ(game->getCheckStatus().isDiscoveryCheck(), testParams->discoveryCheck);
	EXPECT_EQ(game->getCheckStatus().isDoubleCheck(), testParams->doubleCheck);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	VerifyChecksTest,
	::testing::Values(
		new TestParams3(Positions::INITIAL_FEN_POSITION, "", false, false, false),
		new TestParams3("8/8/8/8/8/8/r7/1r5K w - - 0 1", "b3-b1", true, false, false),
		new TestParams3("8/8/8/8/8/4n3/r7/1r5K w - - 0 1", "d1-e3", true, true, false),
		new TestParams3("b7/8/8/8/8/8/r7/1r5K w - - 0 1", "b7-b1", true, false, true),
		new TestParams3("K7/2p5/1pp5/8/8/8/8/8 w - - 0 1", "b5-b6", false, false, false)
	)
);


TEST(GameTest, verifyChecksWithEnPassantTest) {
	Game* game = FEN::fenToNewGame("k7/8/8/b7/8/1p6/8/4K3 w - - 0 1");
	Move move = MoveHelper::getMove(35, 41, BLACK);
	MoveHelper::decorate(move, BPawn, 41, true);
	MoveHelper::setCaptured(move, true);
	game->setLastMove(move);

	game->verifyChecks();

	GTEST_ASSERT_TRUE(game->getCheckStatus().isCheck());
	GTEST_ASSERT_TRUE(game->getCheckStatus().isDiscoveryCheck());
	GTEST_ASSERT_FALSE(game->getCheckStatus().isDoubleCheck());
}


class CheckEndGameTest : public ::testing::TestWithParam<tuple<string, EndGameType>> {};

TEST_P(CheckEndGameTest, checkEndGameTest) {
	string fenBoard = get<0>(GetParam());
	EndGameType expectedEndGame = get<1>(GetParam());
	Game* game = FEN::fenToNewGame(fenBoard);
	vector<Move> moves;
	game->verifyChecks();
	MovesGenerator::generateLegalMoves(*game, moves);

	EXPECT_EQ(game->checkEndGame(moves.empty()), expectedEndGame);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	CheckEndGameTest,
	::testing::Values(
		make_tuple(Positions::INITIAL_FEN_POSITION, EndGameType::NONE),
		make_tuple("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 50 25", EndGameType::FIFTY_MOVE_RULE),
		make_tuple("8/8/8/8/8/8/R7/R6k b - - 50 25", EndGameType::CHECKMATE),
		make_tuple("8/8/8/8/8/8/r7/r6K/ w - - 50 25", EndGameType::CHECKMATE),
		make_tuple("k7/2P5/1PP5/8/8/8/8/8 b - - 50 25", EndGameType::STALEMATE)
	)
);


class TestParams4 {
public:
	TestParams4(string fenBoard, bool isCastling, Position kingPosition, bool expectedResult) {
		this->fenBoard = fenBoard;
		this->isCastling = isCastling;
		this->kingPosition = kingPosition;
		this->expectedResult = expectedResult;
	}

	string fenBoard;
	bool isCastling;
	Position kingPosition;
	bool expectedResult;
};

class CheckControlTest : public ::testing::TestWithParam<TestParams4*> {};

TEST_P(CheckControlTest, checkControlTest) {
	TestParams4* params = GetParam();
	Game* game = FEN::fenToNewGame(params->fenBoard);
	game->setKingPositions();
	Side side = game->getSide(params->kingPosition);
	game->setSideToMove(BoardUtils::opposite(side));
	Position sourcePosition = params->isCastling ? side == WHITE ? 60 : 4 : 0;
	Move move = MoveHelper::getMove(sourcePosition, params->kingPosition, side);
	MoveHelper::decorate(move, game->getPiece(params->kingPosition), game->getEnPassantPosition(), game->isComputerToMove());

	EXPECT_EQ(game->checkControl(move), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	CheckControlTest,
	::testing::Values(
		new TestParams4(Positions::INITIAL_FEN_POSITION, false, 4, true),
		new TestParams4(Positions::INITIAL_FEN_POSITION, false, 60, true),
		new TestParams4("k7/8/8/8/8/8/8/R6Q b - - 0 1", false, 0, false),
		new TestParams4("q6r/8/8/8/8/8/8/7K w - - 0 1", false, 63, false),
		new TestParams4("2kr3r/8/8/8/8/8/8/1R6 b - - 0 1", true, 2, true),
		new TestParams4("2kr3r/8/8/8/8/8/8/2R5 b - - 0 1", true, 2, false),
		new TestParams4("2kr3r/8/8/8/8/8/8/3R4 b - - 0 1", true, 2, false),
		new TestParams4("2kr3r/8/8/8/8/8/8/4R3 b - - 0 1", true, 2, false),
		new TestParams4("r4rk1/8/8/8/8/8/8/4R3 b - - 0 1", true, 6, false),
		new TestParams4("r4rk1/8/8/8/8/8/8/5R2 b - - 0 1", true, 6, false),
		new TestParams4("r4rk1/8/8/8/8/8/8/6R1 b - - 0 1", true, 6, false),
		new TestParams4("r4rk1/8/8/8/8/8/8/7R b - - 0 1", true, 6, true),
		new TestParams4("1r6/8/8/8/8/8/8/2KR3R w - - 0 1", true, 58, true),
		new TestParams4("2r5/8/8/8/8/8/8/2KR3R w - - 0 1", true, 58, false),
		new TestParams4("3r4/8/8/8/8/8/8/2KR3R w - - 0 1", true, 58, false),
		new TestParams4("4r3/8/8/8/8/8/8/2KR3R w - - 0 1", true, 58, false),
		new TestParams4("4r3/8/8/8/8/8/8/R4RK1 w - - 0 1", true, 62, false),
		new TestParams4("5r2/8/8/8/8/8/8/R4RK1 w - - 0 1", true, 62, false),
		new TestParams4("6r1/8/8/8/8/8/8/R4RK1 w - - 0 1", true, 62, false),
		new TestParams4("7r/8/8/8/8/8/8/R4RK1 w - - 0 1", true, 62, true)
	)
);


TEST(GameTest, duplicateTest) {
	const string fenBoard = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 50 25";
	Game* game = FEN::fenToNewGame(fenBoard);
	game->setLastMove(MoveHelper::getMove("b7-a6", BLACK));
	game->verifyChecks();

	Game* newGame = game->duplicate();

	EXPECT_EQ(FEN::gameToFEN(*newGame), fenBoard);
	EXPECT_EQ(newGame->getMovesHistory().size(), 1);
	GTEST_ASSERT_FALSE(MoveHelper::isWhite(newGame->getMovesHistory().front()));
	EXPECT_EQ(MoveHelper::toString(newGame->getMovesHistory().front()), "b7-a6");
	EXPECT_EQ(BoardUtils::positionsCount(newGame->getCheckStatus().getAllCheckPositions()), 25);
	EXPECT_EQ(newGame->getCheckStatus().getCheckPositions().size(), 16);
	EXPECT_EQ(newGame->getCheckStatus().getXRayPositions().size(), 5);
	GTEST_ASSERT_FALSE(newGame->getCheckStatus().isCheck());
	GTEST_ASSERT_FALSE(newGame->getCheckStatus().isDiscoveryCheck());
	GTEST_ASSERT_FALSE(newGame->getCheckStatus().isDoubleCheck());
	GTEST_ASSERT_FALSE(newGame->getCheckStatus().isCheckmate());
}


class TestParams5 {
public:
    TestParams5(string fenBoard, Position source, Position destination) {
        this->fenBoard = fenBoard;
        this->sourcePosition = source;
        this->destinationPosition = destination;
    }

    string fenBoard;
    Position sourcePosition;
    Position destinationPosition;
};

class SimulateAndUndoMoveTest : public ::testing::TestWithParam<TestParams5*> {};

TEST_P(SimulateAndUndoMoveTest, checkControlTest) {
    TestParams5* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenBoard);
    Move move = MoveHelper::getMove(params->sourcePosition, params->destinationPosition, game->isWhiteToMove());
    MoveHelper::decorate(move, game->getPiece(params->sourcePosition), game->getEnPassantPosition(), game->isWhiteToMove());
    Board board;
    board.set(game->getBoard());
    game->simulateMove(move);
    game->undoSimulateMove(move);

    EXPECT_EQ(params->fenBoard, FEN::gameToFEN(*game));

    for (int i = 0; i < SIZE; ++i) {
        EXPECT_EQ(board.pieceBoards[i], game->getBoard().pieceBoards[i]);
    }

    for (int i = 0; i < 64; ++i) {
        EXPECT_EQ(board.piecePositions[i], game->getBoard().piecePositions[i]);
    }
}

INSTANTIATE_TEST_SUITE_P(
        GameTest,
        SimulateAndUndoMoveTest,
        ::testing::Values(
                new TestParams5(Positions::INITIAL_FEN_POSITION, 52, 36),                                   // normal move
                new TestParams5("2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K b - b3 0 23", 32, 41),  // en passant
                new TestParams5("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1", 60, 58),                                // white castling
                new TestParams5("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1", 60, 62),                                // white castling
                new TestParams5("r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1", 4, 2),                                  // black castling
                new TestParams5("r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1", 4, 6),                                  // black castling
                new TestParams5("2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K b - b3 0 23", 9, 33),   // capturing
                new TestParams5("8/P7/8/8/8/8/8/8 w - - 0 1", 8, 0),                                        // promotion
                new TestParams5("1b6/P7/8/8/8/8/8/8 w - - 0 1", 8, 1)                                       // capturing and promotion
        )
);
