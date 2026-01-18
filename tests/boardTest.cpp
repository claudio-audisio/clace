#include <gtest/gtest.h>

#include "../board/board.h"
#include "../utils/fen.h"
#include "testUtils.h"
#include "../move/movesCalculation.h"
#include "../move/move.h"
#include "move/movesGenerator.h"

using namespace std;


class BoardTest : public testing::Test {
protected:
	BoardTest() {
	    initMovesGenerator();
	}
	~BoardTest() {

	}

    static void checkPositionCount(const Board *board, unsigned int counts[]) {
	    EXPECT_EQ(getPieceCount(board, Empty), counts[Empty]);
	    EXPECT_EQ(getPieceCount(board, WPawn), counts[WPawn]);
	    EXPECT_EQ(getPieceCount(board, BPawn), counts[BPawn]);
	    EXPECT_EQ(getPieceCount(board, WKnight), counts[WKnight]);
	    EXPECT_EQ(getPieceCount(board, BKnight), counts[BKnight]);
	    EXPECT_EQ(getPieceCount(board, WBishop), counts[WBishop]);
	    EXPECT_EQ(getPieceCount(board, BBishop), counts[BBishop]);
	    EXPECT_EQ(getPieceCount(board, WRook), counts[WRook]);
	    EXPECT_EQ(getPieceCount(board, BRook), counts[BRook]);
	    EXPECT_EQ(getPieceCount(board, WQueen), counts[WQueen]);
	    EXPECT_EQ(getPieceCount(board, BQueen), counts[BQueen]);
	    EXPECT_EQ(getPieceCount(board, WKing), counts[WKing]);
	    EXPECT_EQ(getPieceCount(board, BKing), counts[BKing]);
	}
};

TEST_F(BoardTest, ConstructorTest) {
	Board *board = new Board();
    reset(board);
	EXPECT_EQ(board->pieceBoards[0], 0xffffffffffffffff);
	for (RawboardIndex i = 1; i < 13; i++) {
		EXPECT_EQ(board->pieceBoards[i], Empty);
	}
	EXPECT_EQ(board->castlingInfo, 0);
	EXPECT_EQ(board->enPassantPosition, NO_POS);
    unsigned int expected_counts[] = {64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkPositionCount(board, expected_counts);
}

TEST_F(BoardTest, allBoardsTest) {
    Game* game = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    Board *board = game->board;
    EXPECT_EQ(~board->pieceBoards[Empty], PIECES(board, _WHITE) | PIECES(board, _BLACK));
	EXPECT_EQ(~PIECES(board, _WHITE), board->pieceBoards[Empty] | OPP_PIECES(board, _WHITE));
	EXPECT_EQ(~PIECES(board, _BLACK), board->pieceBoards[Empty] | OPP_PIECES(board, _BLACK));
    unsigned int expected_counts[] = {32, 8, 8, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1};
    checkPositionCount(board, expected_counts);
    delete game;
}

TEST_F(BoardTest, equalsTest) {
    Game* game = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    const Board *board = game->board;
    Game* game2 = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    const Board *board2 = game2->board;

    ASSERT_TRUE(equals(board, board2));

    Move move = createMove(48, 32, _WHITE, WPawn, NO_POS);
    game->applyMove(move);
    game2->applyMove(move);

    ASSERT_TRUE(equals(board, board2));

    Move move2 = createMove(15, 31, _BLACK, BPawn, NO_POS);
    game->applyMove(move2);
    Move move2_2 = createMove(15, 23, _BLACK, BPawn, NO_POS);
    game2->applyMove(move2_2);

    ASSERT_FALSE(equals(board, board2));
}

TEST_F(BoardTest, getPieceCountTest) {
    Game* game = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    Board *board = game->board;
    EXPECT_EQ(getPieceCount(board, Empty), 32);
    EXPECT_EQ(getPieceCount(board, WPawn), 8);
    EXPECT_EQ(getPieceCount(board, WBishop), 2);
    EXPECT_EQ(getPieceCount(board, WKnight), 2);
    EXPECT_EQ(getPieceCount(board, WRook), 2);
    EXPECT_EQ(getPieceCount(board, WQueen), 1);
    EXPECT_EQ(getPieceCount(board, WKing), 1);
    EXPECT_EQ(getPieceCount(board, BPawn), 8);
    EXPECT_EQ(getPieceCount(board, BBishop), 2);
    EXPECT_EQ(getPieceCount(board, BKnight), 2);
    EXPECT_EQ(getPieceCount(board, BRook), 2);
    EXPECT_EQ(getPieceCount(board, BQueen), 1);
    EXPECT_EQ(getPieceCount(board, BKing), 1);
    delete game;
}

TEST_F(BoardTest, isEmptyTest) {
    Board *board = new Board();
    reset(board);
	for (Position i = 0; i < 64; ++i) {
		GTEST_ASSERT_TRUE(isEmpty(board, i));
	}
	setPiece(board, 5, WPawn);
	GTEST_ASSERT_FALSE(isEmpty(board, 5));
	setEmpty(board, 5);
	GTEST_ASSERT_TRUE(isEmpty(board, 5));
}

TEST_F(BoardTest, isWhiteTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(isWhite(board, i));
    }
    setPiece(board, 0, WPawn);
    setPiece(board, 7, BPawn);
    setPiece(board, 56, BPawn);
    setPiece(board, 63, WPawn);

    GTEST_ASSERT_TRUE(isWhite(board, 0));
    GTEST_ASSERT_FALSE(isWhite(board, 7));
    GTEST_ASSERT_FALSE(isWhite(board, 56));
    GTEST_ASSERT_TRUE(isWhite(board, 63));
}

