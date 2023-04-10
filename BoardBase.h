#ifndef CHESS_BOARDBASE_H
#define CHESS_BOARDBASE_H

#include <vector>
#include "struct.h"

namespace screen {
    struct CoordF {
        float x, y;
    };

    struct TypePiece {
        Colour color = Colour::none;
        Type type = Type::none;
    };

    class BoardBase {
    public:
        virtual ~BoardBase() = default;

        virtual void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord> &piecePossibleMove) = 0;
    };

}

#endif //CHESS_BOARDBASE_H
