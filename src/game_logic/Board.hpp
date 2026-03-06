#ifndef _BOARD_H_
#define _BOARD_H_

#include "Piece.hpp"
#include "../core/BoardObserver.hpp"
#include <memory>
#include <array>
#include <optional>
#include <vector>
#include <utility>
#include <string>

struct Move {
    sf::Vector2i src;
    sf::Vector2i dest;
    Move(sf::Vector2i s, sf::Vector2i d) : src(s), dest(d) {};
    bool operator==(const Move& other) const {
        return src == other.src && dest == other.dest;
    }
};

enum class MoveType { Move, Capture, Invalid };
enum class SpecialMove { Castle, Promote, EnPassant, Illegal, None };

struct MoveResult {
    bool success;
    Move move;
    std::unique_ptr<Piece> captured = nullptr;
    SpecialMove special;
    std::unique_ptr<Piece> promotedPawn = nullptr;
    std::optional<Move> rookMove = std::nullopt;
    bool isCheck = false;
    MoveResult(bool s, Move m, SpecialMove sp) : success(s), move(m), special(sp) {};

    // Since this struct contains unique_ptr, we need to define move semantics
    MoveResult(MoveResult&& other) noexcept = default;
    MoveResult& operator=(MoveResult&& other) noexcept = default;

    // And delete copy semantics
    MoveResult(const MoveResult&) = delete;
    MoveResult& operator=(const MoveResult&) = delete;
};

class Board {
public:
    Board();
    Board(const Board& other);

    MoveResult movePiece(Move move);
    std::unique_ptr<Piece> promote(sf::Vector2i sqr, PieceType type);
    std::optional<std::string> getMoveNotation(Move move, std::optional<PieceType> promoteType = std::nullopt);

public:
    // Helpers
    static const int SIZE = 8;
    bool isValidMove(PieceColor srcColor, sf::Vector2i dest) const;
    static bool isWithinBoard(sf::Vector2i sqr);
    bool isChecked(PieceColor color) const;
    bool isMoveSafe(Move move);
    bool hasLegalMoves(PieceColor color);
    bool insufficientMaterial() const;
    std::vector<Move> getAllValidMoves(PieceColor color);
    std::vector<Move> getAllValidMoves();
    bool applyMoveInternal(Move move, std::optional<PieceType> promoteType = std::nullopt);
    bool undoLastMoveInternal();

    // Getters
    const Piece* getPieceAt(sf::Vector2i sqr) const;
    std::optional<sf::Vector2i> enPassantTarget() const;
    sf::Vector2i whiteKingPos() const;
    sf::Vector2i blackKingPos() const;

    void boardInit();
    void addObserver(std::shared_ptr<BoardObserver> observer);
    
private:
    std::pair<MoveType, SpecialMove> getMoveInfo(Move move);

    // Notifiers
    void onMoveEvent(const MoveResult& result);
    void onPromoteSelection(sf::Vector2i sqr, PieceType& type);

private:
    std::array<std::array<std::unique_ptr<Piece>, SIZE>, SIZE> _grid;
    std::optional<sf::Vector2i> _enPassantTarget;
    std::unique_ptr<Piece> takePieceAt(sf::Vector2i sqr);
    void setupDefaultBoard();
    sf::Vector2i _whiteKingPos = sf::Vector2i(7, 4);
    sf::Vector2i _blackKingPos = sf::Vector2i(0, 4);

private:
    std::vector<MoveResult> _internalMoves;

private:
    std::vector<std::weak_ptr<BoardObserver>> _observers;
};

#endif