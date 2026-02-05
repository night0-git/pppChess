#include "../Piece.hpp"
#include "../Board.hpp"

Rook::Rook(PieceColor color) : Piece(PieceType::Rook, color) {}

vector<sf::Vector2i> Rook::validMoves(Board& board, sf::Vector2i sqr) const {
    std::vector<sf::Vector2i> validMoves;
    validMoves.reserve(Board::SIZE * 2);

    std::array<sf::Vector2i, 4> dirs = {{
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    }};
    
    for (const auto& dir : dirs) {
        sf::Vector2i curr = sqr;
        bool enemySeen = false;
        while(board.isValidMove(_color, curr + dir) && !enemySeen) {
            validMoves.push_back(curr + dir);
            if (board.getPieceAt(curr + dir)) {
                enemySeen = true;
            }
            curr += dir;
        }
    }
    return validMoves;
}
