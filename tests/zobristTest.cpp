#include "gtest/gtest.h"

#include "../cache/zobrist.h"
#include "../move/movesCalculation.h"
#include "cache/transpositionTable.h"
#include "move/movesGenerator.h"
#include "utils/fen.h"
#include "utils/movePool.h"

using namespace std;

class ZobristTest : public testing::Test {
protected:
    MovePool* pool;
    int depth = 4;
    TranspositionTable table;
    Game *game;
    unordered_map<unsLL, string> keys;

    ZobristTest() {
        initMovesGenerator();
        pool = new MovePool(depth + 1);
        game = new Game();
    }

    ~ZobristTest() {
        delete pool;
        delete game;
    }

    unsLL runBulkPerft(const unsigned int currentDepth) {
        Move* moves = pool->getArray(currentDepth - 1);
        MovesAmount amount;
        generateLegalMoves(*game, moves, &amount);

        const unsLL key = game->key;
        const string fen = FEN::gameToFENWithoutMoves(*game);

        if (keys.contains(key)) {
            const string& prevFen = keys.at(key);
            EXPECT_EQ(prevFen, fen);
        } else {
            keys.insert(make_pair(key, fen));
            //cout << key << " - " << fen << endl;
        }

        if (currentDepth == 1) {
            return amount.legal;
        }

        unsLL nodes = 0;

        for (unsigned int i = 0; i < amount.total; i++) {
            if (moves[i]) {
                game->save();
                game->applyMove(moves[i]);
                const unsLL newNodes = runBulkPerft(currentDepth - 1);
                nodes += newNodes;
                game->rollbackLastMove();
            }
        }

        return nodes;
    }
};

TEST_F(ZobristTest, getZobristKeyTest) {
    Game game;
    game.init();

    const unsLL index1 = calculateKey(game);
    const unsLL index2 = calculateKey(game);

    ASSERT_EQ(index1, index2);
}

TEST_F(ZobristTest, zobristKeyCollisionTest) {
#ifndef USE_CACHE
    GTEST_SKIP();
#endif
    game->initFromFEN(PERFT_FEN_POSITION_2);
    runBulkPerft(depth);
}

class TestParams {
public:
    TestParams(const string& fenGame, const Position sourcePosition, const Position destinationPosition, const Piece piece) {
        this->fenGame = fenGame;
        this->sourcePosition = sourcePosition;
        this->destinationPosition = destinationPosition;
        this->piece = piece;
    }

    string fenGame;
    Position sourcePosition;
    Position destinationPosition;
    Piece piece;
};

class UpdateZobristKeyTest : public ::testing::TestWithParam<TestParams*> {};

TEST_P(UpdateZobristKeyTest, updateZobristKeyTest) {
    const TestParams* params = GetParam();
    Game* game = FEN::fenToNewGame(params->fenGame);
    Move move = createMove(params->sourcePosition, params->destinationPosition, game->getSide(params->sourcePosition), params->piece, game->board->enPassantPosition);

    if (isPawnPromotion(move)) {
        setPromotion(move, WQueen + _getSide(params->piece));
    }

    game->applyMove(move);

    ASSERT_EQ(game->key, calculateKey(*game));
}

INSTANTIATE_TEST_SUITE_P(
    ZobristTest,
    UpdateZobristKeyTest,
    ::testing::Values(
        new TestParams(PERFT_FEN_POSITION_2, 14, 22, WPawn),
        new TestParams(PERFT_FEN_POSITION_2_BLACK_TO_MOVE, 46, 38, BPawn),
        new TestParams(PERFT_FEN_POSITION_2, 14, 23, WPawn),
        new TestParams(PERFT_FEN_POSITION_2_BLACK_TO_MOVE, 25, 18, BPawn),
        new TestParams(PERFT_FEN_POSITION_2, 4, 6, WKing),
        new TestParams(PERFT_FEN_POSITION_2, 4, 2, WKing),
        new TestParams(PERFT_FEN_POSITION_2_BLACK_TO_MOVE, 60, 62, BKing),
        new TestParams(PERFT_FEN_POSITION_2_BLACK_TO_MOVE, 60, 58, BKing),
        new TestParams("4k3/8/8/Pp6/8/8/8/4K3 w - b6 0 1", 32, 41, WPawn),
        new TestParams(FEN_EN_PASSANT_BUG_TEST, 24, 17, BPawn),
        new TestParams("2q4k/3P4/8/8/8/8/8/7K w - - 0 1", 51, 59, WPawn),
        new TestParams("7k/8/8/8/8/8/3p4/2Q4K b - - 0 1", 11, 3, BPawn),
        new TestParams("2q4k/3P4/8/8/8/8/8/7K w - - 0 1", 51, 58, WPawn),
        new TestParams("7k/8/8/8/8/8/3p4/2Q4K b - - 0 1", 11, 2, BPawn)
    )
);

TEST_F(ZobristTest, zobristKeyBugTest) {
    GTEST_SKIP();
    Game game1;
    game1.initFromFEN("r3k2r/p1ppqpb1/bn2p1p1/3nN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b Kkq - 0 0");
    Game game2;
    game2.initFromFEN("r3k2r/p1ppqpb1/bn2p1p1/3nN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 0");

    ASSERT_NE(game1.key, game2.key);
}
