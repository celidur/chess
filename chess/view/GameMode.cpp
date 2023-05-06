#include "GameMode.h"

namespace view {
    void GameMode::handleMouseEvent(IBoard &board, Coord &pos) {
        if (board.isSide() && board.getPromoteColor() == Color::none)
            pos = {xBoard - 1 - pos.x, yBoard - 1 - pos.y};
        if (board.getPromoteColor() != Color::none) {
            TypePiece type = board.getPieceToPromote(pos);
            if (type.type != Type::none) {
                emit board.promoteClicked(type, board);
                board.setPromoteColor(Color::none);
                board.removeLayer(ZLayer::promote);
            } else
                return;
        }
        emit board.caseClicked(pos, board);
    }

    void GameMode::handleUpdateEvent(IBoard &board, const Coord *selection, const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame,
                                     const std::vector<Coord> &piecePossibleMove, Color color,
                                     const std::vector<TypePiece> *deadPieces, int point) {
        for (auto &&i: board.getACase()) {
            i = nullptr;
        }
        board.resetBoard();
        board.clear(); // Clear all items
        board.setLayer1();
        board.setLayer2(boardGame);
        board.selectPiece(selection[0], piecePossibleMove);
        board.updatePanel(deadPieces, point);
        if (color != Color::none) {
            board.promote(color);
        }
    }

    void GameMode::handleUpdateEvent(IBoard & board, const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame) {}

    void GameMode::handleUpdatePanel(IBoard &board) {}

    void GameMode::handleUpdatePanel(view::IBoard &board, const std::vector<TypePiece> *deadPieces, int point) {
        const CoordF tileSize = board.getTileSize();
        const bool side = board.isSide();
        QImage interfaceImg((int) (tileSize.x) * 2, (int) (tileSize.y) * 8,
                            QImage::Format::Format_ARGB32);
        QPainter interface(&interfaceImg);
        interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(),
                           QColor::fromRgb(209, 207, 206));
        interface.drawText(QRectF{50, (float) (side ? 0 : 1) * 6 * tileSize.y + 15,
                                  tileSize.x * 2, tileSize.y}, Qt::AlignLeft, "Player 2");
        interface.drawText(QRectF{50, (float) (side ? 1 : 0) * 6 * tileSize.y + 15,
                                  tileSize.x * 2, tileSize.y}, Qt::AlignLeft, "Player 1");
        auto p = (point <= 0 ? "+" : "") + std::to_string(point * -1);
        interface.drawText(QRectF{110, (float) (side ? 0 : 1) * 6 * tileSize.y + 15,
                                  tileSize.x * 2, tileSize.y}, Qt::AlignLeft, p.c_str());
        p = (point >= 0 ? "+" : "") + std::to_string(point);
        interface.drawText(QRectF{110, (float) (side ? 1 : 0) * 6 * tileSize.y + 15,
                                  tileSize.x * 2, tileSize.y}, Qt::AlignLeft, p.c_str());
        board.addImage(interfaceImg, Coord{xBoard, 0}, ZLayer::top, true);
        float size = 17;
        QImage img = board.getImage({5, side ? 0 : 1})
                .scaled((int) (tileSize.x - size * 2), (int) (tileSize.y - size * 2),
                        Qt::KeepAspectRatio);
        board.addImage(img, CoordF{xBoard, 0}, ZLayer::top, true);
        img = board.getImage({5, side ? 1 : 0})
                .scaled((int) (tileSize.x - size * 2), (int) (tileSize.y - size * 2),
                        Qt::KeepAspectRatio);
        board.addImage(img, CoordF{xBoard, 6}, ZLayer::top, true);
        for (int i = 0; i < 2; ++i) {
            float x = 0;
            float y = 0;
            size = 26;
            for (auto &&piece: deadPieces[i]) {
                img = board.getImage({(int) piece.type, i}).scaled((int) (tileSize.x - size * 2),
                                                             (int) (tileSize.y - size * 2),
                                                             Qt::KeepAspectRatio);
                if (x == 8) {
                    x = 0;
                    y++;
                }
                if (y == 4)
                    break;
                board.addImage(img, CoordF{xBoard + (x * (size * 0.67f)) / tileSize.x,
                                     (float) ((i == 0 == side ? 1 : 0) * 6) + (y * size * 0.9f + 40) / tileSize.y},
                         ZLayer::top,
                         true);
                x++;
            }

        }
    }


    GameMode::GameMode() : InterfaceModeState(Mode::game) {
    }

}