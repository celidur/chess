#include "gtest/gtest.h"
#include "common/struct.h"
#include "logic/Game.h"


class BoardTestFixture: public ::testing::Test {
protected:
    void SetUp() override{
        setDefaultBoard();
    }

    void resetBoard(){
        logic::Game::resetBoard(board);
    }

    void setDefaultBoard(){
        logic::Game::setDefaultBoard(board);
    }

    void testBishop(logic::Bishop& bishop, const Coord &pos) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (pos.x != i && pos.y != j) {
                    EXPECT_FALSE(bishop.isLegalMove(this->board, {i, j})) << "Illegal move detected at position " << i << ", " << j << "for bishop " << colorToStr(bishop.getType().color) << std::endl;
                }
            }
        }

        this->resetBoard();
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (pos.x != i && pos.y != j) {
                    if((pos.y == j) || (abs(pos.y - j) != abs(pos.x - i)))
                        EXPECT_FALSE(bishop.isLegalMove(this->board, {i, j})) << "Illegal move detected at position " << i << ", " << j << "for bishop " << colorToStr(bishop.getType().color) << std::endl;
                    else
                        EXPECT_TRUE(bishop.isLegalMove(this->board, {i, j})) << "Legal move not detected at position " << i << ", " << j << "for bishop " << colorToStr(bishop.getType().color) << std::endl;
                }
            }
        }
    }

    static std::string colorToStr(Color color){
        return color == Color::white ? "white" : "black";
    }


  TypePiece board[8][8];
};

TEST (BaseTest, base) {
    EXPECT_EQ (1 + 1, (2));
}

//TEST(INIT_GAME, initNormal) {
//    logic::Game game;
//    EXPECT_EQ(game.getMode(), (Mode::personalisation));
//
//    game.loadGame();
//    EXPECT_EQ(game.getMode(), (Mode::game));
//}


TEST_F(BoardTestFixture, Bishop){
    Coord pos = {2, 7};
    logic::Bishop bishopBlack({2, 7}, Color::black);
    testBishop(bishopBlack, pos);

    setDefaultBoard();

    pos = {2, 0};
    logic::Bishop bishopWhite({2, 0}, Color::white);
    testBishop(bishopWhite, pos);

}
