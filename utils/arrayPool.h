#pragma once

#include <cstring>
#include <stack>

#include "../common/constants.h"

using namespace std;

template <typename T>
class ArrayPool {

    stack<T*> pool;
    unsigned int size;
    unsigned int arraySize;

public:

    explicit ArrayPool(const unsigned int poolSize, const unsigned int arraySize) {
        this->size = poolSize;
        this->arraySize = arraySize;

        for (unsigned int i = 0; i < poolSize; i++) {
            pool.push(new T[arraySize]);
        }
    }

    ~ArrayPool() noexcept(false) {
        if (occupancy() > 0) {
            throw runtime_error("memory leak detected on pool destructor");
        }

        while (pool.size() > 0) {
            delete[] pool.top();
            pool.pop();
        }
    }

    T* getArray() {
        if (!pool.empty()) {
            T *array = pool.top();
            pool.pop();
            return array;
        } else {
            size++;
            return new T[arraySize];
        }
    }

    void release(T *array) {
        memset(array, 0, sizeof(T) * arraySize);
        pool.push(array);
    }

    unsigned int capacitiy() {
        return pool.size();
    }

    unsigned int occupancy() {
        return size - pool.size();
    }

};