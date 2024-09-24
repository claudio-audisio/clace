#pragma once

#include <deque>
#include <chrono>
#include <list>
#include <string>

#include "../common/types.h"

using namespace std;


class Utils {
public:
	template <class T> 
	static void dequeAddAll(const deque<T>& source, deque<T>& destination) {
		for (auto const& move : source) {
			destination.push_back(move);
		}
	}

	static unsLL getElapsedNanos(const chrono::time_point<chrono::steady_clock> start) {
		return chrono::duration_cast<chrono::nanoseconds>(chrono::steady_clock::now() - start).count();
	}

	static unsLL getElapsedMillis(const chrono::time_point<chrono::steady_clock> start) {
		return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();
	}

	static unsLL getElapsedSeconds(const chrono::time_point<chrono::steady_clock> start) {
		return chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count();
	}

};

