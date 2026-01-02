#include "../../../include/game_logic/Piece.hpp"
#include "../../../include/game_logic/Board.hpp"

Knight::Knight(PieceColor color) : Piece(PieceType::Knight, color) {}

vector<sf::Vector2i> Knight::validMoves(const Board& board, const sf::Vector2i& sqr) const {
    std::vector<sf::Vector2i> validMoves;
    validMoves.reserve(8);
    std::array<sf::Vector2i, 8> dirs = {{
        {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    }};

    for (const auto& dir : dirs) {
        if (board.isValidMove(_color, sqr + dir)) {
            validMoves.push_back(sqr + dir);
        }
    }
    return validMoves;
}