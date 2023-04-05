#ifndef BOARD
#define    BOARD

#include "struct.h"
#include <BoardBase.hpp>
#include <QMainWindow>

namespace screen {

    class Board : public BoardBase, public QMainWindow {
        Q_OBJECT
    public:
        Board(CoordF windowSize, QWidget *parent);

        ~Board() override = default;

        void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord>& piecePossibleMove);

        void setMenu();

    private:
        void draw() const;

    };

}

#endif // !BOARD
