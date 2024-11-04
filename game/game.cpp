#include <cassert>

#include "game.h"
#include "../utils/fen.h"
#include "../move/move.h"


Game::Game() {
	BoardUtils::initAttacks();
}

Game::~Game() {
	delete whitePlayer;
	delete blackPlayer;
}

void Game::init() {
	initPlayers();
	initFromFEN(INITIAL_FEN_POSITION);
}

void Game::initFromFEN(const string& fenPosition) {
	initPlayers();
	FEN::fenToGame(fenPosition, *this);
}

void Game::initPlayers() {
	whitePlayer = new Player(true);
	blackPlayer = new Player(false);
}

void Game::initPlayers(Player* white, Player* black) {
	whitePlayer = white;
	blackPlayer = black;
}

MoveResult Game::finalizeMove(Move& move) {
	Piece captured = board.move(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move), MoveHelper::getPiece(move), MoveHelper::isCastling(move));
	updateKingPosition(move);
	board.updateCastlingInfo(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move));
	updateEnPassantInfo(move);

    if (MoveHelper::isPawnPromotion(move)) {
        completePawnPromotion(move);
        // TODO rimuovere pawn promosso
    }

    if (captured == Empty) {
        /*if (MoveHelper::isCastling(move)) {
            board.completeCastlingMove(MoveHelper::getDestinationPosition(move));
        }
        else*/ if (MoveHelper::isEnPassant(move)) {
            captured = completeEnPassant(move);
            // TODO rimuovere pawn avversario
        }
    }

    if (captured) {
        MoveHelper::setCaptured(move, captured);
        processCapture(captured, MoveHelper::getSide(move));
    }

	if (captured == Empty && !MoveHelper::isPawnPromotion(move) && !board.isPawn(MoveHelper::getDestinationPosition(move))) {
		halfMoveClock++;
	}
	else {
		halfMoveClock = 0;
	}

	return MoveHelper::getMoveResult(captured != Empty, MoveHelper::isPawnPromotion(move), MoveHelper::isEnPassant(move), MoveHelper::isCastling(move));
}

MoveResult Game::applyMove(Move& move) {
	MoveResult moveResult = finalizeMove(move);
	lastMove = move;
	movesHistory.push_front(move);

	if (!MoveHelper::isWhite(move)) {
		fullMoves++;
	}

	changeTurn();
	return moveResult;
}

void Game::applyMoves(list<Move>& moves) {
    for (reverse_iterator it = moves.rbegin(); it != moves.rend(); it++) {
        applyMove(*it);
    }
}

void Game::simulateMove(Move& move) {
	Piece captured = board.move(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move), MoveHelper::getPiece(move), MoveHelper::isCastling(move));
	updateKingPosition(move);

    if (MoveHelper::isPawnPromotion(move)) {
        completePawnPromotion(move);
    }

    if (captured == Empty) {
        /*if (MoveHelper::isCastling(move)) {
            board.completeCastlingMove(MoveHelper::getDestinationPosition(move));
        }
        else*/ if (MoveHelper::isEnPassant(move)) {
            captured = completeEnPassant(move);
        }
    }

    MoveHelper::setCaptured(move, captured);
}

void Game::undoSimulateMove(Move& move) {
	if (MoveHelper::isCastling(move)) {
		board.undoCastlingMove(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move));
	} else {
		board.setPiece(MoveHelper::getSourcePosition(move), MoveHelper::getPiece(move));
		board.setEmpty(MoveHelper::getDestinationPosition(move));
		const Piece captured = MoveHelper::getCaptured(move);

		if (captured != Empty) {
			if (MoveHelper::isEnPassant(move)) {
				undoEnPassant(move);
			} else {
				board.setPiece(MoveHelper::getDestinationPosition(move), captured);
			}
		}
	}

    // Undo Pawn promotion is not needed because undoing original move we have already moved a Pawn
    undoKingPosition(move);
}

void Game::undoKingPosition(Move& move) {
    if (PieceHelper::isKing(MoveHelper::getPiece(move))) {
        if (MoveHelper::isWhite(move)) {
            whiteKingPosition = MoveHelper::getSourcePosition(move);
        }
        else {
            blackKingPosition = MoveHelper::getSourcePosition(move);
        }
    }
}

