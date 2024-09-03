#pragma once

#include "../common/types.h"

class CastlingInfoHelper {
	public:

		static CastlingInfo reset() {
			return 0b0000;
		}

		static CastlingInfo setBlackKing(CastlingInfo castlingInfo, bool info) {
			if (info) {
				castlingInfo |= 0b0010;
			}
			else {
				castlingInfo &= 0b1101;
			}

			return castlingInfo;
		}

		static CastlingInfo setBlackQueen(CastlingInfo castlingInfo, bool info) {
			if (info) {
				castlingInfo |= 0b0001;
			}
			else {
				castlingInfo &= 0b1110;
			}

			return castlingInfo;
		}

		static CastlingInfo setWhiteKing(CastlingInfo castlingInfo, bool info) {
			if (info) {
				castlingInfo |= 0b1000;
			}
			else {
				castlingInfo &= 0b0111;
			}

			return castlingInfo;
		}

		static CastlingInfo setWhiteQueen(CastlingInfo castlingInfo, bool info) {
			if (info) {
				castlingInfo |= 0b0100;
			}
			else {
				castlingInfo &= 0b1011;
			}

			return castlingInfo;
		}

		static bool isWhiteKingCastling(CastlingInfo castlingInfo) {
			return (castlingInfo & 0b1000) > 0;
		}

		static bool isWhiteQueenCastling(CastlingInfo castlingInfo) {
			return (castlingInfo & 0b0100) > 0;
		}

		static bool isBlackKingCastling(CastlingInfo castlingInfo) {
			return (castlingInfo & 0b0010) > 0;
		}

		static bool isBlackQueenCastling(CastlingInfo castlingInfo) {
			return (castlingInfo & 0b0001) > 0;
		}

};

