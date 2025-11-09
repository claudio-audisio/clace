#include <gtest/gtest.h>
#include <string>
#include <chrono>
#include <numeric>

#include "../game/game.h"
#include "../utils/fen.h"
#include "../move/movesGenerator.h"
#include "../utils/toString.h"

using namespace std;


class GameTest : public testing::Test {
protected:
	GameTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}
	~GameTest() {

	}
};

TEST_F(GameTest, ConstructorTest) {
	Game game;

	for (Position i = 0; i < 64; i++) {
		GTEST_ASSERT_TRUE(isEmpty(game.board, i));
	}
}

TEST_F(GameTest, initTest) {
	Game game;
	game.init();

	EXPECT_EQ(game.fullMoves, 1);
	EXPECT_EQ(game.halfMoveClock, 0);
	EXPECT_EQ(getKingPosition(game.board, _BLACK), 4);
	EXPECT_EQ(getKingPosition(game.board, _WHITE), 60);
	GTEST_ASSERT_TRUE(game.isWhiteToMove());
	EXPECT_EQ(game.board->enPassantPosition, NO_POS);
	EXPECT_EQ(game.lastMove, 0);
	GTEST_ASSERT_TRUE(FEN::isWhiteQueenCastling(game.board->castlingInfo));
	GTEST_ASSERT_TRUE(FEN::isWhiteKingCastling(game.board->castlingInfo));
	GTEST_ASSERT_TRUE(FEN::isBlackQueenCastling(game.board->castlingInfo));
	GTEST_ASSERT_TRUE(FEN::isBlackKingCastling(game.board->castlingInfo));
	GTEST_ASSERT_TRUE(game.movesHistory.empty());
	EXPECT_EQ(FEN::gameToFEN(game), INITIAL_FEN_POSITION);
}

TEST_F(GameTest, setPieceTest) {
	Game game;
	Piece piece = setPiece(game.board, 0, WPawn);

	EXPECT_EQ(piece, Empty);

	piece = setPiece(game.board, 0, BPawn);

	EXPECT_EQ(piece, WPawn);
}

TEST_F(GameTest, setEmptyTest) {
	Game game;
	setPiece(game.board, 17, WPawn);
	setEmpty(game.board, 17);

	EXPECT_EQ(getPiece(game.board, 17), Empty);
}


class CheckFiftyMoveRuleTest : public ::testing::TestWithParam<tuple<unsigned int, EndGameType>> {};

TEST_P(CheckFiftyMoveRuleTest, checkFiftyMoveRuleTest) {
	Game game;
	unsigned int halfMoveClock = get<0>(GetParam());
	EndGameType expectedEndGame = get<1>(GetParam());
	game.halfMoveClock = halfMoveClock;

	EXPECT_EQ(game.checkFiftyMoveRule(), expectedEndGame);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	CheckFiftyMoveRuleTest,
	::testing::Values(
		make_tuple(0, NONE),
		make_tuple(49, NONE),
		make_tuple(50, FIFTY_MOVE_RULE),
		make_tuple(51, FIFTY_MOVE_RULE)
	)
);


