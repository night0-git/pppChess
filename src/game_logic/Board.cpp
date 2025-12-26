#include "../../include/game_logic/Board.hpp"
#include <algorithm>

Board::Board() {
    
}

std::unique_ptr<Piece> Board::movePiece(const sf::Vector2i& src, const sf::Vector2i& dest) {
    _enPassantTarget.reset();

    if (!isWithinBoard(src) || !isWithinBoard(dest) || !_grid[src.x][src.y]) {
        return nullptr;
    }

    bool isCastling = getPieceAt(src)->type() == PieceType::King && std::abs(dest.y - src.y) == 2;

    auto srcPcs = takePieceAt(src);
    auto destPcs = takePieceAt(dest);

    // Handle en passant and promoting
    bool isPromoting  = false;
    if (srcPcs->type() == PieceType::Pawn) {
        if (std::abs(src.x - dest.x) == 2) {
            int forward = (srcPcs->color() == PieceColor::Black ? 1 : -1);
            _enPassantTarget = src + sf::Vector2i{forward, 0};
        }
        if (dest.x == 0 || dest.x == SIZE - 1) {
            isPromoting = true;
        }
    }

    _grid[dest.x][dest.y] = std::move(srcPcs);
    _grid[dest.x][dest.y]->setMoved();

    // Move the rook if castling
    if (isCastling) {
        sf::Vector2i rookSrc, rookDest;
        if (dest.y > src.y) { // Kingside castling
            rookSrc = {src.x, SIZE - 1};
            rookDest = {src.x, dest.y - 1};
        } else { // Queenside castling
            rookSrc = {src.x, 0};
            rookDest = {src.x, dest.y + 1};
        }
        auto rook = takePieceAt(rookSrc);
        if (rook) {
            rook->setMoved();
            _grid[rookDest.x][rookDest.y] = std::move(rook);
        }
    }
    else if (isPromoting) {
        // TODO: input promotion type here
        promote(dest, PieceType::Queen);
    }

    return destPcs;
}

void Board::promote(const sf::Vector2i& sqr, PieceType type) {
    PieceColor color = getPieceAt(sqr)->color();
    switch(type) {
    case PieceType::Queen:
        _grid[sqr.x][sqr.y] = std::make_unique<Queen>(color);
        break;
    case PieceType::Rook:
        _grid[sqr.x][sqr.y] = std::make_unique<Rook>(color);
        break;
    case PieceType::Knight:
        _grid[sqr.x][sqr.y] = std::make_unique<Knight>(color);
        break;
    case PieceType::Bishop:
        _grid[sqr.x][sqr.y] = std::make_unique<Bishop>(color);
        break;
    default:
        break;
    }
    _grid[sqr.x][sqr.y]->setMoved();
}

bool Board::isWithinBoard(const sf::Vector2i& sqr) {
    return (sqr.x >= 0 && sqr.x < SIZE && sqr.y >= 0 && sqr.y < SIZE);
}

bool Board::isValidMove(PieceColor srcColor, const sf::Vector2i& dest) const {
    if (!isWithinBoard(dest) || srcColor == _grid[dest.x][dest.y]->color()) {
        return false;
    }
    return true;
}

const Piece* Board::getPieceAt(const sf::Vector2i& sqr) const {
    return _grid[sqr.x][sqr.y].get();
}

std::optional<sf::Vector2i> Board::enPassantTarget() const {
    return _enPassantTarget;
}

bool Board::isCheckedSqr(PieceColor color, const sf::Vector2i& sqr) const {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            auto pcs = getPieceAt({i, j});
            if (pcs && pcs->color() != color) {
                auto moves = pcs->validMoves(*this, {i, j});
                bool isCheckedSqr = find(moves.begin(), moves.end(), sqr) != moves.end();
                if (isCheckedSqr) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::unique_ptr<Piece> Board::takePieceAt(const sf::Vector2i& sqr) {
    if (!isWithinBoard(sqr) || !getPieceAt(sqr)) {
        return nullptr;
    }
    auto pcs = std::move(_grid[sqr.x][sqr.y]);
    _grid[sqr.x][sqr.y] = nullptr;
    return pcs;
}