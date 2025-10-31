#include <cassert>

#include "game.h"
#include "../utils/fen.h"
#include "../move/move.h"
#include "../move/movesGenerator.h"
#include "../game/player.h"
#include "../evaluation/basicEvaluator.h"
#include "../utils/toString.h"
#include "../utils/boardUtils.h"

Game::Game() :
	rollback(10), sideToMove(0), fullMoves(0), halfMoveClock(0) {
	reset(board);
	evaluator = new BasicEvaluator();
}

Game::~Game() {
	delete whitePlayer;
	delete blackPlayer;
	delete evaluator;
}

void Game::init() {
	initFromFEN(INITIAL_FEN_POSITION);
}

void Game::initFromFEN(const string& fenPosition) {
	FEN::fenToGame(fenPosition, *this);
}

void Game::initPlayers(Player* white, Player* black) {
	whitePlayer = white;
	blackPlayer = black;
}

Evaluation Game::calculateMove() {
	return getCurrentPlayer()->engine->calculateMove(*this);
}

MoveResult Game::finalizeMove(Move& move) {
	const Piece captured = movePiece(
		board,
		getSourcePosition(move),
		getDestinationPosition(move),
		getPiece(move),
		getType(move),
		getPromotion(move));
	//const Piece captured = board.move(move);
	setCaptured(move, captured);

	if (captured || getPiece(move) < WKnight) {
		halfMoveClock = 0;
	}
	else {
		halfMoveClock++;
	}

	return getMoveResult(captured != Empty, getType(move));
}

