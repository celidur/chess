#ifndef BOARD
#define    BOARD

#include "common/struct.h"
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

    enum class ZLayer {
        bottom,
        middle,
        top
    };


    class Board : public QGraphicsScene, public BoardBase {
    Q_OBJECT
    public:
        explicit Board(CoordF tileSize, const std::string &resFile, QWidget *parent = nullptr);

        ~Board() override = default;

        void update(
                Coord selection[4],
                TypePiece boardGame[8][8],
                std::vector<Coord> &piecePossibleMove,
                Color color) override;

        void update(TypePiece boardGame[8][8]) override;

        void viewBoard(Color color) override;

        QImage getImage(Coord pos);

        void drawRect(QColor color, Coord pos, ZLayer zLayer, bool isPromote);

    signals:

        QEvent *caseClicked(Coord coord, screen::Board &board);

        QEvent* addPiece(TypePiece typePiece, Coord pos, screen::Board& board);

        QEvent *loadGame(screen::Board &board);

        QEvent *promoteClicked(TypePiece, screen::Board &board);

        QEvent* resetBoard(Board& board);

        QEvent* setDefaultBoard(Board& board);

    public slots:
        void displayMessage(const QString& s);

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

        inline static CoordF tileSize_ = {0, 0};

        QImageReader textureLoader_;

        bool side_ = true;

        TypePiece selectedPiece_ = {Color::none, Type::none};

        Color selectedColor_ = Color::white;

        Coord selectedCoord_ = {8, 7};

//        TypePiece board_[8][8];

        Color promoteColor_ = Color::none;
    };

}

#endif // !BOARD
