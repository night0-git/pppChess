#include "../../include/game_logic/ChessGame.hpp"

ChessGame::ChessGame() : _currentTurn(PieceColor::White),
_status(GameStatus::Active) {}

void ChessGame::reset() {
    _board.boardInit();
    _currentTurn = PieceColor::White;
    _status = GameStatus::Active;
    _moveHistory.clear();
    _capturedPiece.clear();
}

const Board& ChessGame::board() const {
    return _board;
}

void ChessGame::addBoardObserver(std::shared_ptr<BoardObserver> observer) {
    _board.addObserver(observer);
}

bool ChessGame::attemptMove(const Move& move) {
    auto srcPtr = _board.getPieceAt(move.src);
    if (srcPtr && srcPtr->color() == _currentTurn) {
        bool moved = _board.movePiece(move);
        if (moved) {
            _currentTurn = !_currentTurn;
        }
        return moved;
    }
    return false;
}