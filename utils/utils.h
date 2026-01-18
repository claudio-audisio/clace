#pragma once

#include <deque>
#include <chrono>
#include <list>
#include <string>

#include "../common/types.h"

using namespace std;

inline unsigned int fastAbs(int value) {
    int s = value >> 31; // cdq, signed shift, -1 if negative, else 0
    value ^= s;  // ones' complement if negative
    value -= s;  // plus one if negative -> two's complement if negative
    return value;
}

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

