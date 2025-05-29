#pragma once

#include <unordered_map>

#include "../common/types.h"

using namespace std;


class CheckStatus {
public:
	CheckStatus();
	~CheckStatus();

    void updateAllCheckPositions(Rawboard newPositions);
    void addCheckPosition(Position position, Rawboard newPositions);
    void addXRayPosition(Position position, Rawboard newPositions);
    void updateStatus(Position kingPosition, Move move);
    void reset();
    void set(const CheckStatus& checkStatus);

    Rawboard allCheckPositions;			// all positions under check
	Rawboard checkPositions[64];		// positions under check by piece position
	Rawboard xRayPositions[64];			// xray positions under check by piece position

	/*unordered_map<Position, Rawboard> checkPositions;    // positions under check by piece position
	unordered_map<Position, Rawboard> xRayPositions;     // xray positions under check by piece position*/

    bool check;
    bool discoveryCheck;
    bool doubleCheck;
    bool checkmate;

    bool isDiscoveryCheck(Position kingPosition, const Move& lastMove) const;
    void adjustChecks();
};
