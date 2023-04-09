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

    Game::Game() : playerRound_(Colour::white), pieceSelected_(nullptr), selection_{{-1, -1},
                                                                                    {-1, -1},
                                                                                    {-1, -1},
                                                                                    {-1, -1}} {
        player_.emplace_back(Colour::black);
        player_.emplace_back(Colour::white);
        update();
    }

    Game::Game(const screen::TypePiece board[8][8], Colour color) : playerRound_(color), pieceSelected_(nullptr),
                                                                    selection_{{-1, -1},
                                                                               {-1, -1},
                                                                               {-1, -1},
                                                                               {-1, -1}} {
        TypePiece boardC[8][8];
        convertBoard(board, boardC);
        player_.emplace_back(Colour::black, boardC);
        player_.emplace_back(Colour::white, boardC);
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
                playerRound_ = (playerRound_ == Colour::white ? Colour::black : Colour::white);
                update();
            }
            pieceSelected_ = nullptr;
            selection_[0] = {-1, -1};
        }
    }


    void Game::update() {
        for (auto &i: board_) {
            for (auto &j: i) {
                j = {Colour::none, Type::none, nullptr};
            }
        }
        player_[0].updateBoard(board_);
        player_[1].updateBoard(board_);
        player_[(int) playerRound_].update(board_);
        Coord pos = player_[(int) playerRound_].getKingPos();
        selection_[3] = {-1, -1};
        switch (player_[(int) playerRound_].getState()) {
            case State::checkmate:
                std::cout << "Echec et mat" << std::endl;
                selection_[3] = pos;
                break;
            case State::stalemate:
                std::cout << "Pat" << std::endl;
                break;
            case State::check:
                std::cout << "Echec" << std::endl;
                selection_[3] = pos;
                break;
            default:
                break;
        }
    }

    void Game::updateBoard(screen::BoardBase& board) {
        screen::TypePiece boardGame[8][8];
        convertBoard(board_, boardGame);
        std::vector<Coord> movePossible =
                pieceSelected_ != nullptr ? pieceSelected_->getPossibleMoves() : std::vector<Coord>();
        board.update(selection_, boardGame, movePossible);
    }

    screen::TypePiece(& Game::getBoard() const) [8][8]{
        // make with no warning stack reference local variable
        static screen::TypePiece boardGame[8][8];
        convertBoard(board_, boardGame);
        return boardGame;
    }


}