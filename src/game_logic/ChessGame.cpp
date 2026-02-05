#include "ChessGame.hpp"
#include <iostream>

ChessGame::ChessGame() : _currentTurn(PieceColor::White),
_status(GameStatus::Active) {}

GameStatus ChessGame::status() const {
    return _status;
}

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

        auto notation = _board.getMoveNotation(move);

        MoveResult res = _board.movePiece(move);
        if (res.success) {
            if (notation) std::cout << *notation << "\n";

            if (res.captured) {
                _capturedPiece.push_back(std::move(res.captured));
            }

            PieceColor enemyColor = !srcPtr->color();
            if (!_board.hasLegalMoves(enemyColor)) {
                if (_board.isChecked(enemyColor)) {
                    _status = srcPtr->color() == PieceColor::White ? GameStatus::WhiteWon : GameStatus::BlackWon;
                } else {
                    _status = GameStatus::Draw;
                }
            }
            else if (_board.insufficientMaterial()) {
                _status = GameStatus::Draw;
            }
            else if (isRepetition(move)) {
                _status = GameStatus::Draw;
            }

            _moveHistory.push_back(move);
            _currentTurn = !_currentTurn;
            return true;
        }
    }
    return false;
}

bool ChessGame::isRepetition(Move move) const {
    int n = _moveHistory.size();
    if (n < 8) {
        return false;
    }

    if (!(_moveHistory[n - 4] == move && _moveHistory[n - 8] == move)) {
        return false;
    }

    for (int i = 1; i <= 3; i++) {
        if (!(_moveHistory[n - i] == _moveHistory[n - i - 4])) {
            return false;
        }
    }

    return true;
}