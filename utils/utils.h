#pragma once

#include <deque>
#include <chrono>
#include <list>
#include <string>

#include "../common/types.h"

using namespace std;


template <class T>
void dequeAddAll(const deque<T>& source, deque<T>& destination) {
	for (auto const& move : source) {
		destination.push_back(move);
	}
}

inline unsLL getElapsedNanos(const chrono::time_point<chrono::steady_clock> start) {
	return chrono::duration_cast<chrono::nanoseconds>(chrono::steady_clock::now() - start).count();
}

inline unsLL getElapsedMicros(const chrono::time_point<chrono::steady_clock> start) {
	return chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();
}

inline unsLL getElapsedMillis(const chrono::time_point<chrono::steady_clock> start) {
	return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();
}

inline unsLL getElapsedSeconds(const chrono::time_point<chrono::steady_clock> start) {
	return chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count();
}

inline Position getFirstPos(const Rawboard board) {
    #ifdef __GNUC__
        return __builtin_ctzll(board);
    #elif defined(_MSC_VER)
        unsigned long position;
        _BitScanForward64(&position, board);
        return static_cast<Position>(position);
    #endif
}

inline Position getFirstPosReverse(const Rawboard board) {
    #ifdef __GNUC__
        return 63 - __builtin_clzll(board);
    #elif defined(_MSC_VER)
        unsigned long position;
        _BitScanReverse64(&position, board);
        return static_cast<Position>(position);
    #endif
}
