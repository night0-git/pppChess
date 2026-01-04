#include "../../../include/game_logic/Piece.hpp"
#include "../../../include/game_logic/Board.hpp"
#include <algorithm>

King::King(PieceColor color) : Piece(PieceType::King, color) {}

vector<sf::Vector2i> King::validMoves(const Board& board, const sf::Vector2i& sqr) const {
    std::vector<sf::Vector2i> validMoves;
    validMoves.reserve(8);

    static std::array<sf::Vector2i, 8> offsets = {{
        {1,  1}, {1,  0}, {1, -1}, {0, -1}, {-1, -1}, {-1,  0}, {-1,  1}, {0,  1}
    }};

    for (const auto& offset : offsets) {
        sf::Vector2i move = sqr + offset;
        if (board.isValidMove(_color, move) && !board.isAttackedSqr(_color, move, {sqr, move})) {
            validMoves.push_back(move);
        }
    }

    // Castling moves
    if (!_hasMoved && !board.isAttackedSqr(_color, sqr)) {
        // Queen-side castling
        sf::Vector2i queenSideRookPos(sqr.x, 0);
        const Piece* rookPtr = board.getPieceAt(queenSideRookPos);
        if (rookPtr && rookPtr->type() == PieceType::Rook && rookPtr->color() == _color && !rookPtr->hasMoved()) {
            bool pathClear = true;
            for (int y = sqr.y - 1; y > 0; y--) {
                sf::Vector2i currentSqr(sqr.x, y);
                if (board.getPieceAt(currentSqr) || (y > sqr.y - 3 && board.isAttackedSqr(_color, currentSqr, {sqr, currentSqr}))) {
                    pathClear = false;
                    break;
                }
            }
            if (pathClear) {
                validMoves.push_back({sqr.x, sqr.y - 2});
            }
        }

        // King-side castling
        sf::Vector2i kingSideRookPos(sqr.x, Board::SIZE - 1);
        rookPtr = board.getPieceAt(kingSideRookPos);
        if (rookPtr && rookPtr->type() == PieceType::Rook && rookPtr->color() == _color && !rookPtr->hasMoved()) {
            bool pathClear = true;
            for (int y = sqr.y + 1; y < Board::SIZE - 1; y++) {
                sf::Vector2i currentSqr(sqr.x, y);
                if (board.getPieceAt(currentSqr) || board.isAttackedSqr(_color, currentSqr, {sqr, currentSqr})) {
                    pathClear = false;
                    break;
                }
            }
            if (pathClear) {
                validMoves.push_back({sqr.x, sqr.y + 2});
            }
        }
    }

    return validMoves;
}
