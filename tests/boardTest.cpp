#include <gtest/gtest.h>

#include "../board/board.h"
#include "../utils/fen.h"
#include "testUtils.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;


class BoardTest : public testing::Test {
protected:
	BoardTest() {
		initAttacks();
	    initDestPosProviders();
	}
	~BoardTest() {

	}

    static void checkPositionCount(const Board& board, unsigned int counts[]) {
	    EXPECT_EQ(board.getPieceCount(Empty), counts[Empty]);
	    EXPECT_EQ(board.getPieceCount(WPawn), counts[WPawn]);
	    EXPECT_EQ(board.getPieceCount(BPawn), counts[BPawn]);
	    EXPECT_EQ(board.getPieceCount(WKnight), counts[WKnight]);
	    EXPECT_EQ(board.getPieceCount(BKnight), counts[BKnight]);
	    EXPECT_EQ(board.getPieceCount(WBishop), counts[WBishop]);
	    EXPECT_EQ(board.getPieceCount(BBishop), counts[BBishop]);
	    EXPECT_EQ(board.getPieceCount(WRook), counts[WRook]);
	    EXPECT_EQ(board.getPieceCount(BRook), counts[BRook]);
	    EXPECT_EQ(board.getPieceCount(WQueen), counts[WQueen]);
	    EXPECT_EQ(board.getPieceCount(BQueen), counts[BQueen]);
	    EXPECT_EQ(board.getPieceCount(WKing), counts[WKing]);
	    EXPECT_EQ(board.getPieceCount(BKing), counts[BKing]);
	}
};

TEST_F(BoardTest, ConstructorTest) {
	Board board;
	EXPECT_EQ(board.pieceBoards[0], 0xffffffffffffffff);
	for (RawboardIndex i = 1; i < SIZE; i++) {
		EXPECT_EQ(board.pieceBoards[i], Empty);
	}
	EXPECT_EQ(board.castlingInfo, 0);
	EXPECT_EQ(board.enPassantPosition, NO_POS);
    checkPositionCount(board, (unsigned int[]){64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
}

TEST_F(BoardTest, allBoardsTest) {
    Game* game = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    Board& board = game->board;
    EXPECT_EQ(~board.EMPTY, board.PIECES(_WHITE) | board.PIECES(_BLACK));
	EXPECT_EQ(~board.PIECES(_WHITE), board.EMPTY | board.OPP_PIECES(_WHITE));
	EXPECT_EQ(~board.PIECES(_BLACK), board.EMPTY | board.OPP_PIECES(_BLACK));
    checkPositionCount(board, (unsigned int[]){32, 8, 8, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1});
    delete game;
}

TEST_F(BoardTest, equalsTest) {
    Game* game = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    const Board& board = game->board;
    Game* game2 = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    const Board& board2 = game2->board;

    ASSERT_TRUE(board.equals(board2));

    Move move = createMove(48, 32, _WHITE, WPawn, NO_POS);
    game->applyMove(move);
    game2->applyMove(move);

    ASSERT_TRUE(board.equals(board2));

    Move move2 = createMove(15, 31, _BLACK, BPawn, NO_POS);
    game->applyMove(move2);
    Move move2_2 = createMove(15, 23, _BLACK, BPawn, NO_POS);
    game2->applyMove(move2_2);

    ASSERT_FALSE(board.equals(board2));
}

TEST_F(BoardTest, getPieceCountTest) {
    Game* game = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    Board& board = game->board;
    EXPECT_EQ(board.getPieceCount(Empty), 32);
    EXPECT_EQ(board.getPieceCount(WPawn), 8);
    EXPECT_EQ(board.getPieceCount(WBishop), 2);
    EXPECT_EQ(board.getPieceCount(WKnight), 2);
    EXPECT_EQ(board.getPieceCount(WRook), 2);
    EXPECT_EQ(board.getPieceCount(WQueen), 1);
    EXPECT_EQ(board.getPieceCount(WKing), 1);
    EXPECT_EQ(board.getPieceCount(BPawn), 8);
    EXPECT_EQ(board.getPieceCount(BBishop), 2);
    EXPECT_EQ(board.getPieceCount(BKnight), 2);
    EXPECT_EQ(board.getPieceCount(BRook), 2);
    EXPECT_EQ(board.getPieceCount(BQueen), 1);
    EXPECT_EQ(board.getPieceCount(BKing), 1);
    delete game;
}

TEST_F(BoardTest, isEmptyTest) {
	Board board;
	for (Position i = 0; i < 64; ++i) {
		GTEST_ASSERT_TRUE(board.isEmpty(i));
	}
	board.setPiece(5, WPawn);
	GTEST_ASSERT_FALSE(board.isEmpty(5));
	board.setEmpty(5);
	GTEST_ASSERT_TRUE(board.isEmpty(5));
}

TEST_F(BoardTest, isWhiteTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isWhite(i));
    }
    board.setPiece(0, WPawn);
    board.setPiece(7, BPawn);
    board.setPiece(56, BPawn);
    board.setPiece(63, WPawn);

    GTEST_ASSERT_TRUE(board.isWhite(0));
    GTEST_ASSERT_FALSE(board.isWhite(7));
    GTEST_ASSERT_FALSE(board.isWhite(56));
    GTEST_ASSERT_TRUE(board.isWhite(63));
}