TEST_F(BoardTest, isBlackTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(isBlack(board, i));
    }
    setPiece(board, 0, WPawn);
    setPiece(board, 7, BPawn);
    setPiece(board, 56, BPawn);
    setPiece(board, 63, WPawn);

    GTEST_ASSERT_FALSE(isBlack(board, 0));
    GTEST_ASSERT_TRUE(isBlack(board, 7));
    GTEST_ASSERT_TRUE(isBlack(board, 56));
    GTEST_ASSERT_FALSE(isBlack(board, 63));
}

TEST_F(BoardTest, isPawnTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(isPawn(board, i));
    }
    EXPECT_EQ(setPiece(board, 0, BPawn), Empty);
    GTEST_ASSERT_TRUE(isPawn(board, 0));
    EXPECT_EQ(setPiece(board, 0, WPawn), BPawn);
    GTEST_ASSERT_TRUE(isPawn(board, 0));
    EXPECT_EQ(setPiece(board, 0, WRook), WPawn);
    GTEST_ASSERT_FALSE(isPawn(board, 0));
}

TEST_F(BoardTest, isKnightTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(isKnight(board, i));
    }
    EXPECT_EQ(setPiece(board, 7, BKnight), Empty);
    GTEST_ASSERT_TRUE(isKnight(board, 7));
    EXPECT_EQ(setPiece(board, 7, WKnight), BKnight);
    GTEST_ASSERT_TRUE(isKnight(board, 7));
    EXPECT_EQ(setPiece(board, 7, WRook), WKnight);
    GTEST_ASSERT_FALSE(isKnight(board, 7));
}

TEST_F(BoardTest, isBishopTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(isBishop(board, i));
    }
    EXPECT_EQ(setPiece(board, 0, BBishop), Empty);
    GTEST_ASSERT_TRUE(isBishop(board, 0));
    EXPECT_EQ(setPiece(board, 0, WBishop), BBishop);
    GTEST_ASSERT_TRUE(isBishop(board, 0));
    EXPECT_EQ(setPiece(board, 0, WRook), WBishop);
    GTEST_ASSERT_FALSE(isBishop(board, 0));
}

TEST_F(BoardTest, isRookTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(isRook(board, i));
    }
    EXPECT_EQ(setPiece(board, 0, BRook), Empty);
    GTEST_ASSERT_TRUE(isRook(board, 0));
    EXPECT_EQ(setPiece(board, 0, WRook), BRook);
    GTEST_ASSERT_TRUE(isRook(board, 0));
    EXPECT_EQ(setPiece(board, 0, WPawn), WRook);
    GTEST_ASSERT_FALSE(isRook(board, 0));
}

