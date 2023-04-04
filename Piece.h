#ifndef PIECE_H
#define PIECE_H

#include <vector>

struct Coord {
    int x, y;

    Coord &operator=(const Coord &b) = default;
};

bool operator==(const Coord &a, const Coord &b) {
    return a.x == b.x && a.y == b.y;
}


enum class Colour {
    white,
    black
};


class Piece {
public:
    Piece(const Coord &pos, Colour color);

    virtual bool isLegalMove(const std::vector<Piece *> &board, Coord pos) = 0;

    virtual bool move(const std::vector<Piece *> &board, const Coord &pos);

    [[nodiscard]] Colour getColor() const { return color_; }

    [[nodiscard]] bool isAlive() const { return isAlive_; }

//    [[nodiscard]] Coord Pos() const;

//    void setPosition(int box);

protected:
    inline static Coord *whiteKingPos = nullptr;
    inline static Coord *blackKingPos = nullptr;
//	bool Check_Move(const std::vector<Piece*>* board, int x, int y, bool eat = true, bool need_eat = false);
//	int Check_Move(int p, const std::vector<Piece*>* board, int x, int y);
    Coord pos_;
    Colour color_;
    bool isAlive_ = true;
};


class Pawn : virtual public Piece {
public:
    Pawn(const Coord &pos, Colour color);

    bool move(const std::vector<Piece *> &board, const Coord &pos) override;

    bool isLegalMove(const std::vector<Piece *> &board, Coord pos) override;

private:
    int first_ = 0;
};

class Knight : virtual public Piece {
public:
    Knight(const Coord &pos, Colour color);

    bool isLegalMove(const std::vector<Piece *> &board, Coord pos) override;

};

class Tower : virtual public Piece {
public:
    Tower(const Coord &pos, Colour color);

    [[nodiscard]] bool getFirst() const;

    bool move(const std::vector<Piece *> &board, const Coord &pos) override;

    bool isLegalMove(const std::vector<Piece *> &board, Coord pos) override;

private:
    bool first_ = true;
};


class Bishop : virtual public Piece {
public:
    Bishop(const Coord &pos, Colour color);

    bool isLegalMove(const std::vector<Piece *> &board, Coord pos) override;
};


class Queen : public Tower, Bishop {
public:
    Queen(const Coord &pos, Colour color);

    bool isLegalMove(const std::vector<Piece *> &board, Coord pos) override;
};

class King : virtual public Piece {
public:
    King(const Coord &pos, Colour color);

    bool move(const std::vector<Piece *> &board, const Coord &pos) override;

    bool isLegalMove(const std::vector<Piece *> &board, Coord pos) override;

    bool isCheck(const std::vector<Piece *> &board);

private:
    bool first_ = true;
};


#endif // PIECE_H