TEST_F(BoardTest, isBlackTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isBlack(i));
    }
    board.setPiece(0, WPawn);
    board.setPiece(7, BPawn);
    board.setPiece(56, BPawn);
    board.setPiece(63, WPawn);

    GTEST_ASSERT_FALSE(board.isBlack(0));
    GTEST_ASSERT_TRUE(board.isBlack(7));
    GTEST_ASSERT_TRUE(board.isBlack(56));
    GTEST_ASSERT_FALSE(board.isBlack(63));
}

TEST_F(BoardTest, isPawnTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isPawn(i));
    }
    EXPECT_EQ(board.setPiece(0, BPawn), Empty);
    GTEST_ASSERT_TRUE(board.isPawn(0));
    EXPECT_EQ(board.setPiece(0, WPawn), BPawn);
    GTEST_ASSERT_TRUE(board.isPawn(0));
    EXPECT_EQ(board.setPiece(0, WRook), WPawn);
    GTEST_ASSERT_FALSE(board.isPawn(0));
}

TEST_F(BoardTest, isKnightTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isKnight(i));
    }
    EXPECT_EQ(board.setPiece(7, BKnight), Empty);
    GTEST_ASSERT_TRUE(board.isKnight(7));
    EXPECT_EQ(board.setPiece(7, WKnight), BKnight);
    GTEST_ASSERT_TRUE(board.isKnight(7));
    EXPECT_EQ(board.setPiece(7, WRook), WKnight);
    GTEST_ASSERT_FALSE(board.isKnight(7));
}

TEST_F(BoardTest, isBishopTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isBishop(i));
    }
    EXPECT_EQ(board.setPiece(0, BBishop), Empty);
    GTEST_ASSERT_TRUE(board.isBishop(0));
    EXPECT_EQ(board.setPiece(0, WBishop), BBishop);
    GTEST_ASSERT_TRUE(board.isBishop(0));
    EXPECT_EQ(board.setPiece(0, WRook), WBishop);
    GTEST_ASSERT_FALSE(board.isBishop(0));
}

TEST_F(BoardTest, isRookTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isRook(i));
    }
    EXPECT_EQ(board.setPiece(0, BRook), Empty);
    GTEST_ASSERT_TRUE(board.isRook(0));
    EXPECT_EQ(board.setPiece(0, WRook), BRook);
    GTEST_ASSERT_TRUE(board.isRook(0));
    EXPECT_EQ(board.setPiece(0, WPawn), WRook);
    GTEST_ASSERT_FALSE(board.isRook(0));
}

TEST_F(BoardTest, isRookWithSideTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isRook(i));
    }
    EXPECT_EQ(board.setPiece(0, BRook), Empty);
    GTEST_ASSERT_FALSE(board.isRook(0, _WHITE));
    GTEST_ASSERT_TRUE(board.isRook(0, _BLACK));
    EXPECT_EQ(board.setPiece(0, WRook), BRook);
    GTEST_ASSERT_TRUE(board.isRook(0, _WHITE));
    GTEST_ASSERT_FALSE(board.isRook(0, _BLACK));
    EXPECT_EQ(board.setPiece(0, WPawn), WRook);
    GTEST_ASSERT_FALSE(board.isRook(0, _WHITE));
    GTEST_ASSERT_FALSE(board.isRook(0, _BLACK));
}

TEST_F(BoardTest, isQueenTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isQueen(i));
    }
    EXPECT_EQ(board.setPiece(0, BQueen), Empty);
    GTEST_ASSERT_TRUE(board.isQueen(0));
    EXPECT_EQ(board.setPiece(0, WQueen), BQueen);
    GTEST_ASSERT_TRUE(board.isQueen(0));
    EXPECT_EQ(board.setPiece(0, WRook), WQueen);
    GTEST_ASSERT_FALSE(board.isQueen(0));
}

TEST_F(BoardTest, isKingTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(board.isKing(i));
    }
    EXPECT_EQ(board.setPiece(0, BKing), Empty);
    GTEST_ASSERT_TRUE(board.isKing(0));
    EXPECT_EQ(board.setPiece(0, WKing), BKing);
    GTEST_ASSERT_TRUE(board.isKing(0));
    EXPECT_EQ(board.setPiece(0, WRook), WKing);
    GTEST_ASSERT_FALSE(board.isKing(0));
}

