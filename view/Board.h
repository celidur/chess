#ifndef BOARD
#define    BOARD

#include "common/fuction.h"
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
        explicit Board(CoordF tileSize, const std::string &resFile,  Mode mode, QWidget *parent = nullptr);

        ~Board() override = default;

        void updateGame(
                Coord selection[4],
                TypePiece boardGame[8][8],
                std::vector<Coord> &piecePossibleMove,
                Color color) override;

        void updatePersonnalisation(TypePiece boardGame[8][8]) override;

        void viewBoard(Color color) override;

        QImage getImage(Coord pos);

        void drawRect(QColor color, Coord pos, ZLayer zLayer, bool isPromote, const std::string& text = "");

    signals:

        QEvent *caseClicked(Coord& coord, screen::Board &board);

        QEvent* pieceAdded(TypePiece& typePiece, Coord& pos, screen::Board& board);

        QEvent *gameStarted(screen::Board &board);

        QEvent *promoteClicked(TypePiece&, screen::Board &board);

        QEvent* boardReset(Board& board);

        QEvent* boardDefaulted(Board& board);

        QEvent* playerSwitched(Color color, Board& board);

        QEvent* rotationSwitched();

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

        void showPersonnalisationMenu();

        void promote();

        void setPossibleMoves(std::vector<Coord> &piecePossibleMove);

        void handleGameMode(Coord& pos);
        void handlePersonnalisationMode(Coord &pos);

        [[nodiscard]] TypePiece getPieceToPromote(const Coord &pos) const;

        inline static CoordF tileSize_ = {0, 0};
        QImageReader textureLoader_;
        bool side_ = true;
        TypePiece selectedPiece_ = {Color::none, Type::none};
        Color selectedColor_ = Color::white;
        Coord selectedCoord_;
        Color promoteColor_ = Color::none;
        Mode mode_;
        bool rotation = true;
    };

}

#endif // !BOARD
