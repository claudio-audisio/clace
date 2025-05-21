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
		return QUEEN_WT * (game.board.getPieceCount(WQueen) - game.board.getPieceCount(BQueen)) +
				ROOK_WT * (game.board.getPieceCount(WRook) - game.board.getPieceCount(BRook)) +
				BISHOP_WT * (game.board.getPieceCount(WBishop) - game.board.getPieceCount(BBishop)) +
				KNIGHT_WT * (game.board.getPieceCount(WKnight) - game.board.getPieceCount(BKnight)) +
				PAWN_WT * (game.board.getPieceCount(WPawn) - game.board.getPieceCount(BPawn));
	}

	static int calculateMobility(Game& game) {
		return game.getAllDestinationQty(WHITE) - game.getAllDestinationQty(BLACK);
	}
};