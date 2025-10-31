#include <gtest/gtest.h>

#include "../evaluation/basicEvaluator.h"
#include "../utils/fen.h"
#include "testUtils.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;


class EvaluationTest : public testing::Test
{
protected:
	EvaluationTest() {
		initAttacks();
		initDestPosProviders();
		initPawnAttacksProviders();
	}
	~EvaluationTest() {

	}
};

TEST_F(EvaluationTest, noMovesEvaluationTest) {
	Game game;
	game.init();

	EXPECT_EQ(game.evaluator->evaluate(game), 0);
}