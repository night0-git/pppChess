#ifndef _CHESS_GAME_H_
#define _CHESS_GAME_H_

#include "Board.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include "../engine/ChessBot.hpp"

enum class GameStatus { Active, WhiteWon, BlackWon, Draw };

class ChessGame {
public:
    ChessGame();

    GameStatus status() const;
    PieceColor currentTurn() const;

    void reset();
    const Piece* getLastCaptured() const;

    // Wrappers
    const Board& board() const;
    void addBoardObserver(std::shared_ptr<BoardObserver> observer);
    bool attemptMove(Move move);

    ChessBot _bot = ChessBot(3);
    PieceColor _botColor = PieceColor::Black;
    void botMove();

private:
    Board _board;
    GameStatus _status;
    PieceColor _currentTurn;

    std::vector<Move> _moveHistory;
    std::vector<std::unique_ptr<Piece>> _capturedPiece;
    bool isRepetition(Move move) const;
};

#endif