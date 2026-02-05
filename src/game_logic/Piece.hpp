#ifndef _PIECE_H_
#define _PIECE_H_

#include <SFML/Graphics.hpp>
#include <vector>
using std::vector;

class Board;

enum class PieceType { Pawn, Knight, Bishop, Rook, Queen, King };
enum class PieceColor { White, Black };
inline PieceColor operator!(PieceColor color) {
    return (color == PieceColor::White ? PieceColor::Black : PieceColor::White);
}

class Piece {
public:
    Piece(PieceType, PieceColor);
    virtual ~Piece() = default;

    PieceType type() const;
    PieceColor color() const;
    bool hasMoved() const;
    void setMoved();

    virtual vector<sf::Vector2i> validMoves(Board& board, sf::Vector2i sqr) const = 0;

protected:
    PieceType _type;
    PieceColor _color;
    bool _hasMoved;
};


class Rook : public Piece {
public:
    Rook(PieceColor color);

    vector<sf::Vector2i> validMoves(Board& board, sf::Vector2i sqr) const override;
};

class Knight : public Piece {
public:
    Knight(PieceColor color);

    vector<sf::Vector2i> validMoves(Board& board, sf::Vector2i sqr) const override;
};

class Bishop : public Piece {
public:
    Bishop(PieceColor color);
    
    vector<sf::Vector2i> validMoves(Board& board, sf::Vector2i sqr) const override;
};

class King : public Piece {
public:
    King(PieceColor color);

    vector<sf::Vector2i> validMoves(Board& board, sf::Vector2i sqr) const override;
};

class Queen : public Piece {
public:
    Queen(PieceColor color);

    vector<sf::Vector2i> validMoves(Board& board, sf::Vector2i sqr) const override;
};

class Pawn : public Piece {
public:
    Pawn(PieceColor color);

    vector<sf::Vector2i> validMoves(Board& board, sf::Vector2i sqr) const override;
};

#endif