TEST_F(BoardTest, isRookWithSideTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(isRook(board, i));
    }
    EXPECT_EQ(setPiece(board, 0, BRook), Empty);
    GTEST_ASSERT_FALSE(isRook(board, 0, _WHITE));
    GTEST_ASSERT_TRUE(isRook(board, 0, _BLACK));
    EXPECT_EQ(setPiece(board, 0, WRook), BRook);
    GTEST_ASSERT_TRUE(isRook(board, 0, _WHITE));
    GTEST_ASSERT_FALSE(isRook(board, 0, _BLACK));
    EXPECT_EQ(setPiece(board, 0, WPawn), WRook);
    GTEST_ASSERT_FALSE(isRook(board, 0, _WHITE));
    GTEST_ASSERT_FALSE(isRook(board, 0, _BLACK));
}

TEST_F(BoardTest, isQueenTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(isQueen(board, i));
    }
    EXPECT_EQ(setPiece(board, 0, BQueen), Empty);
    GTEST_ASSERT_TRUE(isQueen(board, 0));
    EXPECT_EQ(setPiece(board, 0, WQueen), BQueen);
    GTEST_ASSERT_TRUE(isQueen(board, 0));
    EXPECT_EQ(setPiece(board, 0, WRook), WQueen);
    GTEST_ASSERT_FALSE(isQueen(board, 0));
}

TEST_F(BoardTest, isKingTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        GTEST_ASSERT_FALSE(isKing(board, i));
    }
    EXPECT_EQ(setPiece(board, 0, BKing), Empty);
    GTEST_ASSERT_TRUE(isKing(board, 0));
    EXPECT_EQ(setPiece(board, 0, WKing), BKing);
    GTEST_ASSERT_TRUE(isKing(board, 0));
    EXPECT_EQ(setPiece(board, 0, WRook), WKing);
    GTEST_ASSERT_FALSE(isKing(board, 0));
}

TEST_F(BoardTest, getKingPositionTest) {
    Game* game = FEN::fenToNewGame(INITIAL_FEN_POSITION);
    Board *board = game->board;
    EXPECT_EQ(getWhiteKingPosition(board), 4);
    EXPECT_EQ(getBlackKingPosition(board), 60);
    delete game;
}

TEST_F(BoardTest, getPieceTest) {
    Board *board = new Board();
    reset(board);
    for (Position i = 0; i < 63; ++i) {
        EXPECT_EQ(getPiece(board, i), Empty);
    }
	setPiece(board, 0, WPawn);
	setPiece(board, 1, BPawn);
	setPiece(board, 63, WRook);
    EXPECT_EQ(getPiece(board, 0), WPawn);
    EXPECT_EQ(getPiece(board, 1), BPawn);
    EXPECT_EQ(getPiece(board, 63), WRook);
}

