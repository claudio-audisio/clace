#include <gtest/gtest.h>

#include "../utils/fen.h"
#include "../game/game.h"
#include "../common/constants.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;


class fenTest : public testing::Test {
protected:
	fenTest() {
		initAttacks();
		initDestPosProviders();
	}
	~fenTest() {

	}
};

TEST_F(fenTest, fenToGameTest) {
	Game game;
	FEN::fenToGame(PERFT_FEN_POSITION_4, game);

	EXPECT_EQ(game.board.getPiece(0), BRook);
	EXPECT_EQ(game.board.getPiece(1), Empty);
	//...
	EXPECT_EQ(game.board.getPiece(62), WKing);
	EXPECT_EQ(game.board.getPiece(63), Empty);

	EXPECT_EQ(game.board.getKingPosition(_WHITE), 62);
	EXPECT_EQ(game.board.getKingPosition(_BLACK), 4);

	GTEST_ASSERT_TRUE(game.isWhiteToMove());

	GTEST_ASSERT_TRUE(FEN::isBlackKingCastling(game.board.castlingInfo));
	GTEST_ASSERT_TRUE(FEN::isBlackQueenCastling(game.board.castlingInfo));
	GTEST_ASSERT_FALSE(FEN::isWhiteKingCastling(game.board.castlingInfo));
	GTEST_ASSERT_FALSE(FEN::isWhiteQueenCastling(game.board.castlingInfo));

	EXPECT_EQ(game.board.enPassantPosition, NO_POS);

	EXPECT_EQ(game.halfMoveClock, 0);
	EXPECT_EQ(game.fullMoves, 1);
}

class FenToGameToFenTest : public ::testing::TestWithParam<string> {};

TEST_P(FenToGameToFenTest, fenToGameToFenTest) {
	string fenPositions = GetParam();

	Game game;
	FEN::fenToGame(fenPositions, game);
	string gameToFEN = FEN::gameToFEN(game);

	EXPECT_EQ(gameToFEN, fenPositions);
}

INSTANTIATE_TEST_SUITE_P(
	fenTest,
	FenToGameToFenTest,
	::testing::Values(
		INITIAL_FEN_POSITION,
		"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
		"rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2",
		"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		PERFT_FEN_POSITION_3,
		"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
		"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1",
		"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
	)
);

class GameToFENKeyTest : public ::testing::TestWithParam<tuple<string, string>> {};

TEST_P(GameToFENKeyTest, gameToFENKeyTest) {
	string fenGame = get<0>(GetParam());
	string expectedFENKey = get<1>(GetParam());
	Game game;
	FEN::fenToGame(fenGame, game);
	EXPECT_EQ(FEN::gameToFENKey(game), expectedFENKey);
}

INSTANTIATE_TEST_SUITE_P(
	fenTest,
	GameToFENKeyTest,
	::testing::Values(
		make_tuple(INITIAL_FEN_POSITION, "rnbqkbnrpppppppp32PPPPPPPPRNBQKBNRwKQkq-"),
		make_tuple(CASTLING_FEN_POSITION, "r3k2rpppppppp32PPPPPPPPR3K2RwKQkq-"),
		make_tuple(PERFT_FEN_POSITION_2, "r3k2rp1ppqpb1bn2pnp4PN4p2P5N2Q1pPPPBBPPPR3K2RwKQkq-"),
		make_tuple(PERFT_FEN_POSITION_3, "10p8p4KP5r1R3p1k12P1P9w--"),
		make_tuple(PERFT_FEN_POSITION_4, "r3k2rPppp1ppp1b3nbNnP6BBP1P3q4N2Pp1P2PPR2Q1RK1wkq-"),
		make_tuple(PERFT_FEN_POSITION_5, "rnbq1k1rpp1Pbppp2p15B13PPP1NnPPRNBQK2RwKQ-"),
		make_tuple(PERFT_FEN_POSITION_6, "r4rk2pp1qpppp1np1n4b1p1B3B1P1b1P1NP1N3PP1QPPPR4RK1w--"),
		make_tuple(END_FEN_POSITION, "4k55K3w--")
	)
);

class MirrorGameTest : public ::testing::TestWithParam<tuple<string, string>> {};

TEST_P(MirrorGameTest, mirrorFenGameTest) {
	string fenGame = get<0>(GetParam());
	string mirroredGame = get<1>(GetParam());
	EXPECT_EQ(FEN::mirrorFenGame(fenGame), mirroredGame);
}

INSTANTIATE_TEST_SUITE_P(
	fenTest,
	MirrorGameTest,
	::testing::Values(
		make_tuple("1n1nq3/8/8/8/8/8/2K5/3Q4 w KQ - 0 1", "4q3/5k2/8/8/8/8/8/3QN1N1 b kq - 0 1")
	)
);