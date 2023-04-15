/**
* \file   Game.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "Game.h"
#include <iostream>

namespace chess {

    Game::Game() : playerRound_(Color::white), rotation_(true), mode_(Mode::personalisation), selection_{{-1, -1},
                                                                                                         {-1, -1},
                                                                                                         {-1, -1},
                                                                                                         {-1, -1}} {
        setDefaultBoard();
        if (Mode::game == mode_) {
            player_.emplace_back(Color::black, board_);
            player_.emplace_back(Color::white, board_);
            update();
        }
    }

    Game::Game(const TypePiece board[8][8], Color color) : playerRound_(color), rotation_(true),
                                                           mode_(Mode::game),
                                                           selection_{{-1, -1},
                                                                      {-1, -1},
                                                                      {-1, -1},
                                                                      {-1, -1}} {
        copyBoard(board, board_);
        player_.emplace_back(Color::black, board_);
        player_.emplace_back(Color::white, board_);
        update();
    }

    void Game::selectionCase(Coord pos) {
        if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7)
            return;

        if (board_[pos.x][pos.y].type != Type::none && board_[pos.x][pos.y].color == playerRound_) {
            selection_[0] = pos;
        } else if (selection_[0] != Coord{-1, -1}) {
            if (player_[(int) playerRound_].move(board_, selection_[0], pos)) {
                promotionPos_ = {};
                Color other = (playerRound_ == Color::white ? Color::black : Color::white);
                if (board_[pos.x][pos.y].color == other) {
                    player_[(int) other].removePiece(pos);
                }
                auto piece = board_[selection_[0].x][selection_[0].y];
                if (piece.type == Type::king && abs(pos.x - selection_[0].x) == 2) {
                    Coord rookPos = {pos.x == 5 ? 7 : 0, pos.y};
                    Coord rookNewPos = {pos.x == 5 ? 4 : 2, pos.y};
                    player_[(int) playerRound_].move(board_, rookPos, rookNewPos);
                }
                if (piece.type == Type::pawn && abs(pos.x - selection_[0].x) == 1 &&
                    board_[pos.x][pos.y].type == Type::none) {
                    player_[(int) other].removePiece({pos.x, selection_[0].y});
                }
                playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
                if (piece.type == Type::pawn && (pos.y == 0 || pos.y == 7)) {
                    promotionPos_ = pos;
                    playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
                }
                selection_[1] = selection_[0];
                selection_[2] = pos;
                update();
            }
            selection_[0] = {-1, -1};
        }
    }


    void Game::update() {
        clearTypePieceBoard();
        player_[0].updateBoard(board_);
        player_[1].updateBoard(board_);

        Color other = (playerRound_ == Color::white ? Color::black : Color::white);
        player_[(int) playerRound_].update(board_, player_[(int) other]);

        selection_[3] = {-1, -1};
        checkGameState();
    }

    void Game::clearTypePieceBoard() {
        for (auto &&pieces: board_) {
            for (auto &&piece: pieces) {
                piece = {};
            }
        }
    }

    void Game::checkGameState() {
        auto kingPos = player_[(int) playerRound_].getKingPos();
        switch (player_[(int) playerRound_].getState()) {
            case State::checkmate:
                displayMessage("Echec et mat");
                selection_[3] = kingPos;
                break;
            case State::stalemate:
                displayMessage("Pat");
                break;
            case State::check:
                displayMessage("Echec");
                selection_[3] = kingPos;
                break;
            case State::normal:
                break;
        }
    }

    void Game::updateBoard() {
        if (mode_ == Mode::game) {
            if (rotation_)
                viewBoard(playerRound_);
            auto movePossible = player_[(int) playerRound_].getPossibleMoves(selection_[0]);
            std::vector<TypePiece> deadPieces[2] = {player_[0].getDeadPieces(), player_[1].getDeadPieces()};
            updateGameBoard(selection_, board_, movePossible,
                            promotionPos_ != Coord{-1, -1} ? playerRound_ : Color::none, deadPieces);
        } else {
            updatePersonalizationBoard(board_);
        }
    }

    void Game::displayMessage(const std::string &msg) {
        std::cout << msg << std::endl;
    }

    void Game::promotion(Type type) {
        player_[(int) playerRound_].addPiece(type, promotionPos_);
        playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
        promotionPos_ = {};
        update();
    }

    void Game::loadGame() {
        if (!isKingDefined()) {
            displayMessage("There is too much kings!");
            return;
        }
        mode_ = Mode::game;
        Piece::reset();
        player_.clear();
        player_.emplace_back(Color::black, board_);
        player_.emplace_back(Color::white, board_);
        update();
    }

    void Game::resetBoard() {
        for (auto &&line: board_) {
            for (auto &&boardCase: line) {
                boardCase.color = Color::none;
                boardCase.type = Type::none;
            }
        }
    }

    void Game::setDefaultBoard() {
        resetBoard();
        for (auto &column: board_) {
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
        for (auto &&col: board_) {
            for (auto &&boardCase: col) {
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

    void Game::setPlayerRound(Color playerRound) {
        playerRound_ = playerRound;
    }
}