#include "QtGame.h"

namespace chess {
    QtGame::QtGame() : QObject(nullptr) {}

    void QtGame::updateGameState(Coord coord, screen::Board& board) {
        selectionCase(coord);
        updateBoard(board);
    }

    QtGame::~QtGame() = default;

//    void QtGame::displayMessage(std::string msg) {
//        QMessageBox msgBox;
//        msgBox.setWindowTitle("Chess message");
//        msgBox.setText(QString::fromStdString(msg));
//        msgBox.addButton(QMessageBox::Close);
//        std::cout << msg << std::endl;
//        msgBox.exec();
//
//    }
}
