#pragma once

#include "ievaluator.h"
#include "../common/constants.h"
#include "../move/move.h"
#include "../game/game.h"


class BasicEvaluator : public IEvaluator {
public:
	double evaluate(Game& game) override {
		const double evaluation = (calculateMaterialScore(game) * MATERIAL_WT) + (static_cast<double>(calculateMobility(game)) * MOBILITY_WT);

		if (game.isWhiteToMove()) {
			return evaluation;
		}

		return -evaluation;
	}

	static double calculateMaterialScore(Game& game) {
		return QUEEN_WT * (static_cast<double>(getPieceCount(game.board, WQueen)) - static_cast<double>(getPieceCount(game.board, BQueen))) +
				ROOK_WT * (static_cast<double>(getPieceCount(game.board, WRook)) - static_cast<double>(getPieceCount(game.board, BRook))) +
				BISHOP_WT * (static_cast<double>(getPieceCount(game.board, WBishop)) - static_cast<double>(getPieceCount(game.board, BBishop))) +
				KNIGHT_WT * (static_cast<double>(getPieceCount(game.board, WKnight)) - static_cast<double>(getPieceCount(game.board, BKnight))) +
				PAWN_WT * (static_cast<double>(getPieceCount(game.board, WPawn)) - static_cast<double>(getPieceCount(game.board, BPawn)));
	}

	static int calculateMobility(Game& game) {
		return game.getAllDestinationQty(_WHITE) - game.getAllDestinationQty(_BLACK);
	}
};