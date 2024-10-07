#pragma once

#include <list>

#include "move.h"
#include "../game/game.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;

class MovesGenerator {
public:

	static void generateLegalMoves(Game& game, vector<Move>& moves) {
		const Side side = game.getSideToMove();
		Rawboard sources = game.getRawBoard(side);

		/*if (MoveHelper::toString(game.getLastMove()) == "c4-f7") {
			int stop = 1;
		}*/

		while (sources) {
            const Position position = FIRST_POS(sources);
			
			if (game.getCheckStatus().isDoubleCheck() && !game.isKing(position)) {
				sources &= (sources - 1);
				continue;
			}

            const Piece piece = game.getPiece(position);
			const Rawboard opposite = game.getBoard().OPPOSITE(side);
			Rawboard destinations = Positions::getDestinationPositions(game, position, piece, opposite);
            unsigned int count = 0;

			while (destinations) {
                const Position destination = FIRST_POS(destinations);
				Move move = MoveHelper::getMove(position, destination, side);
				MoveHelper::decorate(move, piece, game.getEnPassantPosition(), game.isComputerToMove());

                /*if (MoveHelper::isCastling(move) && game.getMovesHistory().size() == 2) {
                    int stop = 1;
                }*/

                /*if (MoveHelper::isCastling(move) && MoveHelper::toString(move) == "e8-c8" && MoveHelper::toString(game.getLastMove()) == "e5-d7") {
                    int stop = 1;
                }*/

				if (MoveHelper::isPawnPromotion(move)) {
					for (Piece promotion : PieceHelper::getPromotionTypes(side)) {
						Move promotionMove = MoveHelper::getMove(position, destination, side);
						MoveHelper::decorate(promotionMove, piece, game.getEnPassantPosition(), game.isComputerToMove());
						MoveHelper::setPromotion(promotionMove, promotion);
						
						if (isValid(game, promotionMove)) {
                            moves.push_back(promotionMove);
						}/* else {
                            int notValid = 1;
                        }*/
					}
				}
				else if (isValid(game, move)) {
                    /*if (MoveHelper::isCastling(move) && game.getMovesHistory().size() == 2) {
                        cout << game.printMovesHistory() << ", " << MoveHelper::toString(move) << " - " << game.printCastlingInfo() << endl;
                    }*/
					moves.push_back(move);
				}/* else {
                    int notValid = 1;
                }*/

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