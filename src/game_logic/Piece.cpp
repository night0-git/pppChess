#include "../../include/game_logic/Piece.hpp"

Piece::Piece(PieceType type, PieceColor color)
: _type(type), _color(color), _hasMoved(false) {}

PieceType Piece::type() const {
    return _type;
}

PieceColor Piece::color() const {
    return _color;
}

bool Piece::hasMoved() const {
    return _hasMoved;
}

void Piece::setMoved() {
    _hasMoved = true;
}