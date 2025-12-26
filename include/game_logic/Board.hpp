#ifndef _BOARD_H_
#define _BOARD_H_

#include "Piece.hpp"
#include <memory>
#include <array>
#include <optional>
using std::array, std::unique_ptr;

class Board {
public:
    Board();

    std::unique_ptr<Piece> movePiece(const sf::Vector2i& src, const sf::Vector2i& dest);

public:
    // Helpers
    static const int SIZE = 8;
    bool isValidMove(PieceColor srcColor, const sf::Vector2i& dest) const;
    static bool isWithinBoard(const sf::Vector2i& sqr);

    // Getters
    const Piece* getPieceAt(const sf::Vector2i& sqr) const;
    std::optional<sf::Vector2i> enPassantTarget() const;
    bool isCheckedSqr(PieceColor color, const sf::Vector2i& sqr) const;

    // Functional
    std::unique_ptr<Piece> takePieceAt(const sf::Vector2i& sqr);

private:
    array<array<unique_ptr<Piece>, SIZE>, SIZE> _grid;
    std::optional<sf::Vector2i> _enPassantTarget;
};

#endif