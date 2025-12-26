#include "../../../include/game_logic/Piece.hpp"
#include "../../../include/game_logic/Board.hpp"
#include <algorithm>

King::King(PieceColor color) : Piece(PieceType::King, color) {}

vector<sf::Vector2i> King::validMoves(const Board& board, const sf::Vector2i& sqr) const {
    std::vector<sf::Vector2i> validMoves;
    validMoves.reserve(8);

    std::array<sf::Vector2i, 8> dirs = {{
        {1,  1}, {1,  0}, {1, -1}, {0, -1}, {-1, -1}, {-1,  0}, {-1,  1}, {0,  1}
    }};

    for (const auto& dir : dirs) {
        sf::Vector2i move = sqr + dir;
        if (board.isValidMove(_color, move) && !board.isCheckedSqr(_color, sqr)) {
            validMoves.push_back(move);
        }
    }

    // Castling moves
    if (!_hasMoved) {
        if (board.isValidMove(_color, sqr + dirs[1]) && !board.isCheckedSqr(_color, sqr + dirs[1]) &&
        board.isValidMove(_color, sqr + 2 * dirs[1]) && !board.isCheckedSqr(_color, sqr + 2 * dirs[1])) {
            sf::Vector2i move = sqr + 3 * dirs[1];
            bool canCastle = true;
            while (move.x > 0 && move.x < Board::SIZE - 1) {
                if (!board.isValidMove(_color, move)) {
                    canCastle = false;
                    break;
                }
            }
            if (canCastle) {
                validMoves.push_back({sqr + 2 * dirs[1]});
            }
        }

        if (board.isValidMove(_color, sqr + dirs[3]) && !board.isCheckedSqr(_color, sqr + dirs[3]) &&
        board.isValidMove(_color, sqr + 2 * dirs[3]) && !board.isCheckedSqr(_color, sqr + 2 * dirs[3])) {
            sf::Vector2i move = sqr + 3 * dirs[3];
            bool canCastle = true;
            while (move.x > 0 && move.x < Board::SIZE - 1) {
                if (!board.isValidMove(_color, move)) {
                    canCastle = false;
                    break;
                }
            }
            if (canCastle) {
                validMoves.push_back({sqr + 2 * dirs[3]});
            }
        }
    }

    return validMoves;
}
