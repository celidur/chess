//
// Created by Gaetan on 2023-04-05.
//

#include "Game.h"
#include <iostream>

namespace chess {

    void convertBoard(const screen::TypePiece boardGame[8][8], TypePiece board[8][8]) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                board[i][j] = {boardGame[i][j].color, boardGame[i][j].type};
            }
        }
    }

    void convertBoard(const TypePiece boardGame[8][8], screen::TypePiece board[8][8]) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                board[i][j] = {boardGame[i][j].color, boardGame[i][j].type};
            }
        }
    }

    Game::Game() : playerRound_(Color::white), pieceSelected_(nullptr), selection_{{-1, -1},
                                                                                   {-1, -1},
                                                                                   {-1, -1},
                                                                                   {-1, -1}} {
        player_.emplace_back(Color::black);
        player_.emplace_back(Color::white);
        update();
    }

    Game::Game(const screen::TypePiece board[8][8], Color color) : playerRound_(color), pieceSelected_(nullptr),
                                                                   selection_{{-1, -1},
                                                                               {-1, -1},
                                                                               {-1, -1},
                                                                               {-1, -1}} {
        TypePiece boardC[8][8];
        convertBoard(board, boardC);
        player_.emplace_back(Color::black, boardC);
        player_.emplace_back(Color::white, boardC);
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
            pieceSelected_ = board_[pos.x][pos.y].piece;
            selection_[0] = pos;
        } else if (pieceSelected_ != nullptr) {
            Coord previous = pieceSelected_->getPos();
            if (pieceSelected_->move(board_, pos)) {
                selection_[1] = previous;
                selection_[2] = pos;
                playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
                update();
            }
            pieceSelected_ = nullptr;
            selection_[0] = {-1, -1};
        }
    }


    void Game::update() {
        for (auto &i: board_) {
            for (auto &j: i) {
                j = {Color::none, Type::none, nullptr};
            }
        }
        Coord promotionPos;
        promotionPos_ = {-1, -1};
        promotionPos = player_[0].updateBoard(board_);
        if (promotionPos != Coord{-1, -1}) {
            promotionPos_ = promotionPos;
        }
        promotionPos = player_[1].updateBoard(board_);
        if (promotionPos != Coord{-1, -1}) {
            promotionPos_ = promotionPos;
        }
        if (promotionPos != Coord{-1, -1})
            playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
        player_[(int) playerRound_].update(board_);
        Coord pos = player_[(int) playerRound_].getKingPos();
        selection_[3] = {-1, -1};
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

    void Game::updateBoard(screen::Board &board) {
        screen::TypePiece boardGame[8][8];
        convertBoard(board_, boardGame);
        std::vector<Coord> movePossible =
                pieceSelected_ != nullptr ? pieceSelected_->getPossibleMoves() : std::vector<Coord>();
        if (rotation_)
            board.viewBoard(playerRound_);
        if (promotionPos_ != Coord{-1, -1})
            board.update(selection_, boardGame, movePossible, playerRound_);
        else
            board.update(selection_, boardGame, movePossible);
    }

    screen::TypePiece (&Game::getBoard() const )[8][8] {
        // make with no warning stack reference local variable
        static screen::TypePiece boardGame[8][8];
        convertBoard(board_, boardGame);
        return boardGame;
    }

    void Game::displayMessage(std::string msg) {
        std::cout << msg << std::endl;
    }

    void Game::promotion(Type type) {
        player_[(int) playerRound_].addPiece(type, promotionPos_);
        playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
        update();
    }

    void Game::loadGame(const screen::TypePiece board[8][8], Color color) {
        convertBoard(board, board_);
        Piece::reset();
        player_.clear();
        player_.emplace_back(Color::black, board_);
        player_.emplace_back(Color::white, board_);
        playerRound_ = color;
        update();

    }


}