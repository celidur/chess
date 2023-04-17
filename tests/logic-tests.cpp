#include "gtest/gtest.h"
#include "common/struct.h"
#include "logic/Game.h"


TEST (BaseTest, base) {
    EXPECT_EQ (1 + 1, (2));
}

TEST(INIT_GAME, initNormal){
    logic::Game game;
    EXPECT_EQ(game.getMode(), (Mode::personalisation));

    game.loadGame();
    EXPECT_EQ(game.getMode(), (Mode::game));
}

//TEST (SquareRootTest, ZeroAndNegativeNos) {
//    ASSERT_EQ (0.0, square‑root (0.0));
//    ASSERT_EQ (‑1, square‑root (‑22.0));
//}