TEST_F(BoardTest, getKingPositionTest) {
    Game* game = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    Board& board = game->board;
    EXPECT_EQ(board.getWhiteKingPosition(), 60);
    EXPECT_EQ(board.getBlackKingPosition(), 4);
    delete game;
}

TEST_F(BoardTest, getPieceTest) {
    Board board;
    for (Position i = 0; i < 63; ++i) {
        EXPECT_EQ(board.getPiece(i), Empty);
    }
	board.setPiece(0, WPawn);
	board.setPiece(1, BPawn);
	board.setPiece(63, WRook);
    EXPECT_EQ(board.getPiece(0), WPawn);
    EXPECT_EQ(board.getPiece(1), BPawn);
    EXPECT_EQ(board.getPiece(63), WRook);
}

TEST_F(BoardTest, setPieceTest) {
    Board board;
    Piece oldPiece = board.setPiece(4, BKing);
    EXPECT_EQ(oldPiece, Empty);
	EXPECT_EQ(board.piecePositions[4], BKing);
	EXPECT_EQ(board.pieceBoards[BKing], 0x10);
	EXPECT_EQ(board.EMPTY, 0xFFFFFFFFFFFFFFEF);
    checkPositionCount(board, (unsigned int[]){63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    oldPiece = board.setPiece(4, WQueen);
    EXPECT_EQ(oldPiece, BKing);
	EXPECT_EQ(board.piecePositions[4], WQueen);
	EXPECT_EQ(board.pieceBoards[WQueen], 0x10);
	EXPECT_EQ(board.pieceBoards[BKing], 0);
	EXPECT_EQ(board.EMPTY, 0xFFFFFFFFFFFFFFEF);
    checkPositionCount(board, (unsigned int[]){63, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0});
}

TEST_F(BoardTest, setEmptyTest) {
	Board board;
	Piece oldPiece = board.setPiece(4, BKing);
	EXPECT_EQ(oldPiece, Empty);
	oldPiece = board.setEmpty(4);
	EXPECT_EQ(oldPiece, BKing);
	EXPECT_EQ(board.piecePositions[4], Empty);
	EXPECT_EQ(board.pieceBoards[BKing], 0);
	EXPECT_EQ(board.EMPTY, 0xFFFFFFFFFFFFFFFF);
    checkPositionCount(board, (unsigned int[]){64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
}

// TODO aggiungere i test mancanti

TEST_F(BoardTest, moveTest) {
    Board board;
    board.enPassantPosition = 40;
    board.castlingInfo = 0b1100;
    board.setPiece(4, BKing);
    Piece oldPiece = board.move(4, 12, BKing);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(board.isEmpty(4));
    EXPECT_EQ(board.getPiece(12), BKing);
    EXPECT_EQ(board.enPassantPosition, NO_POS);
    EXPECT_EQ(board.castlingInfo, 0b1100);
    checkPositionCount(board, (unsigned int[]){63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});

    board.setPiece(19, WPawn);
    oldPiece = board.move(19, 12, WPawn);

    EXPECT_EQ(oldPiece, BKing);
    GTEST_ASSERT_TRUE(board.isEmpty(19));
    EXPECT_EQ(board.getPiece(12), WPawn);
    checkPositionCount(board, (unsigned int[]){63, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

    board.setPiece(48, WPawn);
    oldPiece = board.move(48, 32, WPawn);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(board.isEmpty(48));
    EXPECT_EQ(board.getPiece(32), WPawn);
    EXPECT_EQ(board.enPassantPosition, 40);
    checkPositionCount(board, (unsigned int[]){62, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
}

TEST_F(BoardTest, moveWithBQCastlingTest) {
    Board board;
    board.enPassantPosition = 40;
    board.castlingInfo = 0b1111;

    board.setPiece(4, BKing);
    board.setPiece(0, BRook);
    board.setPiece(7, BRook);
    Piece oldPiece = board.move(4, 2, BKing, CASTLING);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(board.isEmpty(0));
    GTEST_ASSERT_TRUE(board.isEmpty(1));
    GTEST_ASSERT_TRUE(board.isEmpty(4));
    EXPECT_EQ(board.getPiece(2), BKing);
    EXPECT_EQ(board.getPiece(3), BRook);
    EXPECT_EQ(board.castlingInfo, 0b1100);
    EXPECT_EQ(board.enPassantPosition, NO_POS);
    checkPositionCount(board, (unsigned int[]){61, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1});
}

TEST_F(BoardTest, moveWithBKCastlingTest) {
    Board board;
    board.enPassantPosition = 40;
    board.castlingInfo = 0b1111;

    board.setPiece(4, BKing);
    board.setPiece(0, BRook);
    board.setPiece(7, BRook);
    Piece oldPiece = board.move(4, 6, BKing, CASTLING);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(board.isEmpty(4));
    GTEST_ASSERT_TRUE(board.isEmpty(7));
    EXPECT_EQ(board.getPiece(6), BKing);
    EXPECT_EQ(board.getPiece(5), BRook);
    EXPECT_EQ(board.castlingInfo, 0b1100);
    EXPECT_EQ(board.enPassantPosition, NO_POS);
    checkPositionCount(board, (unsigned int[]){61, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1});
}

TEST_F(BoardTest, moveWithWQCastlingTest) {
    Board board;
    board.enPassantPosition = 40;
    board.castlingInfo = 0b1111;

    board.setPiece(60, WKing);
    board.setPiece(56, WRook);
    board.setPiece(63, WRook);
    Piece oldPiece = board.move(60, 58, WKing, CASTLING);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(board.isEmpty(56));
    GTEST_ASSERT_TRUE(board.isEmpty(57));
    GTEST_ASSERT_TRUE(board.isEmpty(60));
    EXPECT_EQ(board.getPiece(58), WKing);
    EXPECT_EQ(board.getPiece(59), WRook);
    EXPECT_EQ(board.castlingInfo, 0b0011);
    EXPECT_EQ(board.enPassantPosition, NO_POS);
    checkPositionCount(board, (unsigned int[]){61, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0});
}

TEST_F(BoardTest, moveWithWKCastlingTest) {
    Board board;
    board.enPassantPosition = 40;
    board.castlingInfo = 0b1111;

    board.setPiece(60, WKing);
    board.setPiece(56, WRook);
    board.setPiece(63, WRook);
    Piece oldPiece = board.move(60, 62, WKing, CASTLING);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(board.isEmpty(63));
    GTEST_ASSERT_TRUE(board.isEmpty(60));
    EXPECT_EQ(board.getPiece(62), WKing);
    EXPECT_EQ(board.getPiece(61), WRook);
    EXPECT_EQ(board.castlingInfo, 0b0011);
    EXPECT_EQ(board.enPassantPosition, NO_POS);
    checkPositionCount(board, (unsigned int[]){61, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0});
}

TEST_F(BoardTest, moveEnPassant) {
    Board board;
    board.castlingInfo = 0b1111;
    board.enPassantPosition = 41;

    board.setPiece(32, BPawn);
    board.setPiece(33, WPawn);
    Piece oldPiece = board.move(32, 41, BPawn, EN_PASSANT);

    EXPECT_EQ(oldPiece, WPawn);
    GTEST_ASSERT_TRUE(board.isEmpty(32));
    GTEST_ASSERT_TRUE(board.isEmpty(33));
    EXPECT_EQ(board.getPiece(41), BPawn);
    EXPECT_EQ(board.castlingInfo, 0b1111);
    EXPECT_EQ(board.enPassantPosition, NO_POS);
    checkPositionCount(board, (unsigned int[]){63, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
}

TEST_F(BoardTest, movePawnPromotion) {
    Board board;
    board.castlingInfo = 0b1111;
    board.enPassantPosition = 40;

    board.setPiece(8, WPawn);
    board.setPiece(1, BPawn);
    EXPECT_THROW(board.move(8, 1, WPawn, PROMOTION), runtime_error);

    Piece oldPiece = board.move(8, 1, WPawn, PROMOTION, WQueen);

    EXPECT_EQ(oldPiece, BPawn);
    GTEST_ASSERT_TRUE(board.isEmpty(8));
    EXPECT_EQ(board.getPiece(1), WQueen);
    EXPECT_EQ(board.castlingInfo, 0b1111);
    EXPECT_EQ(board.enPassantPosition, NO_POS);
    checkPositionCount(board, (unsigned int[]){63, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0});
}

TEST_F(BoardTest, setTest) {
    Board board;
    Board newboard;
    newboard.setPiece(63, WPawn);
    board.set(newboard);

    GTEST_ASSERT_FALSE(board.isEmpty(63));
    GTEST_ASSERT_TRUE(board.isWhite(63));
    GTEST_ASSERT_FALSE(board.isBlack(63));
    GTEST_ASSERT_TRUE(board.isPawn(63));
    checkPositionCount(newboard, (unsigned int[]){63, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
}

class TestParams {
public:
    TestParams(string fenGame, Side side, list<Position>* expectedPositions) {
        this->fenGame = fenGame;
        this->side = side;
        this->expectedPositions = expectedPositions;
    }

    string fenGame;
    Side side;
    list<Position>* expectedPositions;
};
