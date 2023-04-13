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
        switch (mode_) {
            case Mode::game:
                if (rotation_ && playerRound_ == Color::none)
                    pos = {7 - pos.x, 7 - pos.y};
                if (playerRound_ != Color::none){
                    Type type = Type::none;
                    if (pos.x == 3 && pos.y == 3)
                        type = Type::queen;
                    else if (pos.x == 4 && pos.y == 3)
                        type = Type::rook;
                    else if (pos.x == 3 && pos.y == 4)
                        type = Type::bishop;
                    else if (pos.x == 4 && pos.y == 4)
                        type = Type::knight;
                    if (type != Type::none) {
                        promotion(type);
                    }
                }
                break;
            case Mode::menu:
                break;
            case Mode::personalisation:
                if (0 <= pos.x && pos.x < 8 && 0 <= pos.y && pos.y < 8) {
                    if (side_)
                        pos = {7 - pos.x, 7 - pos.y};
                    board_[pos.x][pos.y] = selectedPiece_;
                    selectPiece();
                } else if (pos.x == 8 && 1 <= pos.y && pos.y < 7) {
                    selectedPiece_ = {selectedColor_, (Type) (pos.y - 1)};
                    selectedCoord_ = pos;
                    selectPiece();
                } else if (pos.x == 8 && pos.y == 0) {
                    selectedColor_ = selectedColor_ == Color::white ? Color::black : Color::white;
                    selectedPiece_.color = selectedColor_;
                    selectPiece();
                } else if (pos.x == 8 && pos.y == 7) {
                    selectedPiece_ = {Color::none, Type::none};
                    selectedCoord_ = pos;
                    selectPiece();
                } else if (pos.x == 9 && pos.y == 0) {
                    if (!isKingDefined()) {
                        displayMessage("There is too much king!");
                        return;
                    }
                    mode_ = Mode::game;
                    this->clear(); // Clear all items
                    setLayer1();
                    setLayer2(board_);
                    emit loadGame(*this);
                } else if (pos.x == 9 && pos.y == 1) {
                    // Reset board
                    setDefaultBoard(board_);
                    selectPiece();
                } else if (pos.x == 9 && pos.y == 2) {
                    resetBoard(board_);
                    selectPiece();
                } else if (pos.x == 9 && pos.y == 3) {
                    side_ = !side_;
                    selectPiece();
                }
                break;
        }

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

    void Game::updateBoard(screen::BoardBase& board) {
        TypePiece boardGame[8][8];
        std::vector<Coord> movePossible =
                pieceSelected_ != nullptr ? pieceSelected_->getPossibleMoves() : std::vector<Coord>();
        if (rotation_)
            board.viewBoard(playerRound_);
        if (promotionPos_ != Coord{-1, -1})
            board.update(selection_, boardGame, movePossible, playerRound_);
        else
            board.update(selection_, boardGame, movePossible);
    }

    TypePiece (&Game::getBoard() const )[8][8] {
        // make with no warning stack reference local variable
        static TypePiece boardGame[8][8];
//        boardGame = board_;
        return boardGame;
    }

    void Game::displayMessage(const std::string& msg) {
        std::cout << msg << std::endl;
    }

    void Game::promotion(Type type) {
        player_[(int) playerRound_].addPiece(type, promotionPos_);
        playerRound_ = (playerRound_ == Color::white ? Color::black : Color::white);
        update();
    }

    void Game::loadGame(Color color) {
        Piece::reset();
        player_.clear();
        player_.emplace_back(Color::black, board_);
        player_.emplace_back(Color::white, board_);
        playerRound_ = color;
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
            for (auto & boardCase : col) {
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


}