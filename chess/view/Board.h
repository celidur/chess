/**
* \file   Bishop.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef BOARD
#define BOARD


#include "InterfaceModeState.hpp"
#include "GameMode.hpp"
#include "PersonnalisationMode.hpp"
#include "IBoard.hpp"
#include "StateFactory.hpp"

namespace view {

    class Board : public IBoard {
    Q_OBJECT

    public:
        explicit Board(CoordF tileSize, const std::string& resFile, Mode mode, QWidget* parent = nullptr);

        ~Board() override = default;

        void removeLayer(const ZLayer& zLayer) override;

        void setLayer1() override;

        void setLayer2(const std::array<std::array<TypePiece, xBoard>, yBoard>& board) override;

        QImage getImage(const Coord& pos) override;

        void drawRect(const QColor& color, const Coord& pos, const ZLayer& zLayer, bool isPromote,
                      const std::string& text) override;

        QGraphicsPixmapItem*
        addImage(const QImage& img, const Coord& coord, const ZLayer& zLayer, bool isPromote) override;

        QGraphicsPixmapItem*
        addImage(const QImage& img, const CoordF& coord, const ZLayer& zLayer, bool isPromote) override;

    signals:

        QEvent* caseClicked(Coord& coord, view::IBoard& board) override;

        QEvent* pieceAdded(TypePiece& typePiece, Coord& pos, view::IBoard& board) override;

        QEvent* gameStarted(view::IBoard& board) override;

        QEvent* promoteClicked(TypePiece&, view::IBoard& board) override;

        QEvent* boardReset(IBoard& board) override;

        QEvent* boardDefaulted(IBoard& board) override;

        QEvent* playerSwitched(Color color, IBoard& board) override;

        QEvent* rotationSwitched(view::IBoard& board) override;

    public slots:

        void displayMessage(const QString& s) override;

        void updateGame(
                const Coord selection[4],
                const std::array<std::array<TypePiece, xBoard>, yBoard>& board,
                const std::vector<Coord>& piecePossibleMove,
                const Color& color, const std::vector<TypePiece> deadPieces[2], int point) override;

        void updateDeadPieces(const std::vector<TypePiece> deadPieces[2], int point) override;

        void movePiece(Coord& older, Coord& newer) override;

        void selectPiece(const Coord& pos, const std::vector<Coord>& piecePossibleMove) override;

        void viewBoard(const Color& color) override;

        void updatePersonalization(const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame) override;

        void updatePiece() override;

        void killPiece(const Coord& pos) override;

        void addPiece(const TypePiece& typePiece, const Coord& pos) override;

        void updateCheckState(const Coord& pos) override;

        void updatePanel(const std::vector<TypePiece> deadPieces[2], int point) override;

        void updatePanelP() override;

        void promote(const Color& color) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

        QGraphicsPixmapItem* addCase(const Coord& pos, const ZLayer& zLayer) override;

        InterfaceModeState* mode_;
    };

}

#endif // !BOARD
