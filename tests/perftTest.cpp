#include <gtest/gtest.h>

#include "../perft/perft.h"
#include "../utils/positions.h"

#define DEPTH 3

class PerftTest : public testing::Test {
protected:
    void checkBulkPerftResult(const unsLL nodes, const unsigned int depth, const unsigned int currentDepth, const Result& result) {
        if (currentDepth == depth) {
            EXPECT_EQ(result.getNodes(currentDepth - 1), nodes);
        }
    }

    void checkPerftResult(const unsLL nodes, const unsLL captures, const unsLL enPassant, const unsLL castling, const unsLL promotions, const unsLL checks, const unsLL discoveryChecks, const unsLL doubleChecks, const unsLL checkmates, const unsigned int depth, const unsigned int currentDepth, const Result& result) {
        if (currentDepth <= depth) {
            if (nodes != -1) EXPECT_EQ(result.getNodes(currentDepth - 1), nodes);
            if (captures != -1) EXPECT_EQ(result.getCaptures(currentDepth - 1), captures);
            if (enPassant != -1) EXPECT_EQ(result.getEnPassant(currentDepth - 1), enPassant);
            if (castling != -1) EXPECT_EQ(result.getCastling(currentDepth - 1), castling);
            if (promotions != -1) EXPECT_EQ(result.getPromotions(currentDepth - 1), promotions);
            if (checks != -1) EXPECT_EQ(result.getChecks(currentDepth - 1), checks);
            if (discoveryChecks != -1) EXPECT_EQ(result.getDiscoveryChecks(currentDepth - 1), discoveryChecks);
            if (doubleChecks != -1) EXPECT_EQ(result.getDoubleChecks(currentDepth - 1), doubleChecks);
            if (checkmates != -1) EXPECT_EQ(result.getCheckmates(currentDepth - 1), checkmates);
        }
    }
};

TEST_F(PerftTest, initialPositionBulkPerft) {
    GTEST_SKIP();
    Perft* perft = new Perft(Positions::INITIAL_FEN_POSITION, DEPTH);
    Result* result = perft->runBulk();

    checkBulkPerftResult(20, DEPTH, 1, *result); 
    checkBulkPerftResult(400, DEPTH, 2, *result);
    checkBulkPerftResult(8902, DEPTH, 3, *result);
    checkBulkPerftResult(197281, DEPTH, 4, *result);
    checkBulkPerftResult(4865609, DEPTH, 5, *result);
    checkBulkPerftResult(119060324, DEPTH, 6, *result);
    checkBulkPerftResult(3195901860L, DEPTH, 7, *result);   // TODO
}

TEST_F(PerftTest, position2BulkPerft) {
    GTEST_SKIP();
    Perft* perft = new Perft(Positions::PERFT_FEN_POSITION_2, DEPTH);
    Result* result = perft->runBulk();

    checkBulkPerftResult(48, DEPTH, 1, *result);
    checkBulkPerftResult(2039, DEPTH, 2, *result);
    checkBulkPerftResult(97862, DEPTH, 3, *result);
    checkBulkPerftResult(4085603, DEPTH, 4, *result);
    checkBulkPerftResult(193690690, DEPTH, 5, *result);
}

TEST_F(PerftTest, position3BulkPerft) {
    GTEST_SKIP();
    Perft* perft = new Perft(Positions::PERFT_FEN_POSITION_3, DEPTH);
    Result* result = perft->runBulk();

    checkBulkPerftResult(14, DEPTH, 1, *result);
    checkBulkPerftResult(191, DEPTH, 2, *result);
    checkBulkPerftResult(2812, DEPTH, 3, *result);
    checkBulkPerftResult(43238, DEPTH, 4, *result);
    checkBulkPerftResult(674624, DEPTH, 5, *result);
    checkBulkPerftResult(11030083, DEPTH, 6, *result);      // TODO
    checkBulkPerftResult(178633661, DEPTH, 7, *result);     // TODO
}

TEST_F(PerftTest, position4BulkPerft) {
    GTEST_SKIP();
    Perft* perft = new Perft(Positions::PERFT_FEN_POSITION_4, DEPTH);
    Result* result = perft->runBulk();

    checkBulkPerftResult(6, DEPTH, 1, *result);
    checkBulkPerftResult(264, DEPTH, 2, *result);
    checkBulkPerftResult(9467, DEPTH, 3, *result);
    checkBulkPerftResult(422333, DEPTH, 4, *result);
    checkBulkPerftResult(15833292, DEPTH, 5, *result);
    checkBulkPerftResult(706045033, DEPTH, 6, *result);     // TODO
}

TEST_F(PerftTest, position5BulkPerft) {
    GTEST_SKIP();
    Perft* perft = new Perft(Positions::PERFT_FEN_POSITION_5, DEPTH);
    Result* result = perft->runBulk();

    checkBulkPerftResult(44, DEPTH, 1, *result);
    checkBulkPerftResult(1486, DEPTH, 2, *result);
    checkBulkPerftResult(62379, DEPTH, 3, *result);
    checkBulkPerftResult(2103487, DEPTH, 4, *result);
    checkBulkPerftResult(89941194, DEPTH, 5, *result);
}

