#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <ostream>
#include <memory>
#include "struct.h"

namespace chess {

    class Piece;

    struct TypePiece {
        Color color = Color::none;
        Type type = Type::none;
        Piece *piece = nullptr;
        int first = 0;

        friend std::ostream& operator<<(std::ostream& os, const TypePiece& piece) {
            switch (piece.type) {
                case Type::pawn:
                    os << (piece.color == Color::white ? "p" : "P");
                    break;
                case Type::rook:
                    os << (piece.color == Color::white ? "t" : "T");
                    break;
                case Type::knight:
                    os << (piece.color == Color::white ? "c" : "C");
                    break;
                case Type::bishop:
                    os << (piece.color == Color::white ? "f" : "F");
                    break;
                case Type::queen:
                    os << (piece.color == Color::white ? "q" : "Q");
                    break;
                case Type::king:
                    os << (piece.color == Color::white ? "k" : "K");
                    break;
                case Type::none:
                    os << " ";
                    break;
            }
            return os;
        }
    };


    class Piece {
    public:
        Piece(const Coord& pos, Color color);

        static void reset();

        virtual bool move(const TypePiece board[8][8], const Coord& pos);

        virtual void update(const TypePiece board[8][8]);

        virtual bool isLegalMove(const TypePiece board[8][8], Coord pos) = 0;

        [[nodiscard]] virtual TypePiece getType() = 0;

        [[nodiscard]] Color getColor() const { return color_; }

        [[nodiscard]] bool isAlive() const { return isAlive_; }

        void kill() { isAlive_ = false; }

        [[nodiscard]] size_t nbMove() const { return possibleMoves_.size(); }

        [[nodiscard]] Coord getPos() const { return pos_; }

        [[nodiscard]] std::vector<Coord> getPossibleMoves() const { return possibleMoves_; }

    protected:
        inline static Piece *whiteKing = nullptr;
        inline static Piece *blackKing = nullptr;
        std::vector<Coord> legalMoves_;
        std::vector<Coord> possibleMoves_;
        Coord pos_;
        Color color_;
        bool isAlive_ = true;
    };


    class Pawn : virtual public Piece {
    public:
        Pawn(const Coord& pos, Color color);

        void update(const TypePiece board[8][8]) override;

        bool move(const TypePiece board[8][8], const Coord& pos) override;

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    private:
        int first_;
    };

    class Knight : virtual public Piece {
    public:
        Knight(const Coord& pos, Color color);

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    };

    class Rook : virtual public Piece {
    public:
        Rook(const Coord& pos, Color color);

        [[nodiscard]] bool getFirst() const;

        bool move(const TypePiece board[8][8], const Coord& pos) override;

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    private:
        bool first_ = true;
    };


    class Bishop : virtual public Piece {
    public:
        Bishop(const Coord& pos, Color color);

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;
    };


    class Queen : public Rook, Bishop {
    public:
        Queen(const Coord& pos, Color color);

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;
    };

    class King : virtual public Piece {
    public:
        King(const Coord& pos, Color color);

        void update(const TypePiece board[8][8]) override;

        bool move(const TypePiece board[8][8], const Coord& pos) override;

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        bool isCheck(const TypePiece board[8][8], Coord pos = Coord{-1, -1});

        [[nodiscard]] TypePiece getType() override;

    private:
        bool first_ = true;
    };

}


#endif // PIECE_H
