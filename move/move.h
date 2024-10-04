#pragma once

#include <string>

#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;


class MoveHelper {
public:
	// Position sourcePosition;			0 - 7
	// Position destinationPosition;	8 - 15
	// Piece piece;						16 - 23
	// Piece promotion = Empty;			24 - 31
    // Piece captured;                  32 - 39
	// Side side;						40 - 47
	// bool castling = false;			48
	// bool enPassant = false;			49
	// bool pawnPromotion = false;		50
	// bool computer;					51

	static Move getMove(const string& move, const Side side) {
		// TODO tirare un'eccezione
	/*
	if (move.length() != 5) {
		throw new RuntimeException("move malformed");
	}
	*/
		return getMove(Positions::coordsToIndex(move.substr(0, 2)), Positions::coordsToIndex(move.substr(3, 5)), side);
	}

	static Move getMove(const Position sourcePosition, const Position destinationPosition, const Side side) {
		Move move = 0;
		setSourcePosition(move, sourcePosition);
		setDestinationPosition(move, destinationPosition);
		setSide(move, side);
		return move;
	}

	static void decorate(Move& move, const Piece piece, const Position enPassantPosition, const bool isComputerToMove) {
		setPiece(move, piece);
		const bool white = PieceHelper::isWhite(piece);
		const Position destinationPosition = getDestinationPosition(move);

		if (PieceHelper::isKing(piece)) {
			// castling
			const Position sourcePosition = getSourcePosition(move);
			setCastling(move, (!white && sourcePosition == 4 && (destinationPosition == 2 || destinationPosition == 6)) ||
				(white && sourcePosition == 60 && (destinationPosition == 58 || destinationPosition == 62)));
		} else if (PieceHelper::isPawn(piece)) {
			// En passant
			setEnPassant(move, enPassantPosition == destinationPosition);
			// Pawn promotion
			setPawnPromotion(move, Positions::isEighthRow(destinationPosition, white));
		}

		setComputer(move, isComputerToMove);
	}

	static string toString(const Move& move) {
		return Positions::indexToCoords(getSourcePosition(move)) + "-" + Positions::indexToCoords(getDestinationPosition(move));
	}

	static void setSourcePosition(Move& move, const Position sourcePosition) {
		move &= ~SOURCE_POS_MASK;
		move |= sourcePosition;
	}

	static Position getSourcePosition(const Move move) {
		return move & SOURCE_POS_MASK;
	}

	static void setDestinationPosition(Move& move, const Position destinationPosition) {
		move &= ~DEST_POS_MASK;
		move |= (Move)destinationPosition << 8;
}

	static Position getDestinationPosition(const Move move) {
		return static_cast<Position>((move & DEST_POS_MASK) >> 8);
	}

	static void setPiece(Move& move, const Piece piece) {
		move &= ~PIECE_MASK;
		move |= (Move)piece << 16;
	}

	static Piece getPiece(const Move move) {
		return static_cast<Piece>((move & PIECE_MASK) >> 16);
	}

	static void setPromotion(Move& move, const Piece promotion) {
		move &= ~PROMOTION_MASK;
		move |= (Move)promotion << 24;
	}

	static Piece getPromotion(const Move move) {
		return static_cast<Piece>((move & PROMOTION_MASK) >> 24);
	}

    static void setCaptured(Move& move, const Piece captured) {
        move &= ~CAPTURED_MASK;
        move |= (Move)captured << 32;
    }

    static Piece getCaptured(const Move move) {
        return static_cast<Piece>((move & CAPTURED_MASK) >> 32);
    }

    static bool isCaptured(const Move move) {
        return getCaptured(move) != Empty;
    }

	static void setSide(Move& move, const Side side) {
		move &= ~SIDE_MASK;
		move |= (Move)side << 40;
	}

	static Side getSide(const Move move) {
		return static_cast<Side>((move & SIDE_MASK) >> 40);
	}

	static bool isWhite(const Move move) {
		return getSide(move) == WHITE;
	}

	static void setCastling(Move& move, const bool castling) {
		if (castling) {
			move |= CASTLING_MASK;
		}
		else {
			move &= ~CASTLING_MASK;
		}
	}

