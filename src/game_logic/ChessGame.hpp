#ifndef CHESS_GAME_HPP
#define CHESS_GAME_HPP

#include "Board.hpp"
#include "../player/Player.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

enum class GameStatus { Active, WhiteWon, BlackWon, Draw };

class ChessGame : public BoardObserver, public std::enable_shared_from_this<ChessGame> {
public:
    ChessGame(std::unique_ptr<Player> opponent, PieceColor localColor);

    void opponentMove();

    GameStatus status() const;
    PieceColor currentTurn() const;
    bool isLocalMove() const;
    void changeTurn();

    void reset();
    const Piece* getLastCaptured() const;

    // Wrappers
    const Board& board() const;
    void addBoardObserver(std::shared_ptr<BoardObserver> observer);
    bool attemptMove(Move move);
    
    // Observer methods
    void onBoardInit() override;
    void onMoveEvent(const MoveResult& result) override;
    void onPromoteSelection(sf::Vector2i sqr, PieceType& type) override;

private:
    std::unique_ptr<Player> _opponent;
    bool _isLocalMove;

    Board _board;
    GameStatus _status = GameStatus::Active;
    PieceColor _currentTurn = PieceColor::White;

    std::vector<Move> _moveHistory;
    std::vector<std::unique_ptr<Piece>> _capturedPiece;

    bool isRepetition(Move move) const;
};

#endif
