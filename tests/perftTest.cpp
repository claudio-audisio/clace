#include <gtest/gtest.h>

#include "../perft/perft.h"
#include "../utils/positions.h"


class BulkPerftTest : public testing::Test {
protected:
    void checkBulkPerftResult(const unsLL nodes, const unsigned int depth, const unsigned int currentDepth, const Result& result) {
        if (currentDepth == depth) {
            EXPECT_EQ(result.getNodes(currentDepth - 1), nodes);
        }
    }
};



TEST_F(BulkPerftTest, initialPositionBulkPerft) {
    GTEST_SKIP();

    const unsigned int DEPTH = 5;
    Perft* perft = new Perft(Positions::INITIAL_FEN_POSITION, DEPTH);
    Result* result = perft->runBulk();

    checkBulkPerftResult(20, DEPTH, 1, *result); 
    checkBulkPerftResult(400, DEPTH, 2, *result);
    checkBulkPerftResult(8902, DEPTH, 3, *result);
    checkBulkPerftResult(197281, DEPTH, 4, *result);        // 120
    checkBulkPerftResult(4865609, DEPTH, 5, *result);       // 2250
    checkBulkPerftResult(119060324, DEPTH, 6, *result);     // 70209
    checkBulkPerftResult(3195901860L, DEPTH, 7, *result);

    EXPECT_NE(1, 1)
        << result->getMoves() << " in " << result->getElapsed() << " msec" << endl;
}