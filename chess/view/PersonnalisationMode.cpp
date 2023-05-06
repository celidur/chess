#include "PersonnalisationMode.h"

namespace view {
    void PersonnalisationMode::handleMouseEvent(IBoard &board, Coord &pos) {
        Coord selection = pos;
        if (0 <= pos.x && pos.x < xBoard && 0 <= pos.y && pos.y < yBoard) {
            if (board.isSide())
                selection = {xBoard - 1 - pos.x, yBoard - 1 - pos.y};
            emit board.pieceAdded(selectedPiece_, selection, board);
        } else if (pos.x == xBoard && 1 <= pos.y && pos.y < 8) {
            selectedPiece_ =  {selectedColor_, (Type) (pos.y - 1)};
            handleUpdatePanel(board);
        } else if (pos.x == xBoard && pos.y == 0) {
            selectedColor_ = selectedColor_ == Color::white ? Color::black : Color::white;
            selectedPiece_.color = selectedColor_;
            handleUpdatePanel(board);
        } else if (pos.x == xBoard + 1 && pos.y == 0) {
            emit board.gameStarted(board);
        } else if (pos.x == xBoard + 1 && pos.y == 1) {
            emit board.boardDefaulted(board);
        } else if (pos.x == xBoard + 1 && pos.y == 2) {
            emit board.boardReset(board);
        } else if (pos.x == xBoard + 1 && pos.y == 3) {
            board.setSide(!board.isSide());
            emit board.playerSwitched(board.isSide() ? Color::white : Color::black, board);
        } else if (pos.x == xBoard + 1 && pos.y == 4) {
            board.setRotation(!board.isRotation());
            emit board.rotationSwitched(board);
        }
    }

    void PersonnalisationMode::handleUpdateEvent(IBoard &board, const Coord *selection, const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame,
                                                 const std::vector<Coord> &piecePossibleMove, Color color,
                                                 const std::vector<TypePiece> *deadPieces, int point) {}

    void PersonnalisationMode::handleUpdateEvent(IBoard &board, const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame) {
        for (auto&& i: board.getACase()) {
            i = nullptr;
        }
        board.resetBoard();
        board.clear(); // Clear all items
        board.setLayer1();
        board.setLayer2(boardGame);
        handleUpdatePanel(board);
    }

    void PersonnalisationMode::handleUpdatePanel(view::IBoard &board) {
        board.removeLayer(ZLayer::top);
        for (int i = 0; i < 7; i++) {
            auto r = ((int) selectedPiece_.type == i) ?
                     selectedPieceColor : inactivePieceColor;
            board.drawRect(r, Coord{xBoard, i + 1}, ZLayer::top, true, "");
            if (i == 6)
                continue;
            QImage img = board.getImage({i, (int) selectedColor_});
            board.addImage(img, Coord{xBoard, i + 1}, ZLayer::top, true);
        }
        board.drawRect(switchBtnColor, {xBoard, 0},
                 ZLayer::top, true, "");

        QImage img = board.getImage({6, (int) selectedColor_});
        float size = 17;
        img = img.scaled((int) (board.getTileSize().x - size * 2), (int) (board.getTileSize().y - size * 2),
                         Qt::KeepAspectRatio);
        board.addImage(img, CoordF{xBoard + size / board.getTileSize().x, size / board.getTileSize().y},
                 ZLayer::top, true);
        board.drawRect(
                playBtnColor, Coord{xBoard + 1, 0},
                ZLayer::top, true, "Play");
        board.drawRect(defaultBtnColor, Coord{xBoard + 1, 1},
                 ZLayer::top, true, "Default");
        board.drawRect(resetBtnColor, Coord{xBoard + 1, 2},
                 ZLayer::top, true, "Reset");
        board.drawRect(firstPlayerBtnColor, Coord{xBoard + 1, 3},
                 ZLayer::top, true,
                 board.isSide() ? "Set\nblack\nfirst" : "Set\nwhite\nfirst");
        board.drawRect(rotationBtnColor, Coord{xBoard + 1, 4},
                 ZLayer::top, true,
                 board.isRotation() ? "Disable\nrotation" : "Enable\nrotation");
    }

    void PersonnalisationMode::handleUpdatePanel(IBoard& board, const std::vector<TypePiece> deadPieces[2], int point) {}


    PersonnalisationMode::PersonnalisationMode() : InterfaceModeState(Mode::personalisation), selectedColor_(Color::white) {}
} // view