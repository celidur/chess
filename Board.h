#ifndef BOARD
#define    BOARD

#include "struct.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QImageReader>
#include <qnamespace.h>
#include <QColor>
#include <QColorTransform>
#include <QPainter>
#include <memory>

namespace screen {
    struct CoordF {
        float x, y;
    };

    struct TypePiece {
        Color color = Color::none;
        Type type = Type::none;
    };

    enum class ZLayer {
        bottom,
        middle,
        top
    };

    enum class Mode {
        game,
        menu,
        personalisation
    };

    void resetBoard(TypePiece board[8][8]);

    void setDefaultBoard(TypePiece board[8][8]);

    class Board : public QGraphicsScene {
    Q_OBJECT
    public:
        explicit Board(CoordF tileSize, const std::string &resFile, TypePiece board[8][8], QWidget *parent = nullptr);

        ~Board() override = default;

        void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord> &piecePossibleMove,
                    Color color = Color::none);

        void viewBoard(Color color);

        auto getBoard() -> TypePiece(*)[8];

    signals:

        QEvent *caseClicked(Coord coord, screen::Board &board);

        QEvent *loadGame(screen::Board &board);

        QEvent *promoteClicked(screen::TypePiece, screen::Board &board);


    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        QImage getPieceImg(const QRect &pieceRect);

        void addImage(QImage &img, Coord coord, ZLayer zLayer, bool isPromote = false);

        void addImage(QImage &img, CoordF coord, ZLayer zLayer, bool isPromote = false);

        void setLayer1(Coord sel[4]= nullptr);

        void setLayer2(TypePiece board[8][8]);

        void selectPiece();

        void promote();

        void setPossibleMoves(std::vector<Coord> &piecePossibleMove);

        QImageReader textureLoader_;
        inline static CoordF tileSize_ = {0, 0};

        bool side_ = true;

        TypePiece selectedPiece_ = {Color::none, Type::none};

        Color selectedColor_ = Color::white;

        Coord selectedCoord_ = {8, 7};

        Mode mode_ = Mode::personalisation;

        TypePiece board_[8][8];

        Color promoteColor_ = Color::none;
    };

}

#endif // !BOARD
