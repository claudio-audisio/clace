#pragma once

#include "../common/types.h"
#include "../move/move.h"

class CastlingHelper {
public:

	static void update(CastlingInfo& castlingInfo, const Move& move) {
		switch (MoveHelper::getSourcePosition(move)) {
			case 0: castlingInfo &= 0b1110; break;
			case 4: castlingInfo &= 0b1100; break;
			case 7: castlingInfo &= 0b1101; break;
			case 56: castlingInfo &= 0b1011; break;
			case 60: castlingInfo &= 0b0011; break;
			case 63: castlingInfo &= 0b0111; break;
			default: break;
		}
        switch (MoveHelper::getDestinationPosition(move)) {
            case 0: castlingInfo &= 0b1110; break;
            case 7: castlingInfo &= 0b1101; break;
            case 56: castlingInfo &= 0b1011; break;
            case 63: castlingInfo &= 0b0111; break;
            default: break;
        }
	}

	static CastlingInfo reset() {
		return 0b0000;
	}

	static void setBlackKing(CastlingInfo& castlingInfo, const bool info) {
		if (info) {
			castlingInfo |= 0b0010;
		}
		else {
			castlingInfo &= 0b1101;
		}
	}

	static void setBlackQueen(CastlingInfo& castlingInfo, const bool info) {
		if (info) {
			castlingInfo |= 0b0001;
		}
		else {
			castlingInfo &= 0b1110;
		}
	}

	static void setWhiteKing(CastlingInfo& castlingInfo, const bool info) {
		if (info) {
			castlingInfo |= 0b1000;
		}
		else {
			castlingInfo &= 0b0111;
		}
	}

	static void setWhiteQueen(CastlingInfo& castlingInfo, const bool info) {
		if (info) {
			castlingInfo |= 0b0100;
		}
		else {
			castlingInfo &= 0b1011;
		}
	}

	static bool isWhiteKingCastling(const CastlingInfo castlingInfo) {
		return (castlingInfo & 0b1000) > 0;
	}

	static bool isWhiteQueenCastling(const CastlingInfo castlingInfo) {
		return (castlingInfo & 0b0100) > 0;
	}

	static bool isBlackKingCastling(const CastlingInfo castlingInfo) {
		return (castlingInfo & 0b0010) > 0;
	}

	static bool isBlackQueenCastling(const CastlingInfo castlingInfo) {
		return (castlingInfo & 0b0001) > 0;
	}

};

