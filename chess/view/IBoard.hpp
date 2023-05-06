#ifndef CHESS_IBOARD_HPP
#define CHESS_IBOARD_HPP

#include "BoardBase.h"
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
#include <QMessageBox>
#include <QCheckBox>
#include "Piece.h"

namespace view{


    class IBoard : public QGraphicsScene, public BoardBase {
    Q_OBJECT

    public:
        explicit IBoard(CoordF tileSize, const std::string& resFile, QWidget* parent = nullptr);

        ~IBoard() override = default;

        virtual void removeLayer(const ZLayer& zLayer) = 0;

        virtual void setLayer1() = 0;

        virtual void setLayer2(const std::array<std::array<TypePiece, xBoard>, yBoard>& board) = 0;

        void resetBoard();

        virtual QImage getImage(const Coord& pos) = 0;

        virtual void drawRect(const QColor& color, const Coord& pos, const ZLayer& zLayer, bool isPromote,
                              const std::string& text) = 0;

        virtual QGraphicsPixmapItem*
        addImage(const QImage& img, const Coord& coord, const ZLayer& zLayer, bool isPromote) = 0;

        virtual QGraphicsPixmapItem*
        addImage(const QImage& img, const CoordF& coord, const ZLayer& zLayer, bool isPromote) = 0;


        [[nodiscard]] TypePiece getPieceToPromote(const Coord& pos) const;

        [[nodiscard]] std::array<QGraphicsPixmapItem *, 4>& getACase();

        [[nodiscard]] std::array<Coord, 4> & getSelection();

        [[nodiscard]] const QImageReader &getTextureLoader() const;

        [[nodiscard]] bool isSide() const;

        void setSide(bool side);

        [[nodiscard]] Color getPromoteColor() const;

        void setPromoteColor(Color promoteColor);

        [[nodiscard]] bool isRotation() const;

        void setRotation(bool rotation);

        [[nodiscard]] const CoordF &getTileSize() const;

        void setTileSize(const CoordF &tileSize);

//    signals:

        virtual QEvent* caseClicked(Coord& coord, view::IBoard& board) = 0;

        virtual QEvent* pieceAdded(TypePiece& typePiece, Coord& pos, view::IBoard& board) = 0;

        virtual QEvent* gameStarted(view::IBoard& board) = 0;

        virtual QEvent* promoteClicked(TypePiece&, view::IBoard& board) = 0;

        virtual QEvent* boardReset(IBoard& board) = 0;

        virtual QEvent* boardDefaulted(IBoard& board) = 0;

        virtual QEvent* playerSwitched(Color color, IBoard& board) = 0;

        virtual QEvent* rotationSwitched(view::IBoard& board) = 0;

    public slots:

        virtual void displayMessage(const QString& s) = 0;

    protected:

        virtual QGraphicsPixmapItem* addCase(const Coord& pos, const ZLayer& zLayer) = 0;

        std::unique_ptr<Piece> board_[xBoard][yBoard];
        std::array<QGraphicsPixmapItem*, 4> case_{};
        std::array<Coord, 4> selection_;
        QImageReader textureLoader_;
        bool side_;
        Color promoteColor_;
        bool rotation_;
        CoordF tileSize_;
    };

}

#endif //CHESS_IBOARD_HPP
