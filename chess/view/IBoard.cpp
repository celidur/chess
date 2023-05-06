#include "IBoard.h"


namespace view {
    IBoard::IBoard(const CoordF tileSize, const std::string& resFile, QWidget* parent) :
            QGraphicsScene(parent),
            tileSize_(tileSize),
            textureLoader_(),
            side_(true),
            rotation_(false),
            promoteColor_(Color::none) {

        textureLoader_.setFileName(QString::fromStdString(resFile));
        resetBoard();
        for (auto&& i: case_) {
            i = nullptr;
        }
    }

    void IBoard::resetBoard() {
        for (auto& i: board_)
            for (auto& j: i)
                j = nullptr;
    }

    std::array<QGraphicsPixmapItem*, 4>& IBoard::getACase() {
        return case_;
    }

    std::array<Coord, 4> & IBoard::getSelection() {
        return selection_;
    }

    const QImageReader &IBoard::getTextureLoader() const {
        return textureLoader_;
    }

    bool IBoard::isSide() const {
        return side_;
    }

    void IBoard::setSide(bool side) {
        side_ = side;
    }

    Color IBoard::getPromoteColor() const {
        return promoteColor_;
    }

    void IBoard::setPromoteColor(Color promoteColor) {
        promoteColor_ = promoteColor;
    }

    bool IBoard::isRotation() const {
        return rotation_;
    }

    void IBoard::setRotation(bool rotation) {
        rotation_ = rotation;
    }

    const CoordF &IBoard::getTileSize() const {
        return tileSize_;
    }

    void IBoard::setTileSize(const CoordF &tileSize) {
        tileSize_ = tileSize;
    }

    TypePiece IBoard::getPieceToPromote(const Coord& pos) const {
        TypePiece type = {promoteColor_, Type::none};
        if (pos.x == 3 && pos.y == 3)
            type.type = Type::queen;
        else if (pos.x == 4 && pos.y == 3)
            type.type = Type::rook;
        else if (pos.x == 3 && pos.y == 4)
            type.type = Type::bishop;
        else if (pos.x == 4 && pos.y == 4)
            type.type = Type::knight;
        return type;
    }
}
