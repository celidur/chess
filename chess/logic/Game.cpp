/**
* \file   Game.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Game.h"
#include <iostream>

namespace logic {

    Game::Game() :
            playerRound_(Color::white),
            rotation_(false),
            mode_(Mode::personalisation),
            selection_{{-1, -1},
                       {-1, -1},
                       {-1, -1},
                       {-1, -1}} {
        setDefaultBoard();
    }

    void Game::selectionCase(Coord& pos) {
        if (pos < 0 || pos >= Coord{xBoard, yBoard})
            return;

        if (board_[pos.x][pos.y].type != Type::none && board_[pos.x][pos.y].color == playerRound_) {
            selection_[0] = pos;
            auto moves = player_[(int) playerRound_].getPossibleMoves(pos);
            updateSelection(pos, moves);
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
                } else if (piece.type == Type::pawn && abs(pos.x - selection_[0].x) == 1 &&
                           board_[pos.x][pos.y].type == Type::none) {
                    auto pos_ = Coord{pos.x, selection_[0].y};
                    player_[(int) other].removePiece(pos_);
                    killPiece(pos_);
                }
                playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
                if (piece.type == Type::pawn && (pos.y == 0 || pos.y == yBoard - 1)) {
                    promotionPos_ = pos;
                    playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
                    showPromotion();
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
        if (Mode::game != mode_) {
            return;
        }
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
        mode_ = Mode::game;
        update();
    }

    void Game::resetBoard() {
        for (auto& i: board_) {
            for (auto& j: i) {
                j.color = Color::none;
                j.type = Type::none;
            }
        }
        Piece::reset();
        player_.clear();
        player_.emplace_back(Color::black, board_);
        player_.emplace_back(Color::white, board_);
    }

    void Game::setDefaultBoard() {
        resetBoard();
        for (auto& i: board_) {
            i[1] = {Color::white, Type::pawn};
            i[yBoard - 2] = {Color::black, Type::pawn};
        }
        int queenX = xBoard / 2;
        board_[0][0] = {Color::white, Type::rook};
        board_[1][0] = {Color::white, Type::knight};
        board_[2][0] = {Color::white, Type::bishop};
        board_[queenX - 1][0] = {Color::white, Type::king};
        board_[queenX][0] = {Color::white, Type::queen};
        board_[xBoard - 3][0] = {Color::white, Type::bishop};
        board_[xBoard - 2][0] = {Color::white, Type::knight};
        board_[xBoard - 1][0] = {Color::white, Type::rook};
        board_[0][yBoard - 1] = {Color::black, Type::rook};
        board_[1][yBoard - 1] = {Color::black, Type::knight};
        board_[2][yBoard - 1] = {Color::black, Type::bishop};
        board_[queenX - 1][yBoard - 1] = {Color::black, Type::king};
        board_[queenX][yBoard - 1] = {Color::black, Type::queen};
        board_[xBoard - 3][yBoard - 1] = {Color::black, Type::bishop};
        board_[xBoard - 2][yBoard - 1] = {Color::black, Type::knight};
        board_[xBoard - 1][yBoard - 1] = {Color::black, Type::rook};
        Piece::reset();
        player_.clear();
        player_.emplace_back(Color::black, board_);
        player_.emplace_back(Color::white, board_);
    }

    Mode Game::getMode() {
        return mode_;
    }

    void Game::addPiece(const Coord& pos, const TypePiece& type) {
        int player = (type.color == Color::black ? 0 : 1);
        player_[player].addPiece(type.type, pos);
        board_[pos.x][pos.y] = type;
    }

    bool Game::isRotation() const {
        return rotation_;
    }

    void Game::setRotation(bool rotation) {
        rotation_ = rotation;
    }

    void Game::setPlayerRound(const Color& playerRound) {
        playerRound_ = playerRound;
    }

    std::array<std::array<TypePiece, xBoard>, yBoard>& Game::getBoard(){return board_;}
}