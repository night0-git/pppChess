#include "../../../include/game_logic/Piece.hpp"

Queen::Queen(PieceColor color) : Piece(PieceType::Queen, color) {}

vector<sf::Vector2i> Queen::validMoves(const Board& board, sf::Vector2i sqr) const {
    auto bishop = std::make_unique<Bishop>(_color);
    auto rook = std::make_unique<Rook>(_color);
    vector<sf::Vector2i> validMoves = bishop->validMoves(board, sqr);
    vector<sf::Vector2i> rookMoves = rook->validMoves(board, sqr);

    validMoves.reserve(validMoves.size() + rookMoves.size());
    validMoves.insert(validMoves.end(), rookMoves.begin(), rookMoves.end());
    return validMoves;
}