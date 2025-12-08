#ifndef _PIECE_H_
#define _PIECE_H_

enum class PieceType { Rook, Knight, Bishop, King, Queen, Pawn };
enum class PieceColor { White, Black };

class Piece {
public:
    Piece(PieceType, PieceColor);
    virtual ~Piece() = default;
    PieceType type();
    PieceColor color();

protected:
    PieceType _type;
    PieceColor _color;
};


class Rook : public Piece {
public:
    Rook(PieceColor color);
};

class Knight : public Piece {
public:
    Knight(PieceColor color);
};

class Bishop : public Piece {
public:
    Bishop(PieceColor color);
};

class King : public Piece {
public:
    King(PieceColor color);
};

class Queen : public Piece {
public:
    Queen(PieceColor color);
};

class Pawn : public Piece {
public:
    Pawn(PieceColor color);
};

#endif