TEST_F(PerftTest, position6BulkPerft) {
    GTEST_SKIP();
    Perft* perft = new Perft(Positions::PERFT_FEN_POSITION_6, DEPTH);
    Result* result = perft->runBulk();

    checkBulkPerftResult(46, DEPTH, 1, *result);
    checkBulkPerftResult(2079, DEPTH, 2, *result);
    checkBulkPerftResult(89890, DEPTH, 3, *result);
    checkBulkPerftResult(3894594, DEPTH, 4, *result);
    checkBulkPerftResult(164075551, DEPTH, 5, *result);
    checkBulkPerftResult(6923051137, DEPTH, 6, *result);        // TODO
    checkBulkPerftResult(287188994746, DEPTH, 7, *result);      // TODO
    checkBulkPerftResult(11923589843526, DEPTH, 8, *result);    // TODO
    checkBulkPerftResult(490154852788714, DEPTH, 9, *result);   // TODO
}

TEST_F(PerftTest, initialPositionPerft) {
    GTEST_SKIP();
    Perft* perft = new Perft(Positions::INITIAL_FEN_POSITION, DEPTH);
    Result* result = perft->run();

    checkPerftResult(20, 0, 0, 0, 0, 0, 0, 0, 0, DEPTH, 1, *result);
    checkPerftResult(400, 0, 0, 0, 0, 0, 0, 0, 0, DEPTH, 2, *result);
    checkPerftResult(8902, 34, 0, 0, 0, 12, 0, 0, 0, DEPTH, 3, *result);
    checkPerftResult(197281, 1576, 0, 0, 0, 469, 0, 0, 8, DEPTH, 4, *result);
    checkPerftResult(4865609, 82719, 258, 0, 0, 27351, 6, 0, 347, DEPTH, 5, *result);
    // TODO
    checkPerftResult(119060324, 2812008, 5248, 0, 0, 809099, 329, 46, 10828, DEPTH, 6, *result);
    // TODO
    checkPerftResult(3195901860L, 108329926, 319617, 883453, 0, 33103848, 18026, 1628, 435767, DEPTH, 7, *result);
}

TEST_F(PerftTest, Position2Perft) {
    //GTEST_SKIP();
    Perft* perft = new Perft(Positions::PERFT_FEN_POSITION_2, DEPTH);
    Result* result = perft->run();

    checkPerftResult(48, 8, 0, 2, 0, 0, 0, 0, 0, DEPTH, 1, *result);
    checkPerftResult(2039, 351, 1, 91, 0, 3, 0, 0, 0, DEPTH, 2, *result);
    checkPerftResult(97862, 17102, 45, 3162, 0, 993, 0, 0, 1, DEPTH, 3, *result);
    checkPerftResult(4085603, 757163, 1929, 128013, 15172, 25523, 42, 6, 43, DEPTH, 4, *result);
    // TODO errore discoveryChecks(19895) doubleChecks(2645)
    checkPerftResult(193690690, 35043416, 73365, 4993637, 8392, 3309887, 19883, 2637, 30171, DEPTH, 5, *result);
}

TEST_F(PerftTest, Position3Perft) {
    GTEST_SKIP();
    Perft* perft = new Perft(Positions::PERFT_FEN_POSITION_3, DEPTH);
    Result* result = perft->run();

    checkPerftResult(14, 1, 0, 0, 0, 2, 0, 0, 0, DEPTH, 1, *result);
    checkPerftResult(191, 14, 0, 0, 0, 10, 0, 0, 0, DEPTH, 2, *result);
    checkPerftResult(2812, 209, 2, 0, 0, 267, 3, 0, 0, DEPTH, 3, *result);
    checkPerftResult(43238, 3348, 123, 0, 0, 1680, 106, 0, 17, DEPTH, 4, *result);
    checkPerftResult(674624, 52051, 1165, 0, 0, 52950, 1292, 3, 0, DEPTH, 5, *result);
    // TODO
    checkPerftResult(11030083, 940350, 33325, 0, 7552, 452473, 26067, 0, 2733, DEPTH, 6, *result);
    // TODO
    checkPerftResult(178633661, 14519036, 294874, 0, 140024, 12797406, 370630, 3612, 87, DEPTH, 7, *result);
}

TEST_F(PerftTest, Position4Perft) {
    GTEST_SKIP();
    Perft* perft = new Perft(Positions::PERFT_FEN_POSITION_4, DEPTH);
    Result* result = perft->run();

    // TODO mancano i discovery e double chacks

    checkPerftResult(6, 0, 0, 0, 0, 0, -1, -1, 0, DEPTH, 1, *result);
    checkPerftResult(264, 87, 0, 6, 48, 10, -1, -1, 0, DEPTH, 2, *result);
    checkPerftResult(9467, 1021, 4, 0, 120, 38, -1, -1, 22, DEPTH, 3, *result);
    checkPerftResult(422333, 131393, 0, 7795, 60032, 15492, -1, -1, 5, DEPTH, 4, *result);
    checkPerftResult(15833292, 2046173, 6512, 0, 329464, 200568, -1, -1, 50562, DEPTH, 5, *result);
    // TODO
    checkPerftResult(706045033, 210369132, 212, 10882006, 81102984, 26973664, -1, -1, 81076, DEPTH, 6, *result);
}
