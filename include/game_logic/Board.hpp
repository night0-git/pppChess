#ifndef _BOARD_H_
#define _BOARD_H_

#include "Piece.hpp"
#include "../core/BoardObserver.hpp"
#include <memory>
#include <array>
#include <optional>
#include <vector>

struct Move {
    sf::Vector2i src;
    sf::Vector2i dest;
    Move(sf::Vector2i s, sf::Vector2i d) : src(s), dest(d) {};
};

struct MoveResult {
    bool success;
    std::unique_ptr<Piece> captured;
    MoveResult(bool s, std::unique_ptr<Piece> c = nullptr) : success(s), captured(std::move(c)) {};
};

enum class MoveType { Move, Capture, Invalid };
enum class SpecialMove { Castle, Promote, EnPassant, None };

struct MoveInfo {
    MoveType type = MoveType::Invalid;
    SpecialMove special = SpecialMove::None;
    MoveInfo(MoveType t, SpecialMove s) : type(t), special(s) {};
};

class Board {
public:
    Board();

    MoveResult movePiece(Move move);
    std::unique_ptr<Piece> promote(sf::Vector2i sqr, PieceType type);

public:
    // Helpers
    static const int SIZE = 8;
    bool isValidMove(PieceColor srcColor, sf::Vector2i dest) const;
    static bool isWithinBoard(sf::Vector2i sqr);
    MoveInfo getMoveInfo(Move move);
    bool isChecked(PieceColor color) const;
    bool isMoveSafe(Move move);
    bool isCheckmate(PieceColor color);
    bool isStalemate(PieceColor color);

    // Getters
    const Piece* getPieceAt(sf::Vector2i sqr) const;
    std::optional<sf::Vector2i> enPassantTarget() const;

    void boardInit();
    void addObserver(std::shared_ptr<BoardObserver> observer);
    
private:
    std::vector<Move> getAllValidMoves(PieceColor color);
    bool hasLegalMoves(PieceColor color);
    bool insufficientMaterial() const;

    // Notifiers
    void pieceMoved(Move move);
    void pieceCaptured(const Piece* piece);
    void selectPromoteType(sf::Vector2i sqr, PieceType& type);
    void piecePromoted(sf::Vector2i sqr, PieceType type, const Piece* oldPiece);

private:
    std::array<std::array<std::unique_ptr<Piece>, SIZE>, SIZE> _grid;
    std::optional<sf::Vector2i> _enPassantTarget;
    std::unique_ptr<Piece> takePieceAt(sf::Vector2i sqr);
    void setupDefaultBoard();
    sf::Vector2i _whiteKingPos = sf::Vector2i(7, 4);
    sf::Vector2i _blackKingPos = sf::Vector2i(0, 4);

private:
    std::vector<std::weak_ptr<BoardObserver>> _observers;
};

#endif