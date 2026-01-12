#include "../../../include/game_logic/Piece.hpp"
#include "../../../include/game_logic/Board.hpp"

Knight::Knight(PieceColor color) : Piece(PieceType::Knight, color) {}

vector<sf::Vector2i> Knight::validMoves(const Board& board, sf::Vector2i sqr) const {
    std::vector<sf::Vector2i> validMoves;
    validMoves.reserve(8);
    static std::array<sf::Vector2i, 8> offsets = {{
        {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    }};

    for (const auto& offset : offsets) {
        if (board.isValidMove(_color, sqr + offset)) {
            validMoves.push_back(sqr + offset);
        }
    }
    return validMoves;
}