TEST_F(GameTest, isWhiteTest) {
	Game game;
	setPiece(game.board, 0, WPawn);
	setPiece(game.board, 1, BPawn);

	GTEST_ASSERT_TRUE(game.getSide(0) == _WHITE);
	GTEST_ASSERT_TRUE(game.getSide(1) == _BLACK);
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
	setPiece(game.board, 0, WPawn);
	setPiece(game.board, 1, BPawn);
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
	game->sideToMove = side;
	Move move = createMove(testParams->sourcePos, testParams->destinationPos, side, getPiece(game->board, testParams->sourcePos), game->board->enPassantPosition);
	setPromotion(move, side == _WHITE ? WQueen : BQueen);
	const MoveResult moveResult = game->applyMove(move);
	string fenBoardAfterMove = FEN::gameToFEN(*game);

	EXPECT_EQ(fenBoardAfterMove, testParams->expectedFenBoardAfterMove);
	EXPECT_EQ(isCapturedMR(moveResult), testParams->expectedCaptured);
	EXPECT_EQ(isPromotedMR(moveResult), testParams->expectedPromotion);
	EXPECT_EQ(moveToString(game->lastMove), testParams->expectedLastMove);
	EXPECT_EQ(game->movesHistory.size(), 1);
	EXPECT_EQ(moveToString(game->movesHistory.front()), testParams->expectedLastMove);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	ApplyMoveTest,
	::testing::Values(
		//new TestParams2(INITIAL_FEN_POSITION, 48, 40, "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1", false, false, "a2a3"),
		new TestParams2(INITIAL_FEN_POSITION, 48, 32, "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1", false, false, "a2a4"),
		new TestParams2(CASTLING_FEN_POSITION, 4, 2, "2kr3r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQ - 1 2", false, false, "e8c8"),
		new TestParams2(CASTLING_FEN_POSITION, 4, 6, "r4rk1/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQ - 1 2", false, false, "e8g8"),
		new TestParams2(CASTLING_FEN_POSITION, 60, 58, "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq - 1 1", false, false, "e1c1"),
		new TestParams2(CASTLING_FEN_POSITION, 60, 62, "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1", false, false, "e1g1"),
		new TestParams2("8/8/8/8/Pp6/8/8/8 b - a3 0 1", 33, 40, "8/8/8/8/8/p7/8/8 w - - 0 2", true, false, "b4a3"),
		new TestParams2("8/P7/8/8/8/8/8/8 w - - 0 1", 8, 0, "Q7/8/8/8/8/8/8/8 b - - 0 1", false, true, "a7a8"),
		new TestParams2("1b6/P7/8/8/8/8/8/8 w - - 0 1", 8, 1, "1Q6/8/8/8/8/8/8/8 b - - 0 1", true, true, "a7b8"),
		new TestParams2("q7/8/8/8/8/8/8/7Q b - - 0 1", 0, 63, "8/8/8/8/8/8/8/7q w - - 0 2", true, false, "a8h1")
	)
);


// TODO aggiungere test per fast verify check

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

class VerifyChecksTest : public ::testing::TestWithParam<TestParams3*> {
protected:
	VerifyChecksTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}
};

TEST_P(VerifyChecksTest, verifyChecksTest) {
	TestParams3* testParams = GetParam();
	Game* game = FEN::fenToNewGame(testParams->fenBoard);
	
	if (!testParams->lastMove.empty()) {
		game->setLastMove(createMove(testParams->lastMove, _BLACK));
	}

	game->verifyChecks();

	EXPECT_EQ(game->checkStatus.check, testParams->check);
	EXPECT_EQ(game->checkStatus.discoveryCheck, testParams->discoveryCheck);
	EXPECT_EQ(game->checkStatus.doubleCheck, testParams->doubleCheck);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	VerifyChecksTest,
	::testing::Values(
		new TestParams3(INITIAL_FEN_POSITION, "", false, false, false),
		new TestParams3("8/8/8/8/8/8/r7/1r5K w - - 0 1", "b3b1", true, false, false),
		new TestParams3("8/8/8/8/8/4n3/r7/1r5K w - - 0 1", "d1e3", true, true, false),
		new TestParams3("b7/8/8/8/8/8/r7/1r5K w - - 0 1", "b7b1", true, false, true),
		new TestParams3("K7/2p5/1pp5/8/8/8/8/8 w - - 0 1", "b5b6", false, false, false)
	)
);


TEST_F(GameTest, verifyChecksWithEnPassantTest) {
	Game* game = FEN::fenToNewGame("k7/8/8/b7/8/1p6/8/4K3 w - - 0 1");
	Move move = createMove(35, 41, _BLACK, BPawn, 41);
	setCaptured(move, true);
	game->setLastMove(move);

	game->verifyChecks();

	GTEST_ASSERT_TRUE(game->checkStatus.check);
	GTEST_ASSERT_TRUE(game->checkStatus.discoveryCheck);
	GTEST_ASSERT_FALSE(game->checkStatus.doubleCheck);
}


class CheckEndGameTest : public ::testing::TestWithParam<tuple<string, EndGameType>> {
protected:
	CheckEndGameTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}
};

