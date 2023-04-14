/*
* Travail : $FOLDER_NAME-Rook.hpp
* Section # : 01
* Équipe # : 11
* Auteurs: Charles Khoury, Thomas Tzeng Souvanlasy
* Correcteur : -------
*/

#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H
#include "Piece.h"


namespace chess {
    class Rook : virtual public Piece {
    public:
        Rook(const Coord& pos, Color color);

        [[nodiscard]] bool getFirst() const;

        bool move(const TypePiece board[8][8], const Coord &pos) override;

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    private:
        bool first_ = true;
    };
}


#endif //CHESS_ROOK_H