TEST_F(BoardTest, setPieceTest) {
    Board *board = new Board();
    reset(board);
    Piece oldPiece = setPiece(board, 4, BKing);
    EXPECT_EQ(oldPiece, Empty);
	EXPECT_EQ(board->piecePositions[4], BKing);
	EXPECT_EQ(board->pieceBoards[BKing], 0x10);
	EXPECT_EQ(board->pieceBoards[Empty], 0xFFFFFFFFFFFFFFEF);
    unsigned int expected_counts[] = {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    checkPositionCount(board, expected_counts);
    oldPiece = setPiece(board, 4, WQueen);
    EXPECT_EQ(oldPiece, BKing);
	EXPECT_EQ(board->piecePositions[4], WQueen);
	EXPECT_EQ(board->pieceBoards[WQueen], 0x10);
	EXPECT_EQ(board->pieceBoards[BKing], 0);
	EXPECT_EQ(board->pieceBoards[Empty], 0xFFFFFFFFFFFFFFEF);
    unsigned int expected_counts2[] = {63, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
    checkPositionCount(board, expected_counts2);
}

TEST_F(BoardTest, setEmptyTest) {
	Board *board = new Board();
    reset(board);
	Piece oldPiece = setPiece(board, 4, BKing);
	EXPECT_EQ(oldPiece, Empty);
	oldPiece = setEmpty(board, 4);
	EXPECT_EQ(oldPiece, BKing);
	EXPECT_EQ(board->piecePositions[4], Empty);
	EXPECT_EQ(board->pieceBoards[BKing], 0);
	EXPECT_EQ(board->pieceBoards[Empty], 0xFFFFFFFFFFFFFFFF);
    unsigned int expected_counts[] = {64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkPositionCount(board, expected_counts);
}

// TODO aggiungere i test mancanti

TEST_F(BoardTest, moveTest) {
    Board *board = new Board();
    reset(board);
    board->enPassantPosition = 40;
    board->castlingInfo = 0b1100;
    setPiece(board, 4, WKing);
    Piece oldPiece = movePiece(board, 4, 12, WKing);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(isEmpty(board, 4));
    EXPECT_EQ(getPiece(board, 12), WKing);
    EXPECT_EQ(board->enPassantPosition, NO_POS);
    EXPECT_EQ(board->castlingInfo, 0b0000);
    unsigned int expected_counts[] = {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
    checkPositionCount(board, expected_counts);

    setPiece(board, 19, BPawn);
    oldPiece = movePiece(board, 19, 12, BPawn);

    EXPECT_EQ(oldPiece, WKing);
    GTEST_ASSERT_TRUE(isEmpty(board, 19));
    EXPECT_EQ(getPiece(board, 12), BPawn);
    unsigned int expected_counts2[] = {63, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkPositionCount(board, expected_counts2);

    setPiece(board, 48, BPawn);
    oldPiece = movePiece(board, 48, 32, BPawn);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(isEmpty(board, 48));
    EXPECT_EQ(getPiece(board, 32), BPawn);
    EXPECT_EQ(board->enPassantPosition, 40);
    unsigned int expected_counts3[] = {62, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkPositionCount(board, expected_counts3);
}

TEST_F(BoardTest, moveWithWQCastlingTest) {
    Board *board = new Board();
    reset(board);
    board->enPassantPosition = 40;
    board->castlingInfo = 0b1111;

    setPiece(board, 4, WKing);
    setPiece(board, 0, WRook);
    setPiece(board, 7, WRook);
    Piece oldPiece = movePiece(board, 4, 2, WKing, CASTLING);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(isEmpty(board, 0));
    GTEST_ASSERT_TRUE(isEmpty(board, 1));
    GTEST_ASSERT_TRUE(isEmpty(board, 4));
    EXPECT_EQ(getPiece(board, 2), WKing);
    EXPECT_EQ(getPiece(board, 3), WRook);
    EXPECT_EQ(board->castlingInfo, 0b0011);
    EXPECT_EQ(board->enPassantPosition, NO_POS);
    unsigned int expected_counts[] = {61, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0};
    checkPositionCount(board, expected_counts);
}

TEST_F(BoardTest, moveWithWKCastlingTest) {
    Board *board = new Board();
    reset(board);
    board->enPassantPosition = 40;
    board->castlingInfo = 0b1111;

    setPiece(board, 4, WKing);
    setPiece(board, 0, WRook);
    setPiece(board, 7, WRook);
    Piece oldPiece = movePiece(board, 4, 6, WKing, CASTLING);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(isEmpty(board, 4));
    GTEST_ASSERT_TRUE(isEmpty(board, 7));
    EXPECT_EQ(getPiece(board, 6), WKing);
    EXPECT_EQ(getPiece(board, 5), WRook);
    EXPECT_EQ(board->castlingInfo, 0b0011);
    EXPECT_EQ(board->enPassantPosition, NO_POS);
    unsigned int expected_counts[] = {61, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0};
    checkPositionCount(board, expected_counts);
}

TEST_F(BoardTest, moveWithBQCastlingTest) {
    Board *board = new Board();
    reset(board);
    board->enPassantPosition = 40;
    board->castlingInfo = 0b1111;

    setPiece(board, 60, BKing);
    setPiece(board, 56, BRook);
    setPiece(board, 63, BRook);
    Piece oldPiece = movePiece(board, 60, 58, BKing, CASTLING);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(isEmpty(board, 56));
    GTEST_ASSERT_TRUE(isEmpty(board, 57));
    GTEST_ASSERT_TRUE(isEmpty(board, 60));
    EXPECT_EQ(getPiece(board, 58), BKing);
    EXPECT_EQ(getPiece(board, 59), BRook);
    EXPECT_EQ(board->castlingInfo, 0b1100);
    EXPECT_EQ(board->enPassantPosition, NO_POS);
    unsigned int expected_counts[] = {61, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1};
    checkPositionCount(board, expected_counts);
}

TEST_F(BoardTest, moveWithBKCastlingTest) {
    Board *board = new Board();
    reset(board);
    board->enPassantPosition = 40;
    board->castlingInfo = 0b1111;

    setPiece(board, 60, BKing);
    setPiece(board, 56, BRook);
    setPiece(board, 63, BRook);
    Piece oldPiece = movePiece(board, 60, 62, BKing, CASTLING);

    EXPECT_EQ(oldPiece, Empty);
    GTEST_ASSERT_TRUE(isEmpty(board, 63));
    GTEST_ASSERT_TRUE(isEmpty(board, 60));
    EXPECT_EQ(getPiece(board, 62), BKing);
    EXPECT_EQ(getPiece(board, 61), BRook);
    EXPECT_EQ(board->castlingInfo, 0b1100);
    EXPECT_EQ(board->enPassantPosition, NO_POS);
    unsigned int expected_counts[] = {61, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1};
    checkPositionCount(board, expected_counts);
}

TEST_F(BoardTest, moveEnPassant) {
    Board *board = new Board();
    reset(board);
    board->castlingInfo = 0b1111;
    board->enPassantPosition = 41;

    setPiece(board, 32, WPawn);
    setPiece(board, 33, BPawn);
    Piece oldPiece = movePiece(board, 32, 41, WPawn, EN_PASSANT);

    EXPECT_EQ(oldPiece, BPawn);
    GTEST_ASSERT_TRUE(isEmpty(board, 32));
    GTEST_ASSERT_TRUE(isEmpty(board, 33));
    EXPECT_EQ(getPiece(board, 41), WPawn);
    EXPECT_EQ(board->castlingInfo, 0b1111);
    EXPECT_EQ(board->enPassantPosition, NO_POS);
    unsigned int expected_counts[] = {63, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkPositionCount(board, expected_counts);
}

TEST_F(BoardTest, movePawnPromotion) {
    Board *board = new Board();
    reset(board);
    board->castlingInfo = 0b1111;
    board->enPassantPosition = 40;

    setPiece(board, 8, WPawn);
    setPiece(board, 1, BPawn);
    EXPECT_THROW(movePiece(board, 8, 1, WPawn, PROMOTION), runtime_error);

    Piece oldPiece = movePiece(board, 8, 1, WPawn, PROMOTION, WQueen);

    EXPECT_EQ(oldPiece, BPawn);
    GTEST_ASSERT_TRUE(isEmpty(board, 8));
    EXPECT_EQ(getPiece(board, 1), WQueen);
    EXPECT_EQ(board->castlingInfo, 0b1111);
    EXPECT_EQ(board->enPassantPosition, NO_POS);
    unsigned int expected_counts[] = {63, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
    checkPositionCount(board, expected_counts);
}

TEST_F(BoardTest, copyTest) {
    Board *board = new Board();
    reset(board);
    Board *newboard = new Board();
    reset(newboard);
    setPiece(newboard, 63, WPawn);
    copy(newboard, board);

    GTEST_ASSERT_FALSE(isEmpty(board, 63));
    GTEST_ASSERT_TRUE(isWhite(board, 63));
    GTEST_ASSERT_FALSE(isBlack(board, 63));
    GTEST_ASSERT_TRUE(isPawn(board, 63));
    unsigned int expected_counts[] = {63, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkPositionCount(newboard, expected_counts);
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
