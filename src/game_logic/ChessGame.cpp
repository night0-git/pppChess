#include "ChessGame.hpp"
#include <iostream>
#include <chrono>
#include <thread>

ChessGame::ChessGame(PieceColor localColor)
: _opponent(nullptr), _isLocalMove(localColor == PieceColor::White) {}

ChessGame::ChessGame(std::unique_ptr<Player> opponent, PieceColor localColor)
: _opponent(std::move(opponent)), _isLocalMove(localColor == PieceColor::White) {}

Player* ChessGame::opponent() {
    return _opponent.get();
}

void ChessGame::opponentMove() {
    auto start = std::chrono::steady_clock::now();
    auto move = _opponent->getMove(_board, _currentTurn);

    if (move.has_value()) {
        // Ensure the animation has finished
        std::this_thread::sleep_until(start + std::chrono::milliseconds(200));

        if (!attemptMove(move.value())) {
            std::cerr << "Opponent made an invalid move.";
        }
    }
}

void ChessGame::changeLocalColor() {
    _isLocalMove = !_isLocalMove;
}

GameStatus ChessGame::status() const {
    return _status;
}

PieceColor ChessGame::currentTurn() const {
    return _currentTurn;
}

bool ChessGame::isLocalMove() const {
    return _isLocalMove;
}

std::optional<std::type_index> ChessGame::nonLocalOpponent() const {
    if (!_opponent) {
        return std::nullopt;
    }
    
    return std::type_index(typeid(*_opponent));
}

void ChessGame::changeTurn() {
    _isLocalMove = !_isLocalMove;
    _currentTurn = !_currentTurn;
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

void ChessGame::onBoardInit() {}
    
void ChessGame::onMoveEvent(const MoveResult& result) {
    if (result.success) {
        changeTurn();
    }
}

void ChessGame::onPromoteSelection(sf::Vector2i sqr, PieceType& type) {}