/*void Game::undoCastlingMove(Move& move) {
    switch (MoveHelper::getDestinationPosition(move)) {
        case 2: board.move(3, 0, BRook); break;
        case 6: board.move(5, 7, BRook); break;
        case 58: board.move(59, 56, WRook); break;
        case 62: board.move(61, 63, WRook); break;
        default: break;
    }
}*/

void Game::undoEnPassant(Move& move) {
    const Position destination = MoveHelper::getDestinationPosition(move) + (MoveHelper::isWhite(move) ? 8 : -8);
    board.setPiece(destination, MoveHelper::isWhite(move) ? BPawn : WPawn);
}

void Game::verifyChecks() {
    checkStatus.reset();
	Positions::calculateCheckPositions(*this, getOppositeSide());
	const Position kingPosition = isWhiteToMove() ? whiteKingPosition : blackKingPosition;
	checkStatus.updateStatus(kingPosition, movesHistory.empty() ? 0 : movesHistory.front());
}

EndGameType Game::checkEndGame(const bool noMoves) {
	if (noMoves) {
		return checkStatus.check ? EndGameType::CHECKMATE : EndGameType::STALEMATE;
	}

	return checkFiftyMoveRule();
}

EndGameType Game::checkFiftyMoveRule() {
	return halfMoveClock >= 50 ? EndGameType::FIFTY_MOVE_RULE : EndGameType::NONE;
}

bool Game::checkFiveFoldRepetitions() {
	// TODO
	return false;
}

bool Game::checkControl(const Move& move) {
	const Side side = MoveHelper::getSide(move);
	const Rawboard checkBoard = board.getAttacks(BLACK - side);
	const Position kingPosition = !side ? whiteKingPosition : blackKingPosition;

	if (BoardUtils::isUnderCheck(checkBoard, kingPosition)) {
		return false;
	}

	bool castlingNotValid = false;

	if (MoveHelper::isCastling(move)) {
		if (side) {
			switch (MoveHelper::getDestinationPosition(move)) {
				case 2: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 4) || BoardUtils::isUnderCheck(checkBoard, 3); break;
				case 6: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 4) || BoardUtils::isUnderCheck(checkBoard, 5); break;
				default: break;
			}
		}
		else {
			switch (MoveHelper::getDestinationPosition(move)) {
				case 58: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 60) || BoardUtils::isUnderCheck(checkBoard, 59); break;
				case 62: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 60) || BoardUtils::isUnderCheck(checkBoard, 61); break;
				default: break;
			}
		}
	}

	return !castlingNotValid;
}

void Game::setKingPositions() {
	whiteKingPosition = board.getWhiteKingPosition();
	blackKingPosition = board.getBlackKingPosition();
}

void Game::updateKingPosition(const Move& move) {
	if (PieceHelper::isKing(MoveHelper::getPiece(move))) {
		if (MoveHelper::isWhite(move)) {
			whiteKingPosition = MoveHelper::getDestinationPosition(move);
		}
		else {
			blackKingPosition = MoveHelper::getDestinationPosition(move);
		}
	}
}

void Game::updateEnPassantInfo(const Move& move) {
	if (PieceHelper::isPawn(MoveHelper::getPiece(move)) &&
		Positions::isSecondRow(MoveHelper::getSourcePosition(move), MoveHelper::isWhite(move)) &&
		Positions::isFourthRow(MoveHelper::getDestinationPosition(move), MoveHelper::isWhite(move))) {
		board.enPassantPosition = MoveHelper::getSourcePosition(move) + (MoveHelper::isWhite(move) ? -8 : 8);
	}
	else {
		board.enPassantPosition = NO_POS;
	}
}

/*void Game::completeCastlingMove(const Move& move) {
	switch (MoveHelper::getDestinationPosition(move)) {
		case 2: board.move(0, 3, BRook); break;
		case 6: board.move(7, 5, BRook); break;
		case 58: board.move(56, 59, WRook); break;
		case 62: board.move(63, 61, WRook); break;
		default: break;
	}
}*/

