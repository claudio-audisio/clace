#pragma once

#include <deque>

using namespace std;

template <class T> void dequeAddAll(const deque<T>& source, deque<T>& destination) {
	for (auto const& move : source) {
	//for (deque<T>::iterator it = source.begin(); it != source.end(); it++) {
		destination.push_back(move);
	}
}