#include "../../include/game_logic/Piece.hpp"

Piece::Piece(PieceType type, PieceColor color) : _type(type), _color(color) {}

PieceType Piece::type() {
    return _type;
}

PieceColor Piece::color() {
    return _color;
}
