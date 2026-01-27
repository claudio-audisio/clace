#include "gtest/gtest.h"
#include "utils/arrayPool.h"


using namespace std;


class ArrayPoolTest : public testing::Test {
protected:
    ArrayPoolTest() {

    }
    ~ArrayPoolTest() {

    }
};

TEST_F(ArrayPoolTest, constructorTest) {
    ArrayPool<Move> *pool = new ArrayPool<Move>(10, 20, 1);

    EXPECT_EQ(pool->capacitiy(), 10);
    EXPECT_EQ(pool->occupancy(), 0);
    EXPECT_EQ(pool->_incrementSize(), 1);
}

TEST_F(ArrayPoolTest, constructorTest2) {
    ArrayPool<Move> *pool = new ArrayPool<Move>(10, 20);

    EXPECT_EQ(pool->capacitiy(), 10);
    EXPECT_EQ(pool->occupancy(), 0);
    EXPECT_EQ(pool->_incrementSize(), 2);
}

TEST_F(ArrayPoolTest, destructorTest) {
    ArrayPool<Move> *pool = new ArrayPool<Move>(10, 20, 1);

    auto array = pool->getArray();

    ASSERT_THROW(delete pool, runtime_error);
}

TEST_F(ArrayPoolTest, getArrayTest) {
    ArrayPool<Move> *pool = new ArrayPool<Move>(1, 20, 1);

    auto array = pool->getArray();

    EXPECT_EQ(pool->capacitiy(), 0);
    EXPECT_EQ(pool->occupancy(), 1);

    auto array2 = pool->getArray();

    EXPECT_EQ(pool->capacitiy(), 0);
    EXPECT_EQ(pool->occupancy(), 2);
}

TEST_F(ArrayPoolTest, releaseTest) {
    ArrayPool<Move> *pool = new ArrayPool<Move>(1, 20, 1);

    auto array = pool->getArray();

    EXPECT_EQ(pool->capacitiy(), 0);
    EXPECT_EQ(pool->occupancy(), 1);

    pool->release(array);

    EXPECT_EQ(pool->capacitiy(), 1);
    EXPECT_EQ(pool->occupancy(), 0);

    auto array2 = pool->getArray();

    EXPECT_EQ(array, array2);   // it is the same array (same piece of memory)

    auto array3 = pool->getArray();

    EXPECT_EQ(pool->capacitiy(), 0);
    EXPECT_EQ(pool->occupancy(), 2);

    pool->release(array);
    pool->release(array3);

    EXPECT_EQ(pool->capacitiy(), 2);
    EXPECT_EQ(pool->occupancy(), 0);
}