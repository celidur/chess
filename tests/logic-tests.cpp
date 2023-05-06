#include "gtest/gtest.h"
#include "common/struct.h"
#include "logic/Game.h"
#include "controller/QtGame.h"


class BoardTestFixture: public ::testing::Test {
protected:
    void SetUp() override{
        setDefaultBoard();
    }

    void resetBoard(){
        game.resetBoard();
    }

    void setDefaultBoard(){
        game.setDefaultBoard();
    }

    static std::string errorMsg(logic::Piece& piece, int x, int y){
        std::stringstream s;
        s << "move detected at position " << x << ", " << y << " for piece " << colorToStr(piece.getType().color);
        return s.str();
    }

    void expectIllegalMove(logic::Piece& piece, int x, int y){
        EXPECT_FALSE(piece.isLegalMove(game.boar(), {x, y})) << "Illegal " << errorMsg(piece, x, y) << std::endl;
    }

    void expectLegalMove(logic::Piece& piece, int x, int y){
        EXPECT_TRUE(piece.isLegalMove(game.getBoard(), {x, y})) << "Legal " << errorMsg(piece, x, y) << std::endl;
    }

    static void checkAllCasesExceptPos(Coord pos, const std::function<void (int, int)>& func){
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (!(pos.x == i && pos.y == j)) {
                    func(i, j);
                }
            }
        }
    }

    void testBishop(logic::Bishop& bishop) {
        Coord pos = bishop.getPos();
        checkAllCasesExceptPos(pos, [&](int i, int j){expectIllegalMove(bishop, i, j);});

        this->resetBoard();

        checkAllCasesExceptPos(pos, [&](int i, int j){
            if ((pos.y == j) || (abs(pos.y - j) != abs(pos.x - i)))
                expectIllegalMove(bishop, i, j);
            else
                expectLegalMove(bishop, i, j);
        });
    }


    void testRook(logic::Rook& rook){
        Coord pos = rook.getPos();
        checkAllCasesExceptPos(pos, [&](int i, int j){expectIllegalMove(rook, i, j);});

        this->resetBoard();
        checkAllCasesExceptPos(pos, [&](int i, int j){
            if ((pos.y == j) || (pos.x == i)) {
                std::cout << "testing rook with " << i << "," << j << std::endl;
                expectLegalMove(rook, i, j);
            }else
                expectIllegalMove(rook, i, j);
        });
    }

    void testPawn(logic::Pawn& pawn){ // To come back so we make sure it test the right thing + test en passant
        Coord pos = pawn.getPos();
        int modif = pawn.getType().color == Color::white ? 1 : -1;

        checkAllCasesExceptPos(pos, [&](int i, int j){
            if((i == pos.x) && (((pos.y + (modif * 1) ) == j) || ((pos.y + (modif * 2)) == j)))
                expectLegalMove(pawn, i, j);
            else
                expectIllegalMove(pawn, i, j);
        });
    }


//    void testKnight(logic::Knight& knight){
//        Coord pos = knight.getPos();
//
//        checkAllCasesExceptPos(pos, [&](int i, int j){
//            if(i == )
//        })
//    }

    static std::string colorToStr(Color color){
        return color == Color::white ? "white" : "black";
    }


private:
    controller::QtGame game;
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


TEST_F(BoardTestFixture, BlackBishop) {
    logic::Bishop bishopBlack({2, 7}, Color::black);
    testBishop(bishopBlack);
}

TEST_F(BoardTestFixture, WhiteBishop){
    logic::Bishop bishopWhite({2, 0}, Color::white);
    testBishop(bishopWhite);
}


TEST_F(BoardTestFixture, BlackRook) {
    logic::Rook rookBlack({0, 7}, Color::black);
    testRook(rookBlack);
}

TEST_F(BoardTestFixture, WhiteRook){
    logic::Rook rookWhite({0, 0}, Color::white);
    testRook(rookWhite);
}

TEST_F(BoardTestFixture, BlackPawn){
    for (int i = 0; i < 8; ++i) {
        logic::Pawn pawnBlack({i, 6}, Color::black);
        testPawn(pawnBlack);
    }
}

TEST_F(BoardTestFixture, WhitePawn){
    for (int i = 0; i < 8; ++i) {
        logic::Pawn pawnWhite({i, 1}, Color::white);
        testPawn(pawnWhite);
    }
}