MoveResult Game::applyMove(Move& move) {
	const MoveResult moveResult = finalizeMove(move);
	lastMove = move;
	movesHistory.push_front(move);

	if (!isWhite(move)) {
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
	Piece captured = simulateMovePiece(
		board,
		getSourcePosition(move),
		getDestinationPosition(move),
		getPiece(move),
		isCastling(move));

	if (isPawnPromotion(move)) {
		completePawnPromotion(move);
	}

	if (captured == Empty && isEnPassant(move)) {
		captured = completeEnPassant(move);
	}

	setCaptured(move, captured);
}

Piece Game::completeEnPassant(const Move move) {
	const Position destination = getDestinationPosition(move) + (isWhite(move) ? 8 : -8);
	return setEmpty(board, destination);
}

void Game::completePawnPromotion(const Move move) {
	const Piece promotionPiece = getPromotion(move);

	if (promotionPiece == Empty) {
		throw runtime_error("promotion piece not set");
	}

	//System.out.print("Pawn promotion --> " + getPieceTypeForPawnPromotion().name());
	setPiece(board, getDestinationPosition(move), promotionPiece);
}

void Game::undoSimulateMove(Move& move) {
	if (isCastling(move)) {
		undoCastlingMove(board, getSourcePosition(move), getDestinationPosition(move));
	} else {
		setPiece(board, getSourcePosition(move), getPiece(move));
		setEmpty(board, getDestinationPosition(move));
		const Piece captured = getCaptured(move);

		if (captured != Empty) {
			if (isEnPassant(move)) {
				undoEnPassant(move);
			} else {
				setPiece(board, getDestinationPosition(move), captured);
			}
		}
	}

    // Undo Pawn promotion is not needed because undoing original move we have already moved a Pawn
}

void Game::undoEnPassant(Move& move) {
    const Position destination = getDestinationPosition(move) + (isWhite(move) ? 8 : -8);
    setPiece(board, destination, isWhite(move) ? BPawn : WPawn);
}

void Game::verifyChecks() {
	calculateCheckPositions(getOppositeSide());
	const Position kingPosition = getKingPosition(board, sideToMove);
	checkStatus.updateStatus(kingPosition, movesHistory.empty() ? 0 : movesHistory.front());
}

EndGameType Game::checkEndGame(const unsigned char legalMoves) {
	if (!legalMoves) {
		checkStatus.checkmate = checkStatus.check;
		return checkStatus.check ? CHECKMATE : STALEMATE;
	}

	if (checkFiftyMoveRule()) {
		return FIFTY_MOVE_RULE;
	}

	return checkFiveFoldRepetitions() ? FIVEFOLD_REPETITION : NONE;
}

bool Game::checkFiftyMoveRule() const {
	return halfMoveClock >= 50;
}

bool Game::checkFiveFoldRepetitions() const {
	// TODO
	return false;
}

bool Game::checkControl(const Move move) {
	const Side side = getMoveSide(move);
	const Rawboard checkBoard = allAttacks(board, OPPOSITE(side));
	const Position kingPosition = getKingPosition(board, side);

	if (isUnderCheck(checkBoard, kingPosition)) {
		return false;
	}

	bool castlingNotValid = false;

	if (isCastling(move)) {
		if (side) {
			switch (getDestinationPosition(move)) {
				case 2: castlingNotValid = isUnderCheck(checkBoard, 4) || isUnderCheck(checkBoard, 3); break;
				case 6: castlingNotValid = isUnderCheck(checkBoard, 4) || isUnderCheck(checkBoard, 5); break;
				default: break;
			}
		}
		else {
			switch (getDestinationPosition(move)) {
				case 58: castlingNotValid = isUnderCheck(checkBoard, 60) || isUnderCheck(checkBoard, 59); break;
				case 62: castlingNotValid = isUnderCheck(checkBoard, 60) || isUnderCheck(checkBoard, 61); break;
				default: break;
			}
		}
	}

	return !castlingNotValid;
}

void Game::changeTurn() {
	sideToMove = OPPOSITE(sideToMove);
}

Side Game::getSide(Position position) const {
	if (isEmpty(board, position)) {
		assert(false);
	}

	if (isWhite(board, position)) {
		return _WHITE;
	}

	return _BLACK;
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

Player* Game::getCurrentPlayer() const {
	return isWhiteToMove() ? whitePlayer : blackPlayer;
}

Player* Game::getOtherPlayer() const {
	return isWhiteToMove() ? blackPlayer : whitePlayer;
}

bool Game::isComputerToMove() const {
	Player* player = this->getCurrentPlayer();

	if (!player) {
		return false;
	}
	
	return player->computer;
}

void Game::setLastMove(const Move move) {
	movesHistory.push_front(move);
	lastMove = move;
}

Game* Game::duplicate() {
	Game* newGame = new Game();
	newGame->init();
	copy(&board, &newGame->board);
	newGame->board.castlingInfo = board.castlingInfo;
	newGame->sideToMove = sideToMove;
	newGame->board.enPassantPosition = board.enPassantPosition;
	newGame->fullMoves = fullMoves;
	newGame->halfMoveClock = halfMoveClock;
	dequeAddAll(movesHistory, newGame->movesHistory);
	newGame->checkStatus.set(checkStatus);
	return newGame;
}

string Game::printMovesHistory(const int depth) const {
    string moves;
	int i = 0;
    for (Move move : movesHistory) {
        moves = moveToString(move) + ", " + moves;
    	if (depth > 0 && ++i == depth) {
    		break;
    	}
    }
    return moves.substr(0, moves.length() - 2);
}

string Game::printCastlingInfo() const {
    return FEN::castlingInfoToFEN(board.castlingInfo);
}

string Game::getCapturedList(const Side side) {
	string captured;
	for (int i = 0; i < 1 - positionsCount(board.pieceBoards[WQueen + side]); ++i) {
		captured += getPieceCode(WQueen + side);
		captured.append(" ");
	}

	for (int i = 0; i < 2 - positionsCount(board.pieceBoards[WRook + side]); ++i) {
		captured += getPieceCode(WRook + side);
		captured.append(" ");
	}

	for (int i = 0; i < 2 - positionsCount(board.pieceBoards[WBishop + side]); ++i) {
		captured += getPieceCode(WBishop + side);
		captured.append(" ");
	}

	for (int i = 0; i < 2 - positionsCount(board.pieceBoards[WKnight + side]); ++i) {
		captured += getPieceCode(WKnight + side);
		captured.append(" ");
	}

	for (int i = 0; i < 8 - positionsCount(board.pieceBoards[WPawn + side]); ++i) {
		captured += getPieceCode(WPawn + side);
		captured.append(" ");
	}

	return captured;
}

void Game::calculateCheckPositions(const Side side) {
	checkStatus.reset();
	Rawboard positions = PIECES(board, side);

	while(positions) {
		const Position position = getFirstPos(positions);
		const Piece piece = getPiece(board, position);

		if (_isPawn(piece)) {
			const Rawboard attacks = pawnAttacks(board, position, side);
			checkStatus.updateAllCheckPositions(attacks);
			checkStatus.addCheckPosition(position, attacks);
		} else if (_isRook(piece)) {
			const Rawboard attacks = _rookAttacks(position, ~board.pieceBoards[Empty], ~PIECES(board, side));
			checkStatus.updateAllCheckPositions(attacks);
			checkStatus.addCheckPosition(position, attacks);
			checkStatus.addXRayPosition(position, attacks);
		} else if (_isKnight(piece)) {
			const Rawboard attacks = _knightAttacks(board, position, OPP_PIECES(board, side));
			checkStatus.updateAllCheckPositions(attacks);
			checkStatus.addCheckPosition(position, attacks);
		} else if (_isBishop(piece)) {
			const Rawboard attacks = _bishopAttacks(position, ~board.pieceBoards[Empty], ~PIECES(board, side));
			checkStatus.updateAllCheckPositions(attacks);
			checkStatus.addCheckPosition(position, attacks);
			checkStatus.addXRayPosition(position, attacks);
		} else if (_isQueen(piece)) {
			const Rawboard attacks = _queenAttacks(position, ~board.pieceBoards[Empty], ~PIECES(board, side));
			checkStatus.updateAllCheckPositions(attacks);
			checkStatus.addCheckPosition(position, attacks);
			checkStatus.addXRayPosition(position, attacks);
		} else if (_isKing(piece)) {
			const Rawboard attacks = kingMoves(board, position, side);
			checkStatus.updateAllCheckPositions(attacks);
			checkStatus.addCheckPosition(position, attacks);
		}

		positions &= (positions - 1);
	}
}

int Game::getAllDestinationQty(const Side side) {
	int count = 0;
	Rawboard pieces = PIECES(board, side);

	while(pieces) {
		const Rawboard destinations = getDestinationPositions(board, getFirstPos(pieces));
		count += positionsCount(destinations);
		pieces &= (pieces - 1);
	}

	return count;
}

string Game::getDescription() const {
	return format("{} vs {}", whitePlayer->getDescription(), blackPlayer->getDescription());
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