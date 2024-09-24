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
    //GTEST_SKIP();

    const unsigned int DEPTH = 5;
    Perft* perft = new Perft(Positions::INITIAL_FEN_POSITION, DEPTH);
    Result* result = perft->runBulk();

    checkBulkPerftResult(20, DEPTH, 1, *result); 
    checkBulkPerftResult(400, DEPTH, 2, *result);
    checkBulkPerftResult(8902, DEPTH, 3, *result);
    checkBulkPerftResult(197281, DEPTH, 4, *result);        // 52       (60)
    checkBulkPerftResult(4865609, DEPTH, 5, *result);       // 800      (800)
    checkBulkPerftResult(119060324, DEPTH, 6, *result);     // 31900    (36600)
    checkBulkPerftResult(3195901860L, DEPTH, 7, *result);
}


class PerftTest : public testing::Test {
protected:
    void checkPerftResult(const unsLL nodes, const unsLL captures, const unsLL enPassant, const unsLL castling, const unsLL promotions, const unsLL checks, const unsLL discoveryChecks, const unsLL doubleChecks, const unsLL checkmates, const unsigned int depth, const unsigned int currentDepth, const Result& result) {
        if (currentDepth <= depth) {
            EXPECT_EQ(result.getNodes(currentDepth - 1), nodes);
            EXPECT_EQ(result.getCaptures(currentDepth - 1), captures);
            EXPECT_EQ(result.getEnPassant(currentDepth - 1), enPassant);
            EXPECT_EQ(result.getCastling(currentDepth - 1), castling);
            EXPECT_EQ(result.getPromotions(currentDepth - 1), promotions);
            EXPECT_EQ(result.getChecks(currentDepth - 1), checks);
            EXPECT_EQ(result.getDiscoveryChecks(currentDepth - 1), discoveryChecks);
            EXPECT_EQ(result.getDoubleChecks(currentDepth - 1), doubleChecks);
            EXPECT_EQ(result.getCheckmates(currentDepth - 1), checkmates);
        }
    }
};

TEST_F(PerftTest, initialPositionPerft) {
    GTEST_SKIP();

    const unsigned int DEPTH = 5;
    Perft* perft = new Perft(Positions::INITIAL_FEN_POSITION, DEPTH);
    Result* result = perft->run();

    checkPerftResult(20, 0, 0, 0, 0, 0, 0, 0, 0, DEPTH, 1, *result);
    checkPerftResult(400, 0, 0, 0, 0, 0, 0, 0, 0, DEPTH, 2, *result);
    checkPerftResult(8902, 34, 0, 0, 0, 12, 0, 0, 0, DEPTH, 3, *result);
    checkPerftResult(197281, 1576, 0, 0, 0, 469, 0, 0, 8, DEPTH, 4, *result);
    checkPerftResult(4865609, 82719, 258, 0, 0, 27351, 6, 0, 347, DEPTH, 5, *result);
    checkPerftResult(119060324, 2812008, 5248, 0, 0, 809099, 329, 46, 10828, DEPTH, 6, *result);
    checkPerftResult(3195901860L, 108329926, 319617, 883453, 0, 33103848, 18026, 1628, 435767, DEPTH, 7, *result);
}