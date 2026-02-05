#include "../Piece.hpp"
#include "../Board.hpp"

Bishop::Bishop(PieceColor color) : Piece(PieceType::Bishop, color) {}

vector<sf::Vector2i> Bishop::validMoves(Board& board, sf::Vector2i sqr) const {
    vector<sf::Vector2i> validMoves;
    validMoves.reserve(Board::SIZE * 2);

    std::array<sf::Vector2i, 4> dirs = {{
        {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
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