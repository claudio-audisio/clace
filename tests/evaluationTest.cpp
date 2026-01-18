#include <gtest/gtest.h>

#include "../evaluation/basicEvaluator.h"
#include "../utils/fen.h"
#include "testUtils.h"
#include "../move/movesCalculation.h"
#include "move/movesGenerator.h"

using namespace std;


class EvaluationTest : public testing::Test
{
protected:
	EvaluationTest() {
		initMovesGenerator();
	}
	~EvaluationTest() {

	}
};

TEST_F(EvaluationTest, noMovesEvaluationTest) {
	Game game;
	game.init();

	EXPECT_EQ(game.evaluator->evaluate(game), 0);
}