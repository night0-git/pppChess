#ifndef _BOARD_H_
#define _BOARD_H_

#include "Piece.hpp"
#include "../core/BoardObserver.hpp"
#include <memory>
#include <array>
#include <optional>
#include <vector>
#include <utility>
using std::array, std::unique_ptr;

enum class MoveType { Move, Capture, Invalid };
enum class SpecialMove { Castle, Promote, EnPassant, None };

class Board {
public:
    Board();

    bool movePiece(const sf::Vector2i& src, const sf::Vector2i& dest);
    std::unique_ptr<Piece> promote(const sf::Vector2i& sqr, PieceType type);

public:
    // Helpers
    static const int SIZE = 8;
    bool isValidMove(PieceColor srcColor, const sf::Vector2i& dest) const;
    static bool isWithinBoard(const sf::Vector2i& sqr);
    std::pair<MoveType, SpecialMove> getMoveInfo(const sf::Vector2i& src, const sf::Vector2i& dest) const;

    // Getters
    const Piece* getPieceAt(const sf::Vector2i& sqr) const;
    std::optional<sf::Vector2i> enPassantTarget() const;
    bool isAttackedSqr(PieceColor color, const sf::Vector2i& sqr) const;

    // Notifiers
    void boardInit();
    void pieceMoved(const sf::Vector2i& src, const sf::Vector2i& dest);
    void pieceCaptured(const Piece* piece);
    void piecePromoted(const sf::Vector2i& sqr, PieceType type, const Piece* oldPiece);
    void addObserver(std::shared_ptr<BoardObserver> observer);

private:
    array<array<unique_ptr<Piece>, SIZE>, SIZE> _grid;
    std::vector<unique_ptr<Piece>> _capturedPieces;
    std::optional<sf::Vector2i> _enPassantTarget;
    std::unique_ptr<Piece> takePieceAt(const sf::Vector2i& sqr);
    void setupDefaultBoard();

private:
    std::vector<std::weak_ptr<BoardObserver>> _observers;
};

#endif