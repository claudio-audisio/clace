#pragma once

#include <deque>
#include <chrono>
#include <list>
#include <string>

#include "../move/move.h"

using namespace std;


class Utils {
public:
	template <class T> 
	static void dequeAddAll(const deque<T>& source, deque<T>& destination) {
		for (auto const& move : source) {
			destination.push_back(move);
		}
	}

	static long long getElapsedNanos(const chrono::time_point<chrono::steady_clock> start) {
		return chrono::duration_cast<chrono::nanoseconds>(chrono::steady_clock::now() - start).count();
	}

	static long long getElapsedMillis(const chrono::time_point<chrono::steady_clock> start) {
		return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();
	}

	static long long getElapsedSeconds(const chrono::time_point<chrono::steady_clock> start) {
		return chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count();
	}

};

