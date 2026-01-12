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

const Piece* ChessGame::getLastCaptured() const {
    if (_capturedPiece.empty()) {
        return nullptr;
    }
    return _capturedPiece.back().get();
}

const Board& ChessGame::board() const {
    return _board;
}

void ChessGame::addBoardObserver(std::shared_ptr<BoardObserver> observer) {
    _board.addObserver(observer);
}

bool ChessGame::attemptMove(Move move) {
    auto srcPtr = _board.getPieceAt(move.src);
    if (srcPtr && srcPtr->color() == _currentTurn) {
        MoveResult res = _board.movePiece(move);
        if (res.success) {
            if (res.captured) {
                _capturedPiece.push_back(std::move(res.captured));
            }
            _currentTurn = !_currentTurn;
            return true;
        }
    }
    return false;
}