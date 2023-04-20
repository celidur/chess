/**
* \file   Game.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "Game.h"
#include <iostream>

namespace logic {

    Game::Game() : playerRound_(Color::white), rotation_(false), mode_(Mode::personalisation), selection_{{-1, -1},
                                                                                                         {-1, -1},
                                                                                                         {-1, -1},
                                                                                                         {-1, -1}} {
        setDefaultBoard(board_);
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
            auto moves = player_[(int) playerRound_].getPossibleMoves(pos);
            updateSelection(pos,moves);
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
                    movePiece(rookPos, rookNewPos);
                }
                else if (piece.type == Type::pawn && abs(pos.x - selection_[0].x) == 1 &&
                    board_[pos.x][pos.y].type == Type::none) {
                    auto pos_ = Coord{pos.x, selection_[0].y};
                    player_[(int) other].removePiece(pos_);
                    killPiece(pos_);
                }
                playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
                if (piece.type == Type::pawn && (pos.y == 0 || pos.y == 7)) {
                    promotionPos_ = pos;
                    playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
                }
                selection_[1] = selection_[0];
                selection_[2] = pos;
                movePiece(selection_[0], pos);
                update();
            }
            selection_[0] = {-1, -1};
            std::vector<Coord> moves;
            updateSelection(selection_[0], moves);
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
        if (rotation_)
            viewBoard(playerRound_);
    }

    void Game::clearTypePieceBoard() {
        for (auto&& pieces: board_) {
            for (auto&& piece: pieces) {
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
                // displayMessage("Echec");
                selection_[3] = kingPos;
                break;
            case State::normal:
                break;
        }
        updateCheck();
    }


    void Game::displayMessage(const std::string& msg) {
        std::cout << msg << std::endl;
    }

    void Game::promotion(Type type) {
        player_[(int) playerRound_].addPiece(type, promotionPos_, true);
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

    void Game::resetBoard(TypePiece board[8][8]) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                board[i][j].color = Color::none;
                board[i][j].type = Type::none;
            }
        }
    }

    void Game::setDefaultBoard(TypePiece board[8][8]) {
        resetBoard(board);
        for (int i = 0; i < 8; ++i) {
            board[i][1] = {Color::white, Type::pawn};
            board[i][6] = {Color::black, Type::pawn};
        }
        board[0][0] = {Color::white, Type::rook};
        board[1][0] = {Color::white, Type::knight};
        board[2][0] = {Color::white, Type::bishop};
        board[3][0] = {Color::white, Type::king};
        board[4][0] = {Color::white, Type::queen};
        board[5][0] = {Color::white, Type::bishop};
        board[6][0] = {Color::white, Type::knight};
        board[7][0] = {Color::white, Type::rook};
        board[0][7] = {Color::black, Type::rook};
        board[1][7] = {Color::black, Type::knight};
        board[2][7] = {Color::black, Type::bishop};
        board[3][7] = {Color::black, Type::king};
        board[4][7] = {Color::black, Type::queen};
        board[5][7] = {Color::black, Type::bishop};
        board[6][7] = {Color::black, Type::knight};
        board[7][7] = {Color::black, Type::rook};
    }

    bool Game::isKingDefined() {
        int white = 0;
        int black = 0;
        for (auto&& col: board_) {
            for (auto&& boardCase: col) {
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

    void Game::addPiece(Coord& pos, TypePiece& type) {
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