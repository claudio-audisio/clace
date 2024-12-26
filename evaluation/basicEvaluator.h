#pragma once

#include "ievaluator.h"
#include "../common/constants.h"
#include "../utils/boardUtils.h"

class BasicEvaluator : public IEvaluator {
public:
	double evaluate(Game& game) override {
		return (calculateMaterialScore(game) * MATERIAL_WT) + ((double)calculateMobility(game) * MOBILITY_WT);
	}

	static double calculateMaterialScore(Game& game) {
		return QUEEN_WT * (BoardUtils::positionsCount(game.board.pieceBoards[WQueen]) - BoardUtils::positionsCount(game.board.pieceBoards[BQueen])) +
				ROOK_WT * (BoardUtils::positionsCount(game.board.pieceBoards[WRook]) - BoardUtils::positionsCount(game.board.pieceBoards[BRook])) +
				BISHOP_WT * (BoardUtils::positionsCount(game.board.pieceBoards[WBishop]) - BoardUtils::positionsCount(game.board.pieceBoards[BBishop])) +
				KNIGHT_WT * (BoardUtils::positionsCount(game.board.pieceBoards[WKnight]) - BoardUtils::positionsCount(game.board.pieceBoards[BKnight])) +
				PAWN_WT * (BoardUtils::positionsCount(game.board.pieceBoards[WPawn]) - BoardUtils::positionsCount(game.board.pieceBoards[BPawn]));
	}

	static int calculateMobility(Game& game) {
		return game.getAllDestinationQty(WHITE) - game.getAllDestinationQty(BLACK);
	}
};