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
		return QUEEN_WT * (static_cast<double>(game.board.getPieceCount(WQueen)) - static_cast<double>(game.board.getPieceCount(BQueen))) +
				ROOK_WT * (static_cast<double>(game.board.getPieceCount(WRook)) - static_cast<double>(game.board.getPieceCount(BRook))) +
				BISHOP_WT * (static_cast<double>(game.board.getPieceCount(WBishop)) - static_cast<double>(game.board.getPieceCount(BBishop))) +
				KNIGHT_WT * (static_cast<double>(game.board.getPieceCount(WKnight)) - static_cast<double>(game.board.getPieceCount(BKnight))) +
				PAWN_WT * (static_cast<double>(game.board.getPieceCount(WPawn)) - static_cast<double>(game.board.getPieceCount(BPawn)));
	}

	static int calculateMobility(Game& game) {
		return game.getAllDestinationQty(_WHITE) - game.getAllDestinationQty(_BLACK);
	}
};