TEST_P(CheckEndGameTest, checkEndGameTest) {
	string fenBoard = get<0>(GetParam());
	EndGameType expectedEndGame = get<1>(GetParam());
	Game* game = FEN::fenToNewGame(fenBoard);
	game->verifyChecks();
	Move* moves = new Move[MAX_MOVES];

	MovesAmount amount;
	generateLegalMoves(*game, moves, &amount);

	EXPECT_EQ(game->checkEndGame(amount.legal), expectedEndGame);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	CheckEndGameTest,
	::testing::Values(
		make_tuple(INITIAL_FEN_POSITION, NONE),
		make_tuple("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 50 25", FIFTY_MOVE_RULE),
		make_tuple("8/8/8/8/8/8/R7/R6k b - - 50 25", CHECKMATE),
		make_tuple("8/8/8/8/8/8/r7/r6K/ w - - 50 25", CHECKMATE),
		make_tuple("k7/2P5/1PP5/8/8/8/8/8 b - - 50 25", STALEMATE)
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

class CheckControlTest : public ::testing::TestWithParam<TestParams4*> {
protected:
	CheckControlTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}
};

TEST_P(CheckControlTest, checkControlTest) {
	TestParams4* params = GetParam();
	Game* game = FEN::fenToNewGame(params->fenBoard);
	Side side = game->getSide(params->kingPosition);
	game->sideToMove = OPPOSITE(side);
	Position sourcePosition = params->isCastling ? side == _WHITE ? 60 : 4 : 0;
	Move move = createMove(sourcePosition, params->kingPosition, side, getPiece(game->board, params->kingPosition), game->board->enPassantPosition);

	EXPECT_EQ(game->checkControl(move), params->expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
	GameTest,
	CheckControlTest,
	::testing::Values(
		new TestParams4(INITIAL_FEN_POSITION, false, 4, true),
		new TestParams4(INITIAL_FEN_POSITION, false, 60, true),
		new TestParams4("k7/8/8/8/8/8/8/R4K1Q b - - 0 1", false, 0, false),
		new TestParams4("q4k1r/8/8/8/8/8/8/7K w - - 0 1", false, 63, false),
		new TestParams4("2kr3r/8/8/8/8/8/8/1R5K b - - 0 1", true, 2, true),
		new TestParams4("2kr3r/8/8/8/8/8/8/2R4K b - - 0 1", true, 2, false),
		new TestParams4("2kr3r/8/8/8/8/8/8/3R3K b - - 0 1", true, 2, false),
		new TestParams4("2kr3r/8/8/8/8/8/8/4R2K b - - 0 1", true, 2, false),
		new TestParams4("r4rk1/8/8/8/8/8/8/4R2K b - - 0 1", true, 6, false),
		new TestParams4("r4rk1/8/8/8/8/8/8/5R1K b - - 0 1", true, 6, false),
		new TestParams4("r4rk1/8/8/8/8/8/8/6RK b - - 0 1", true, 6, false),
		new TestParams4("r4rk1/8/8/8/8/8/8/K6R b - - 0 1", true, 6, true),
		new TestParams4("1r5k/8/8/8/8/8/8/2KR3R w - - 0 1", true, 58, true),
		new TestParams4("2r4k/8/8/8/8/8/8/2KR3R w - - 0 1", true, 58, false),
		new TestParams4("3r3k/8/8/8/8/8/8/2KR3R w - - 0 1", true, 58, false),
		new TestParams4("4r2k/8/8/8/8/8/8/2KR3R w - - 0 1", true, 58, false),
		new TestParams4("4r2k/8/8/8/8/8/8/R4RK1 w - - 0 1", true, 62, false),
		new TestParams4("5r1k/8/8/8/8/8/8/R4RK1 w - - 0 1", true, 62, false),
		new TestParams4("6rk/8/8/8/8/8/8/R4RK1 w - - 0 1", true, 62, false),
		new TestParams4("k6r/8/8/8/8/8/8/R4RK1 w - - 0 1", true, 62, true)
	)
);


TEST_F(GameTest, duplicateTest) {
	const string fenBoard = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 50 25";
	Game* game = FEN::fenToNewGame(fenBoard);
	game->setLastMove(createMove("b7a6", _BLACK));
	game->verifyChecks();

	Game* newGame = game->duplicate();

	EXPECT_EQ(FEN::gameToFEN(*newGame), fenBoard);
	EXPECT_EQ(newGame->movesHistory.size(), 1);
	GTEST_ASSERT_FALSE(isWhite(newGame->movesHistory.front()));
	EXPECT_EQ(moveToString(newGame->movesHistory.front()), "b7a6");
	EXPECT_EQ(positionsCount(newGame->checkStatus.allCheckPositions), 25);
	GTEST_ASSERT_FALSE(newGame->checkStatus.check);
	GTEST_ASSERT_FALSE(newGame->checkStatus.discoveryCheck);
	GTEST_ASSERT_FALSE(newGame->checkStatus.doubleCheck);
	GTEST_ASSERT_FALSE(newGame->checkStatus.checkmate);

	int checkPositionsCount = 0;
	int xRayPositionsCount = 0;
	for (int i = 0; i < 64; ++i) {
		if (newGame->checkStatus.checkPositions[i] != 0) {
			++checkPositionsCount;
		}
		if (newGame->checkStatus.xRayPositions[i] != 0) {
			++xRayPositionsCount;
		}
	}

	EXPECT_EQ(checkPositionsCount, 14);
	EXPECT_EQ(xRayPositionsCount, 5);
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
    Move move = createMove(params->sourcePosition, params->destinationPosition, game->sideToMove, getPiece(game->board, params->sourcePosition), game->board->enPassantPosition);
	setPromotion(move, WQueen);
    Board *board = new Board();
	copy(game->board, board);
    game->simulateMove(move);
    game->undoSimulateMove(move);

    EXPECT_EQ(params->fenBoard, FEN::gameToFEN(*game));

    for (int i = 0; i < SIZE; ++i) {
        EXPECT_EQ(board->pieceBoards[i], game->board->pieceBoards[i]);
    }

    for (int i = 0; i < 64; ++i) {
        EXPECT_EQ(board->piecePositions[i], game->board->piecePositions[i]);
    }
}

INSTANTIATE_TEST_SUITE_P(
        GameTest,
        SimulateAndUndoMoveTest,
        ::testing::Values(
                new TestParams5(INITIAL_FEN_POSITION, 52, 36),                                   // normal move
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

TEST_F(GameTest, getCapturedListTest) {
	Game game;
	game.init();

	movePiece(game.board, 48, 8, WPawn);

	EXPECT_EQ(game.getCapturedList(_WHITE), "");
	EXPECT_EQ(game.getCapturedList(_BLACK), "p ");

	movePiece(game.board, 1, 57, BKnight);

	EXPECT_EQ(game.getCapturedList(_WHITE), "N ");
	EXPECT_EQ(game.getCapturedList(_BLACK), "p ");

	movePiece(game.board, 49, 3, WPawn);
	movePiece(game.board, 57, 50, BKnight);

	EXPECT_EQ(game.getCapturedList(_WHITE), "N P ");
	EXPECT_EQ(game.getCapturedList(_BLACK), "q p ");
}


TEST_F(GameTest, getAllDestinationQty) {
	Game game;
	FEN::fenToGame(INITIAL_FEN_POSITION, game);

	EXPECT_EQ(game.getAllDestinationQty(_WHITE), 20);
	EXPECT_EQ(game.getAllDestinationQty(_BLACK), 20);

	FEN::fenToGame(PERFT_FEN_POSITION_2, game);
	EXPECT_EQ(game.getAllDestinationQty(_WHITE), 48);

	FEN::fenToGame(PERFT_FEN_POSITION_3, game);
	EXPECT_EQ(game.getAllDestinationQty(_WHITE), 16);

	FEN::fenToGame(PERFT_FEN_POSITION_4, game);
	EXPECT_EQ(game.getAllDestinationQty(_WHITE), 38);

	FEN::fenToGame(PERFT_FEN_POSITION_5, game);
	EXPECT_EQ(game.getAllDestinationQty(_WHITE), 41);

	FEN::fenToGame(PERFT_FEN_POSITION_6, game);
	EXPECT_EQ(game.getAllDestinationQty(_WHITE), 46);
}
