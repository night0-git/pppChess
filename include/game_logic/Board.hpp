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

struct Move {
    sf::Vector2i src;
    sf::Vector2i dest;
    Move(const sf::Vector2i& s, const sf::Vector2i& d) : src(s), dest(d) {};
};

struct MoveResult {
    bool success;
    std::unique_ptr<Piece> captured;
    MoveResult(bool s, std::unique_ptr<Piece> c = nullptr) : success(s), captured(std::move(c)) {};
};

enum class MoveType { Move, Capture, Invalid };
enum class SpecialMove { Castle, Promote, EnPassant, None };

class Board {
public:
    Board();

    MoveResult movePiece(const Move& move);
    std::unique_ptr<Piece> promote(const sf::Vector2i& sqr, PieceType type);

public:
    // Helpers
    static const int SIZE = 8;
    bool isValidMove(PieceColor srcColor, const sf::Vector2i& dest) const;
    static bool isWithinBoard(const sf::Vector2i& sqr);
    std::pair<MoveType, SpecialMove> getMoveInfo(const Move& move) const;

    // Getters
    const Piece* getPieceAt(const sf::Vector2i& sqr) const;
    std::optional<sf::Vector2i> enPassantTarget() const;
    bool isAttackedSqr(PieceColor color, const sf::Vector2i& sqr, const Move& incMove = {{-1, -1}, {-1, -1}}) const;

    // Notifiers
    void boardInit();
    void addObserver(std::shared_ptr<BoardObserver> observer);
    
private:
    // Notifiers
    void pieceMoved(const Move& move);
    void pieceCaptured(const Piece* piece);
    void selectPromoteType(const sf::Vector2i& sqr, PieceType& type);
    void piecePromoted(const sf::Vector2i& sqr, PieceType type, const Piece* oldPiece);
    

private:
    array<array<unique_ptr<Piece>, SIZE>, SIZE> _grid;
    std::optional<sf::Vector2i> _enPassantTarget;
    std::unique_ptr<Piece> takePieceAt(const sf::Vector2i& sqr);
    void setupDefaultBoard();

private:
    std::vector<std::weak_ptr<BoardObserver>> _observers;
};

#endif