#pragma once

#include "../common/types.h"

/* first implementation for sliding pieces attacks */
inline Rawboard slidingAttack(Rawboard(*direction)(Rawboard), const Rawboard position, const Rawboard occupiedBoard) const {
    Rawboard positions = 0;
    Rawboard newPos = position;

    while (newPos) {
        const Rawboard attack = direction(newPos);
        const Rawboard emptyPos = attack & EMPTY;

        if (!emptyPos) {
            positions |= attack & occupiedBoard;
            break;
        }

        positions |= emptyPos;
        newPos = emptyPos;
    }

    return positions;
}

/*TEST_F(BoardTest, slidingAttackTest) {
    Board board;
    Rawboard posInd = Board::posInd(36);
    EXPECT_EQ(board.slidingAttack(Board::soEastOne, posInd, board.BOARD(WHITE)), 0x8040200000000000LL);
    board.setPiece(54, WPawn);
    EXPECT_EQ(board.slidingAttack(Board::soEastOne, posInd, board.BOARD(WHITE)), 0x40200000000000LL);
    board.setPiece(54, BPawn);
    EXPECT_EQ(board.slidingAttack(Board::soEastOne, posInd, board.BOARD(WHITE)), 0x200000000000LL);
    board.setPiece(29, WPawn);
    GTEST_ASSERT_TRUE(checkBoard(board.slidingAttack(Board::noEastOne, posInd, board.PIECES(WHITE)), 29));
}*/