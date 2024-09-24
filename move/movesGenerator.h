#pragma once

#include <list>

#include "move.h"
#include "../game/game.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;

class MovesGenerator {
public:
	static void calculateLegalMoves(Game& game, vector<Move>& moves) {
		const bool white = game.isWhiteToMove();
		Rawboard sources = game.getRawBoard(white);

		while (sources) {
            const Position position = countr_zero(sources);
			
			if (game.getCheckStatus().isDoubleCheck() && !game.isKing(position)) {
				sources &= (sources - 1);
				continue;
			}

            const Piece piece = game.getPiece(position);
			Rawboard destinations = Positions::getDestinationPositions(game, position, piece);
            unsigned int count = 0;

			while (destinations) {
                const Position destination = countr_zero(destinations);
				Move move = MoveHelper::getMove(position, destination, white);
				MoveHelper::decorate(move, piece, game.getEnPassantPosition(), game.isComputerToMove());

				if (MoveHelper::isPawnPromotion(move)) {
					for (Piece promotion : PieceHelper::getPromotionTypes(white)) {
						Move promotionMove = MoveHelper::getMove(position, destination, white);
						MoveHelper::decorate(promotionMove, piece, game.getEnPassantPosition(), game.isComputerToMove());
						MoveHelper::setPromotion(promotionMove, promotion);
						
						if (isValid(game, promotionMove)) {
                            moves.push_back(promotionMove);
						}
					}
				}
				else if (isValid(game, move)) {
					moves.push_back(move);
				}

				destinations &= (destinations - 1);
			}

			sources &= (sources - 1);
		}

        // TODO
        // Dovrei tenere una cache con le mosse gia' calcolate a partire da una data scacchiere utilizzando anche enPassant e Castling
        // Se viene fatta una mossa non reversibile allora posso svuotare la cache
        // Come chiave potrei utilizzare una sorta di FEN ristretto
	}

	static bool isValid(Game& game, Move& move) {
		//game.lightSave();
		game.simulateMove(move);
		const bool checkControl = game.checkControl(move);
		//game.lightRollback();
        game.undoSimulateMove(move);

		return checkControl;
	}

};