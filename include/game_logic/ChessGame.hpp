#ifndef _CHESS_GAME_H_
#define _CHESS_GAME_H_

#include "Board.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

enum class GameStatus { Active, WhiteWon, BlackWon, Draw };

class ChessGame {
public:
    ChessGame();

    void reset();

    // Wrappers
    const Board& board() const;
    void addBoardObserver(std::shared_ptr<BoardObserver> observer);
    bool attemptMove(const Move& move);

    

private:
    std::optional<sf::Vector2i> selectedSqr;
    Board _board;
    GameStatus _status;
    PieceColor _currentTurn;

    std::vector<Move> _moveHistory;
    std::vector<std::unique_ptr<Piece>> _capturedPiece;
};

#endif