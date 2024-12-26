#include <gtest/gtest.h>

#include "../evaluation/basicEvaluator.h"
#include "../utils/fen.h"
#include "testUtils.h"

using namespace std;


TEST(evaluationTest, test) {
	Game game;
	game.init();

	EXPECT_EQ(game.evaluator->evaluate(game), 0);
}