	static bool isCastling(const Move move) {
		return (move & CASTLING_MASK) >> 41;
	}

	static void setEnPassant(Move& move, const bool enPassant) {
		if (enPassant) {
			move |= EN_PASSANT_MASK;
		}
		else {
			move &= ~EN_PASSANT_MASK;
		}
	}

	static bool isEnPassant(const Move move) {
		return (move & EN_PASSANT_MASK) >> 42;
	}

	static void setPawnPromotion(Move& move, const bool pawnPromotion) {
		if (pawnPromotion) {
			move |= PAWN_PROM_MASK;
		}
		else {
			move &= ~PAWN_PROM_MASK;
		}
	}

	static bool isPawnPromotion(const Move move) {
		return (move & PAWN_PROM_MASK) >> 43;
	}

	static void setComputer(Move& move, const bool computer) {
		if (computer) {
			move |= COMPUTER_MASK;
		}
		else {
			move &= ~COMPUTER_MASK;
		}
	}

	static bool isComputer(const Move move) {
		return (move & COMPUTER_MASK) >> 44;
	}

    static MoveResult getMoveResult(const bool captured, const bool promoted, const bool enPassant, const bool castling) {
        MoveResult moveResult = 0;
        setCapturedMR(moveResult, captured);
        setPromotedMR(moveResult, promoted);
        setEnPassantMR(moveResult, enPassant);
        setCastlingMR(moveResult, castling);
        return moveResult;
    }

    static void setCapturedMR(MoveResult& moveResult, const bool captured) {
        if (captured) {
            moveResult |= MR_CAPTURED_MASK;
        }
        else {
            moveResult &= ~MR_CAPTURED_MASK;
        }
    }

    static bool isCapturedMR(const MoveResult moveResult) {
        return (moveResult & MR_CAPTURED_MASK);
    }

    static void setPromotedMR(MoveResult& moveResult, const bool promoted) {
        if (promoted) {
            moveResult |= MR_PROMOTED_MASK;
        }
        else {
            moveResult &= ~MR_PROMOTED_MASK;
        }
    }

    static bool isPromotedMR(const MoveResult moveResult) {
        return (moveResult & MR_PROMOTED_MASK);
    }

    static void setEnPassantMR(MoveResult& moveResult, const bool enPassant) {
        if (enPassant) {
            moveResult |= MR_PASSANT_MASK;
        }
        else {
            moveResult &= ~MR_PASSANT_MASK;
        }
    }

    static bool isEnPassantMR(const MoveResult moveResult) {
        return (moveResult & MR_PASSANT_MASK);
    }

    static void setCastlingMR(MoveResult& moveResult, const bool castling) {
        if (castling) {
            moveResult |= MR_CASTLING_MASK;
        }
        else {
            moveResult &= ~MR_CASTLING_MASK;
        }
    }

    static bool isCastlingMR(const MoveResult moveResult) {
        return (moveResult & MR_CASTLING_MASK);
    }

private:
	static const Move SOURCE_POS_MASK = 0x00000000000000ffLL;
	static const Move DEST_POS_MASK = 0x000000000000ff00LL;
	static const Move PIECE_MASK = 0x0000000000ff0000LL;
	static const Move PROMOTION_MASK = 0x00000000ff000000LL;
    static const Move CAPTURED_MASK = 0x000000ff00000000LL;
	static const Move SIDE_MASK = 0x0000ff0000000000LL;
	static const Move CASTLING_MASK = 0x0001000000000000LL;
	static const Move EN_PASSANT_MASK = 0x0002000000000000LL;
	static const Move PAWN_PROM_MASK = 0x0004000000000000LL;
	static const Move COMPUTER_MASK = 0x0008000000000000LL;

    static const MoveResult MR_CAPTURED_MASK = 0x01;
    static const MoveResult MR_PROMOTED_MASK = 0x02;
    static const MoveResult MR_PASSANT_MASK = 0x04;
    static const MoveResult MR_CASTLING_MASK = 0x08;

};