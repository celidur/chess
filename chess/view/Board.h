/**
* \file   Bishop.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef BOARD
#define BOARD

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

namespace view {

    enum class ZLayer {
        board,
        caseSelected,
        check,
        piece,
        move,
        top,
        promote,
    };

    class Board : public QGraphicsScene, public BoardBase {
    Q_OBJECT
    public:
        explicit Board(CoordF tileSize, const std::string& resFile, Mode mode, QWidget* parent = nullptr);

        ~Board() override = default;

    signals:

        QEvent* caseClicked(Coord& coord, view::Board& board);

        QEvent* pieceAdded(TypePiece& typePiece, Coord& pos, view::Board& board);

        QEvent* gameStarted(view::Board& board);

        QEvent* promoteClicked(TypePiece&, view::Board& board);

        QEvent* boardReset(Board& board);

        QEvent* boardDefaulted(Board& board);

        QEvent* playerSwitched(Color color, Board& board);

        QEvent* rotationSwitched(view::Board& board);

    public slots:

        static void displayMessage(const QString& s);

        void updateGame(
                const Coord selection[4],
                const TypePiece boardGame[xBoard][yBoard],
                const std::vector<Coord>& piecePossibleMove,
                const Color& color, const std::vector<TypePiece> deadPieces[2], int point) override;

        void updateDeadPieces(const std::vector<TypePiece> deadPieces[2], int point) override;

        void movePiece(Coord& older, Coord& newer) override;

        void selectPiece(const Coord& pos, const std::vector<Coord>& piecePossibleMove) override;

        void viewBoard(const Color& color) override;

        void updatePersonalization(const TypePiece boardGame[xBoard][yBoard]) override;

        void updatePersonalizationMenu() override;

        void updatePiece() override;

        void killPiece(const Coord& pos) override;

        void addPiece(const TypePiece& typePiece, const Coord& pos) override;

        void updateCheckState(const Coord& pos) override;

        void updatePanel(const std::vector<TypePiece> deadPieces[2], int point) override;

        void promote(const Color& color) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    private:

        QImage getImage(const Coord& pos);

        void drawRect(const QColor& color, const Coord& pos, const ZLayer& zLayer, bool isPromote,
                      const std::string& text = "");

        QGraphicsPixmapItem*
        addImage(const QImage& img, const Coord& coord, const ZLayer& zLayer, bool isPromote = false);

        QGraphicsPixmapItem*
        addImage(const QImage& img, const CoordF& coord, const ZLayer& zLayer, bool isPromote = false);

        void setLayer1();

        void removeLayer(const ZLayer& zLayer);

        void setLayer2(const TypePiece board[xBoard][yBoard]);

        QGraphicsPixmapItem* addCase(const Coord& pos, const ZLayer& zLayer);

        void handleGameMode(const Coord& pos);

        void handlePersonalizationMode(const Coord& pos);

        void resetBoard();

        [[nodiscard]] TypePiece getPieceToPromote(const Coord& pos) const;

        std::unique_ptr<Piece> board_[xBoard][yBoard];
        QGraphicsPixmapItem* case_[4]{};
        Coord selection_[4];
        QImageReader textureLoader_;
        bool side_;
        TypePiece selectedPiece_;
        Color selectedColor_;
        Color promoteColor_;
        Mode mode_;
        bool rotation_;
        CoordF tileSize_;
    };

}

#endif // !BOARD