Piece Game::completeEnPassant(const Move& move) {
	const Position destination = MoveHelper::getDestinationPosition(move) + (MoveHelper::isWhite(move) ? 8 : -8);
	return board.setEmpty(destination);
}

void Game::completePawnPromotion(const Move& move) {
	const Piece promotionPiece = MoveHelper::getPromotion(move);
	if (promotionPiece == Empty) {
		// TODO tirare eccezione
		//throw new RuntimeException("Pawn promotion piece type not set");
	}

	//System.out.print("Pawn promotion --> " + MoveHelper::getPieceTypeForPawnPromotion().name());
	board.setPiece(MoveHelper::getDestinationPosition(move), promotionPiece);
}

bool Game::processCapture(const Piece piece, const Side side) {
	if (PieceHelper::isEmpty(piece)) {
		return false;
	}

	// TODO move piece capturing management to Board
	if (side == WHITE) {
		whitePlayer->onCaptured(piece);
	}
	else {
		blackPlayer->onCaptured(piece);
	}

	return true;
}

void Game::changeTurn() {
	sideToMove = BoardUtils::opposite(sideToMove);
}

Side Game::getSide(Position position) const {
	if (board.isEmpty(position)) {
		assert(false);
	}

	if (board.isWhite(position)) {
		return WHITE;
	}

	return BLACK;
}

void Game::save() {
	rollback.save(*this);
}

void Game::rollbackLastMove() {
	checkStatus.reset();
	movesHistory.pop_front();
	lastMove = movesHistory.empty() ? 0 : movesHistory.front();
	rollback.rollback(*this);
}

/*
public Evaluation evaluate(Move move) {
	return Optional.ofNullable(evaluator).map(e->e.evaluate(this, move)).orElseGet(() -> new Evaluation(move, Double.NaN));
}

public double evaluateAsWhite() {
	if (lastMove != null) {
		return evaluate(lastMove).getValue() * (isWhiteToMove() ? -1 : 1);
	}

	return 0;
}
*/
Player* Game::getCurrentPlayer() const {
	return isWhiteToMove() ? whitePlayer : blackPlayer;
}

bool Game::isComputerToMove() const {
	Player* player = this->getCurrentPlayer();

	if (!player) {
		return false;
	}
	
	return player->computer;
}

void Game::setLastMove(const Move& move) {
	movesHistory.push_front(move);
	lastMove = move;
}

void Game::setPlayerPieces(const Game& game) {
	whitePlayer->setPieces(game.whitePlayer->pieces);
	blackPlayer->setPieces(game.blackPlayer->pieces);
}

void Game::resetPlayersPieces() {
	whitePlayer->resetPieces();
	blackPlayer->resetPieces();
}

void Game::incrementPlayerPieces(const Piece piece) {
	if (whitePlayer) {
		if (PieceHelper::isWhite(piece)) {
			whitePlayer->incrementPieces(piece);
		}
		else {
			blackPlayer->incrementPieces(piece);
		}
	}
}

Game* Game::duplicate() {
	Game* newGame = new Game();
	newGame->init();
	newGame->board.set(board);
	newGame->board.castlingInfo = board.castlingInfo;
	newGame->sideToMove = sideToMove;
	newGame->whiteKingPosition = whiteKingPosition;
	newGame->blackKingPosition = blackKingPosition;
	newGame->board.enPassantPosition = board.enPassantPosition;
	newGame->fullMoves = fullMoves;
	newGame->halfMoveClock = halfMoveClock;
	Utils::dequeAddAll(movesHistory, newGame->movesHistory);
	newGame->checkStatus.set(checkStatus);
	newGame->setPlayerPieces(*this);
	return newGame;
}

string Game::printMovesHistory() {
    string moves;
    for (Move move : movesHistory) {
        moves = MoveHelper::toString(move) + ", " + moves;
    }
    return moves.substr(0, moves.length() - 2);
}

string Game::printCastlingInfo() const {
    return FEN::castlingInfoToFEN(board.castlingInfo);
}