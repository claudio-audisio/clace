#pragma once

#include <cmath>

#include "evaluator.h"
#include "../common/constants.h"
#include "../move/move.h"
#include "../game/game.h"


class BasicEvaluator : public Evaluator {
public:
	Evaluation evaluate(Game& game, const unsigned int depth) override {
		const double evaluation = (calculateMaterialScore(game) * MATERIAL_WT +
			static_cast<double>(calculateMobility(game)) * MOBILITY_WT);	// TODO questo costa una madonna

		return {0, evaluation * signForSide(game.sideToMove), NONE, depth, createPvMoves(depth)};
	}

	Evaluation evaluateEndGame(Game& game, const unsigned int depth, const EndGameType endGame) override {
		const double evaluation = endGame == CHECKMATE ? game.sideToMove ? WIN_VALUE : LOSS_VALUE : DRAW_VALUE;
		return {0, evaluation * signForSide(game.sideToMove), endGame, depth, createPvMoves(depth)};
	}

	static Move *createPvMoves(const unsigned int size) {
		if (size) {
			auto pvMoves = new Move[size];
			memset(pvMoves, 0, size * sizeof(Move));
			return pvMoves;
		}

		return nullptr;
	}

	static void setPVMove(const Evaluation& eval, const unsigned int pos, const Move move) {
		eval.pvMoves[pos] = move;
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