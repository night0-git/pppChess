#include "../../../include/game_logic/Piece.hpp"
#include "../../../include/game_logic/Board.hpp"

Pawn::Pawn(PieceColor color) : Piece(PieceType::Pawn, color) {}

vector<sf::Vector2i> Pawn::validMoves(const Board& board, sf::Vector2i sqr) const {
    std::vector<sf::Vector2i> validMoves;
    validMoves.reserve(4);

    int forward = (_color == PieceColor::Black ? 1 : -1);

    std::array<sf::Vector2i, 4> moves = {{
        sqr + sf::Vector2i(forward, 0), 
        sqr + sf::Vector2i(2 * forward, 0), 
        sqr + sf::Vector2i(forward, 1), 
        sqr + sf::Vector2i(forward, -1)
    }};

    if (board.isValidMove(_color, moves[0]) && !board.getPieceAt(moves[0])) {
        validMoves.push_back(moves[0]);
        if (board.isValidMove(_color, moves[1]) && !_hasMoved && !board.getPieceAt(moves[1])) {
            validMoves.push_back(moves[1]);
        }
    }

    if (board.isValidMove(_color, moves[2])) {
        if (board.getPieceAt(moves[2])) {
            validMoves.push_back(moves[2]);
        }
        // En passant
        else {
            if (board.enPassantTarget() && moves[2] == *board.enPassantTarget()) {
                validMoves.push_back(moves[2]);
            }
        }
    }

    if (board.isValidMove(_color, moves[3])) {
        if (board.getPieceAt(moves[3])) {
            validMoves.push_back(moves[3]);
        }
        // En passant
        else {
            if (board.enPassantTarget() && moves[3] == *board.enPassantTarget()) {
                validMoves.push_back(moves[3]);
            }
        }
    }

    return validMoves;
}