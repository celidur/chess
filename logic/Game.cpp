//
// Created by Gaetan on 2023-04-05.
//

#include "Game.h"
#include <iostream>

namespace chess {

    Game::Game() : playerRound_(Color::white), pieceSelected_(nullptr), selection_{{-1, -1},
                                                                                   {-1, -1},
                                                                                   {-1, -1},
                                                                                   {-1, -1}} {
        player_.emplace_back(Color::black);
        player_.emplace_back(Color::white);
        update();
    }

    Game::Game(const TypePiece board[8][8], Color color) : playerRound_(color), pieceSelected_(nullptr),
                                                                   selection_{{-1, -1},
                                                                               {-1, -1},
                                                                               {-1, -1},
                                                                               {-1, -1}} {
        copyBoard(board,board_);
        player_.emplace_back(Color::black, board_);
        player_.emplace_back(Color::white, board_);
        update();
    }

    void Game::selectionCase(Coord pos) {
        if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7)
            return;

        if (pieceSelected_ != nullptr && pieceSelected_->getColor() != playerRound_) {
            pieceSelected_ = nullptr;
            selection_[0] = {-1, -1};
        }

        if (board_[pos.x][pos.y].type != Type::none && board_[pos.x][pos.y].color == playerRound_) {
            pieceSelected_ = player_[(int) playerRound_].getPiece(pos);
            selection_[0] = pos;
        } else if (pieceSelected_ != nullptr) {
            Coord previous = pieceSelected_->getPos();
            if (pieceSelected_->move(board_, pos)) {
                Color other = (playerRound_ == Color::white ? Color::black : Color::white);
                if (board_[pos.x][pos.y].color == other) {
                    player_[(int) other].getPiece(pos)->kill();
                }
                if (pieceSelected_->getType().type == Type::king && abs(pos.x - previous.x) == 2) {
                    Coord rookPos = {pos.x == 6 ? 7 : 0, pos.y};
                    Coord rookNewPos = {pos.x == 6 ? 5 : 3, pos.y};
                    player_[(int) playerRound_].getPiece(rookPos)->move(board_, rookNewPos);
                }
                if (pieceSelected_->getType().type == Type::pawn && abs(pos.x - previous.x) == 1 &&
                    board_[pos.x][pos.y].type == Type::none) {
                    Coord enPassantPos = {pos.x, previous.y};
                    player_[(int) other].getPiece(enPassantPos)->kill();
                }
            }
            selection_[1] = previous;
            selection_[2] = pos;
            playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
            update();
            pieceSelected_ = nullptr;
            selection_[0] = {-1, -1};
        }
    }


    void Game::update() {
        clearTypePieceBoard();

        promotionPos_ = {-1, -1};
        updatePlayerBoard(0);
        updatePlayerBoard(1);

        if (promotionPos_ != Coord{-1, -1})
            playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
        Color other = (playerRound_ == Color::white ? Color::black : Color::white);
        player_[(int) playerRound_].update(board_, player_[(int) other]);
        Coord pos = player_[(int) playerRound_].getKingPos();
        selection_[3] = {-1, -1};

        checkGameState(pos);
    }

    void Game::clearTypePieceBoard() {
        for (auto&& pieces : board_) {
            for (auto&& piece: pieces) {
                piece = TypePiece{Color::none, Type::none};
            }
        }
    }

    void Game::updatePlayerBoard(int playerNumber) {
        Coord promotionPos = player_[playerNumber].updateBoard(board_);
        if (promotionPos != Coord{-1, -1}) {
            promotionPos_ = promotionPos;
        }
    }

    void Game::checkGameState(const Coord &pos) {
        switch (player_[(int) playerRound_].getState()) {
            case State::checkmate:
                displayMessage("Echec et mat");
                selection_[3] = pos;
                break;
            case State::stalemate:
                displayMessage("Pat");
                break;
            case State::check:
                displayMessage("Echec");
                selection_[3] = pos;
                break;
            default:
                break;
        }
    }

    void Game::updateBoard(screen::BoardBase &board) {
        std::vector<Coord> movePossible =
                pieceSelected_ != nullptr ? pieceSelected_->getPossibleMoves() : std::vector<Coord>();
        switch (mode_) {
            case Mode::game:
                if (rotation_)
                    board.viewBoard(playerRound_);

                board.updateGame(
                        selection_,
                        board_,
                        movePossible,
                        promotionPos_ != Coord{-1, -1} ? playerRound_: Color::none);
                break;
            case Mode::personalisation:
                board.updatePersonnalisation(board_);
                break;
        }
    }

    TypePiece (&Game::getBoard())[8][8] {
        return board_;
    }

    void Game::displayMessage(const std::string &msg) {
        std::cout << msg << std::endl;
    }

    void Game::promotion(Type type) {
        player_[(int) playerRound_].addPiece(type, promotionPos_);
        playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
        update();
    }

    void Game::loadGame() {
        if (!isKingDefined()){
            displayMessage("There is too much kings!");
            return;
        }

        setMode(Mode::game);
        Piece::reset();
        player_.clear();
        player_.emplace_back(Color::black, board_);
        player_.emplace_back(Color::white, board_);
        update();
    }

    void Game::resetBoard() {
        for (auto&& line : board_) {
            for (auto&& boardCase : line) {
                boardCase.color = Color::none;
                boardCase.type = Type::none;
            }
        }
    }

    void Game::setDefaultBoard() {
        resetBoard();
        for (auto & column : board_) {
            column[1] = {Color::white, Type::pawn};
            column[6] = {Color::black, Type::pawn};
        }
        board_[0][0] = {Color::white, Type::rook};
        board_[1][0] = {Color::white, Type::knight};
        board_[2][0] = {Color::white, Type::bishop};
        board_[3][0] = {Color::white, Type::king};
        board_[4][0] = {Color::white, Type::queen};
        board_[5][0] = {Color::white, Type::bishop};
        board_[6][0] = {Color::white, Type::knight};
        board_[7][0] = {Color::white, Type::rook};
        board_[0][7] = {Color::black, Type::rook};
        board_[1][7] = {Color::black, Type::knight};
        board_[2][7] = {Color::black, Type::bishop};
        board_[3][7] = {Color::black, Type::king};
        board_[4][7] = {Color::black, Type::queen};
        board_[5][7] = {Color::black, Type::bishop};
        board_[6][7] = {Color::black, Type::knight};
        board_[7][7] = {Color::black, Type::rook};
    }

    bool Game::isKingDefined() {
        int white = 0;
        int black = 0;
        for (auto&& col : board_) {
            for (auto&& boardCase : col) {
                if (boardCase.type == Type::king) {
                    if (boardCase.color == Color::white)
                        ++white;
                    else
                        ++black;
                }
            }
        }
        return white <= 1 && black <= 1;
    }

    Mode Game::getMode() {
        return mode_;
    }

    void Game::addPiece(Coord &pos, TypePiece &type) {
        board_[pos.x][pos.y] = type;
    }

    bool Game::isRotation() const {
        return rotation_;
    }

    void Game::setRotation(bool rotation) {
        rotation_ = rotation;
    }

    void Game::setMode(Mode mode) {
        mode_ = mode;
    }

    const Coord &Game::getSelectedCoord() const {
        return selectedCoord_;
    }

    void Game::setSelectedCoord(const Coord &selectedCoord) {
        selectedCoord_ = selectedCoord;
    }

    Color Game::getPlayerRound() const {
        return playerRound_;
    }

    void Game::setPlayerRound(Color playerRound) {
        playerRound_ = playerRound;
    }
}