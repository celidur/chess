#ifndef PLAYER
#define PLAYER

#include "logic/Queen.h"
#include "logic/Pawn.h"
#include "logic/Knight.h"
#include "logic/King.h"


namespace chess {

    enum class State {
        normal,
        check,
        checkmate,
        stalemate
    };


    class Player {
    public:
        explicit Player(Color color = Color::white);

        Player(Color color, TypePiece board[8][8]);

        void update(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8]);

        void addPiece(Type type, const Coord& pos);

        Coord updateBoard(TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8]);

        [[nodiscard]] State getState() const;

        [[nodiscard]] Color getColor() const { return playerColor_; }

        [[nodiscard]] Coord getKingPos() const { return kingPos_; }

    private:
        std::vector<std::shared_ptr<Piece>> pieces_;
        size_t nbMove_;
        bool isCheck_;
        Color playerColor_;
        Coord kingPos_;
    };

}

#endif // !PLAYER


