#include <cassert>

#include "game.h"

#include "../cache/zobrist.h"
#include "../utils/fen.h"
#include "../move/move.h"
#include "../move/movesGenerator.h"
#include "../game/player.h"
#include "../evaluation/basicEvaluator.h"
#include "../utils/toString.h"
#include "../utils/boardUtils.h"
#include "../move/rollback.h"

Game::Game() :
	sideToMove(0), fullMoves(0), halfMoveClock(0) {
	movesHistory = static_cast<Move*>(calloc(HISTORY_MOVES, sizeof(Move)));
	snapshots = allocateSnapshots(10);
	board = static_cast<Board*>(malloc(sizeof(Board)));
	reset(board);
	evaluator = new BasicEvaluator();
	initRandoms(randoms);
}

Game::~Game() {
	free(movesHistory);
	deallocateSnapshots(snapshots, 10);
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
	movesHistory[movesHistIndex++] = move;

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

void Game::simulateMove(Move& move) const {
	Piece captured = simulateMovePiece(
		board,
		getSourcePosition(move),
		getDestinationPosition(move),
		getPiece(move),
		isCastling(move));

	if (isPawnPromotion(move)) {
		completePawnPromotion(move);
	}

	if (!captured && isEnPassant(move)) {
		captured = completeEnPassant(move);
	}

	setCaptured(move, captured);
}

Piece Game::completeEnPassant(const Move move) const {
	const Position destination = getDestinationPosition(move) + (-8 + 8 * 2 * getMoveSide(move));
	return setEmpty(board, destination);
}

void Game::completePawnPromotion(const Move move) const {
	const Piece promotionPiece = getPromotion(move);

	if (!promotionPiece) {
		throw runtime_error("promotion piece not set");
	}

	//System.out.print("Pawn promotion --> " + getPieceTypeForPawnPromotion().name());
	setPiece(board, getDestinationPosition(move), promotionPiece);
}

void Game::undoSimulateMove(const Move move) const {
	if (isCastling(move)) {
		undoCastlingMove(board, getSourcePosition(move), getDestinationPosition(move));
	} else {
		setPiece(board, getSourcePosition(move), getPiece(move));
		setEmpty(board, getDestinationPosition(move));
		const Piece captured = getCaptured(move);

		if (captured) {
			if (isEnPassant(move)) {
				undoEnPassant(move);
			} else {
				setPiece(board, getDestinationPosition(move), captured);
			}
		}
	}

    // Undo Pawn promotion is not needed because undoing original move we have already moved a Pawn
}

void Game::undoEnPassant(const Move move) const {
	const Side side = getMoveSide(move);
    const Position destination = getDestinationPosition(move) + (-8 + 8 * 2 * side);
    setPiece(board, destination, BPawn - side);
}

void Game::verifyChecks() {
	const Position kingPosition = getKingPosition(board, sideToMove);
	calculateCheckPositions(getOppositeSide(), kingPosition);
	checkStatus.updateStatus(kingPosition, lastMove);
}

EndGameType Game::checkEndGame(const unsigned int legalMoves) {
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

bool Game::checkControl(const Move move) const {
	const Side side = getMoveSide(move);
	const Position kingPosition = getKingPosition(board, side);
	Rawboard checkBoard = allAttacks(board, OPPOSITE(side), kingPosition);

	if (isUnderCheck(checkBoard, kingPosition)) {
		return false;
	}

	if (isCastling(move)) {
		checkBoard |= allRayAttacks(board, OPPOSITE(side));

		switch (getDestinationPosition(move)) {
			case 2: return (checkBoard & 24L) == 0;
			case 6: return (checkBoard & 48L) == 0;
			case 58: return (checkBoard & 1729382256910270464L) == 0;
			case 62: return (checkBoard & 3458764513820540928L) == 0;
			default: break;
		}
	}

	return true;
}

void Game::changeTurn() {
	sideToMove = OPPOSITE(sideToMove);
}

Side Game::getSide(const Position position) const {
	if (isEmpty(board, position)) {
		assert(false);
	}

	if (isWhite(board, position)) {
		return _WHITE;
	}

	return _BLACK;
}

void Game::save() {
	saveSnapshot(board, sideToMove, fullMoves, halfMoveClock, snapshots, snapshotIndex++);
}

void Game::rollbackLastMove() {
	checkStatus.reset();
	movesHistIndex--;
	lastMove = movesHistIndex == 0 ? 0 : movesHistory[movesHistIndex - 1];
	loadSnapshot(this->board, sideToMove, fullMoves, halfMoveClock, snapshots, --snapshotIndex);
}

Player* Game::getCurrentPlayer() const {
	return isWhiteToMove() ? whitePlayer : blackPlayer;
}

Player* Game::getOtherPlayer() const {
	return isWhiteToMove() ? blackPlayer : whitePlayer;
}

bool Game::isComputerToMove() const {
	const Player* player = this->getCurrentPlayer();

	if (!player) {
		return false;
	}
	
	return player->computer;
}

void Game::setLastMove(const Move move) {
	movesHistory[movesHistIndex++] = move;
	lastMove = move;
}

Game* Game::duplicate() const {
	Game* newGame = new Game();
	newGame->init();
	copy(board, newGame->board);
	newGame->board->castlingInfo = board->castlingInfo;
	newGame->sideToMove = sideToMove;
	newGame->board->enPassantPosition = board->enPassantPosition;
	newGame->fullMoves = fullMoves;
	newGame->halfMoveClock = halfMoveClock;
	/*for (unsigned int i = 0; i < HISTORY_MOVES; i++) {
		newGame->movesHistory[i] = movesHistory[i];
	}*/
	memcpy(newGame->movesHistory, movesHistory, sizeof(movesHistory));
	newGame->movesHistIndex = movesHistIndex;
	newGame->checkStatus.set(checkStatus);
	return newGame;
}

string Game::printMovesHistory(const int depth) const {
    string moves;
	int size = depth == 0 ? movesHistIndex : depth;

	for (int i = 0; i < size; i++) {
		moves = moveToString(movesHistory[i]) + ", " + moves;
	}

    return moves.substr(0, moves.length() - 2);
}

string Game::printCastlingInfo() const {
    return FEN::castlingInfoToFEN(board->castlingInfo);
}

string Game::getCapturedList(const Side side) const {
	string captured;
	for (int i = 0; i < 1 - positionsCount(board->pieceBoards[WQueen + side]); ++i) {
		captured += getPieceCode(WQueen + side);
		captured.append(" ");
	}

	for (int i = 0; i < 2 - positionsCount(board->pieceBoards[WRook + side]); ++i) {
		captured += getPieceCode(WRook + side);
		captured.append(" ");
	}

	for (int i = 0; i < 2 - positionsCount(board->pieceBoards[WBishop + side]); ++i) {
		captured += getPieceCode(WBishop + side);
		captured.append(" ");
	}

	for (int i = 0; i < 2 - positionsCount(board->pieceBoards[WKnight + side]); ++i) {
		captured += getPieceCode(WKnight + side);
		captured.append(" ");
	}

	for (int i = 0; i < 8 - positionsCount(board->pieceBoards[WPawn + side]); ++i) {
		captured += getPieceCode(WPawn + side);
		captured.append(" ");
	}

	return captured;
}

void Game::calculateCheckPositions(const Side side, const Position kingPosition) {
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
			if (areOnSameRowOrColumn(position, kingPosition)) {
				const Rawboard attacks = _rookAttacks(position, ~board->pieceBoards[Empty], ~PIECES(board, side));
				checkStatus.updateAllCheckPositions(attacks);
				checkStatus.addCheckPosition(position, attacks);
				checkStatus.addXRayPosition(position, attacks);
			}
		} else if (_isKnight(piece)) {
			const Rawboard attacks = _knightAttacks(board, position, OPP_PIECES(board, side));
			checkStatus.updateAllCheckPositions(attacks);
			checkStatus.addCheckPosition(position, attacks);
		} else if (_isBishop(piece)) {
			if (areOnSameDiagonal(position, kingPosition)) {
				const Rawboard attacks = _bishopAttacks(position, ~board->pieceBoards[Empty], ~PIECES(board, side));
				checkStatus.updateAllCheckPositions(attacks);
				checkStatus.addCheckPosition(position, attacks);
				checkStatus.addXRayPosition(position, attacks);
			}
		} else if (_isQueen(piece)) {
			if (areOnSameRowOrColumnOrDiagonal(position, kingPosition)) {
				const Rawboard attacks = _queenAttacks(position, ~board->pieceBoards[Empty], ~PIECES(board, side));
				checkStatus.updateAllCheckPositions(attacks);
				checkStatus.addCheckPosition(position, attacks);
				checkStatus.addXRayPosition(position, attacks);
			}
		} else if (_isKing(piece)) {
			const Rawboard attacks = kingMoves(board, position, side);
			checkStatus.updateAllCheckPositions(attacks);
			checkStatus.addCheckPosition(position, attacks);
		}

		positions &= (positions - 1);
	}
}

int Game::getAllDestinationQty(const Side side) const {
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