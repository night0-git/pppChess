#include "../../include/game_logic/Board.hpp"
#include <algorithm>
#include <iostream>
Board::Board() {
    setupDefaultBoard();
}

bool Board::movePiece(const sf::Vector2i& src, const sf::Vector2i& dest) {
    std::pair<MoveType, SpecialMove> moveInfo = getMoveInfo(src, dest);
    MoveType type = moveInfo.first;
    SpecialMove special = moveInfo.second;

    if (type == MoveType::Invalid) {
        return false;
    }

    if (special != SpecialMove::EnPassant) {
        _enPassantTarget.reset();
    }

    auto sourcePtr = getPieceAt(src);
    auto sourcePcs = takePieceAt(src);
    auto destPcs = takePieceAt(dest);

    sourcePcs->setMoved();
    _grid[dest.x][dest.y] = std::move(sourcePcs);
    pieceMoved(src, dest);
    
    if (special == SpecialMove::EnPassant) {
        if (type == MoveType::Move) {
            int forward = (sourcePtr->color() == PieceColor::Black ? 1 : -1);
            _enPassantTarget = src + sf::Vector2i{forward, 0};
        }
        else {
            _enPassantTarget.reset();
            
            sf::Vector2i victimPos = {src.x, dest.y};
            auto victimPawn = takePieceAt(victimPos);
            if (victimPawn) {
                pieceCaptured(victimPawn.get());
                _capturedPieces.push_back(std::move(victimPawn));
            }
        }
    }

    else if (special == SpecialMove::Castle) {
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
            pieceMoved(rookSrc, rookDest);
        }
    }

    else if (special == SpecialMove::Promote) {
        // TODO: input promotion type here
        auto oldPawn = promote(dest, PieceType::Queen);
        piecePromoted(dest, PieceType::Queen, oldPawn.get());
    }

    if (type == MoveType::Capture && destPcs) {
        pieceCaptured(destPcs.get());
        _capturedPieces.push_back(std::move(destPcs));
    }

    return true;
}

std::unique_ptr<Piece> Board::promote(const sf::Vector2i& sqr, PieceType type) {
    PieceColor color = getPieceAt(sqr)->color();
    auto oldPawn = takePieceAt(sqr);
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
    return oldPawn;
}

bool Board::isWithinBoard(const sf::Vector2i& sqr) {
    return (sqr.x >= 0 && sqr.x < SIZE && sqr.y >= 0 && sqr.y < SIZE);
}

bool Board::isValidMove(PieceColor srcColor, const sf::Vector2i& dest) const {
    if (!isWithinBoard(dest)) {
        return false;
    }

    auto destPtr = getPieceAt(dest);
    return (!destPtr || srcColor != destPtr->color());
}

std::pair<MoveType, SpecialMove> Board::getMoveInfo(const sf::Vector2i& src, const sf::Vector2i& dest) const {
    auto type = MoveType::Invalid;
    auto special = SpecialMove::None;

    auto sourcePtr = getPieceAt(src);
    auto destPtr = getPieceAt(dest);

    if (!isWithinBoard(src) || !isWithinBoard(dest) || !isValidMove(sourcePtr->color(), dest)) {
        return std::make_pair(type, special);
    }

    if (!sourcePtr) {
        return std::make_pair(type, special);
    }

    std::vector<sf::Vector2i> validMoves = sourcePtr->validMoves(*this, src);
    if (std::find(validMoves.begin(), validMoves.end(), dest) == validMoves.end()) {
        return std::make_pair(type, special);
    }

    type = (destPtr ? MoveType::Capture : MoveType::Move);

    bool isCastling = (sourcePtr->type() == PieceType::King && std::abs(src.y - dest.y) == 2);
    if (isCastling) {
        special = SpecialMove::Castle;
        return std::make_pair(type, special);
    }

    bool isEnPassantTarget = (sourcePtr->type() == PieceType::Pawn && std::abs(src.x - dest.x) == 2)
                            || (_enPassantTarget && *_enPassantTarget == dest);
    if (isEnPassantTarget) {
        type = (src.y == dest.y ? MoveType::Move : MoveType::Capture);
        special = SpecialMove::EnPassant;
        return std::make_pair(type, special);
    }

    bool isPromoting = (sourcePtr->type() == PieceType::Pawn
                        && (dest.x == 0 || dest.x == SIZE - 1));
    if (isPromoting) {
        special = SpecialMove::Promote;
        return std::make_pair(type, special);
    }

    special = SpecialMove::None;
    return std::make_pair(type, special);
}


const Piece* Board::getPieceAt(const sf::Vector2i& sqr) const {
    if (!isWithinBoard(sqr)) {
        return nullptr;
    }
    return _grid[sqr.x][sqr.y].get();
}

std::optional<sf::Vector2i> Board::enPassantTarget() const {
    return _enPassantTarget;
}

bool Board::isCheckedSqr(PieceColor color, const sf::Vector2i& sqr) const {
    // Knight
    static const std::vector<sf::Vector2i> knightOffsets = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    };
    for (const auto& offset : knightOffsets) {
        sf::Vector2i pos = sqr + offset;
        if (!isValidMove(color, pos)) continue;
        auto piece = getPieceAt(pos);
        if (piece && piece->type() == PieceType::Knight) {
            return true; // Found an attacking knight
        }
    }

    // Pawn
    int forward = (color == PieceColor::White ? -1 : 1); 
    std::vector<sf::Vector2i> pawnAttacks = {
        {sqr.x + forward, sqr.y + 1},
        {sqr.x + forward, sqr.y - 1}
    };
    for (const auto& pos : pawnAttacks) {
        if (!isValidMove(color, pos)) continue;
        auto piece = getPieceAt(pos);
        if (piece && piece->type() == PieceType::Pawn) {
            return true;
        }
    }

    // Sliding check
    static const std::vector<sf::Vector2i> orthoDirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (const auto& dir : orthoDirs) {
        sf::Vector2i pos = sqr + dir;
        while (isWithinBoard(pos)) {
            auto piece = getPieceAt(pos);
            if (piece) {
                // Found an attacking enemy
                if (piece->color() != color && (piece->type() == PieceType::Rook || piece->type() == PieceType::Queen)) {
                    return true;
                }
                // Sees own king, ignore
                if (piece->type() != PieceType::King) {
                    break;
                }
            }
            pos += dir;
        }
    }

    // Diagonal check
    static const std::vector<sf::Vector2i> diagDirs = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (const auto& dir : diagDirs) {
        sf::Vector2i pos = sqr + dir;
        while (isWithinBoard(pos)) {
            auto piece = getPieceAt(pos);
            if (piece) {
                // Found an attacking enemy
                if (piece->color() != color && (piece->type() == PieceType::Rook || piece->type() == PieceType::Queen)) {
                    return true;
                }
                // Sees own king, ignore
                if (piece->type() != PieceType::King) {
                    break;
                }
            }
            pos += dir;
        }
    }

    // King
    static const std::vector<sf::Vector2i> kingOffsets = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    for (const auto& offset : kingOffsets) {
        sf::Vector2i pos = sqr + offset;
        if (!isValidMove(color, pos)) continue;
        auto piece = getPieceAt(pos);
        if (piece && piece->type() == PieceType::King) {
            return true;
        }
    }

    return false;
}

void Board::boardInit() {
    for (auto it = _observers.begin(); it != _observers.end(); ) {
        if (auto observer = it->lock()) {
            observer->onBoardInit();
            it++;
        }
        else {
            it = _observers.erase(it);
        }
    }
}

void Board::pieceMoved(const sf::Vector2i& src, const sf::Vector2i& dest) {
    for (auto it = _observers.begin(); it != _observers.end(); ) {
        if (auto observer = it->lock()) {
            observer->onPieceMoved(src, dest);
            it++;
        }
        else {
            it = _observers.erase(it);
        }
    }
}

void Board::pieceCaptured(const Piece* piece) {
    for (auto it = _observers.begin(); it != _observers.end(); ) {
        if (auto observer = it->lock()) {
            observer->onPieceCaptured(piece);
            it++;
        }
        else {
            it = _observers.erase(it);
        }
    }
}

void Board::piecePromoted(const sf::Vector2i& sqr, PieceType type, const Piece* oldPiece) {
    for (auto it = _observers.begin(); it != _observers.end(); ) {
        if (auto observer = it->lock()) {
            observer->onPromotion(sqr, type, oldPiece);
            it++;
        }
        else {
            it = _observers.erase(it);
        }
    }
}

void Board::addObserver(std::shared_ptr<BoardObserver> observer) {
    _observers.push_back(observer);
}

std::unique_ptr<Piece> Board::takePieceAt(const sf::Vector2i& sqr) {
    if (!getPieceAt(sqr)) {
        return nullptr;
    }
    auto pcs = std::move(_grid[sqr.x][sqr.y]);
    _grid[sqr.x][sqr.y] = nullptr;
    return pcs;
}

void Board::setupDefaultBoard() {
    // Place pawns
    for (int col = 0; col < SIZE; ++col) {
        _grid[1][col] = std::make_unique<Pawn>(PieceColor::Black);
        _grid[SIZE - 2][col] = std::make_unique<Pawn>(PieceColor::White);
    }

    // Black
    _grid[0][0] = std::make_unique<Rook>(PieceColor::Black);
    _grid[0][1] = std::make_unique<Knight>(PieceColor::Black);
    _grid[0][2] = std::make_unique<Bishop>(PieceColor::Black);
    _grid[0][3] = std::make_unique<Queen>(PieceColor::Black);
    _grid[0][4] = std::make_unique<King>(PieceColor::Black);
    _grid[0][5] = std::make_unique<Bishop>(PieceColor::Black);
    _grid[0][6] = std::make_unique<Knight>(PieceColor::Black);
    _grid[0][7] = std::make_unique<Rook>(PieceColor::Black);
    
    // White
    _grid[SIZE - 1][0] = std::make_unique<Rook>(PieceColor::White);
    _grid[SIZE - 1][1] = std::make_unique<Knight>(PieceColor::White);
    _grid[SIZE - 1][2] = std::make_unique<Bishop>(PieceColor::White);
    _grid[SIZE - 1][3] = std::make_unique<Queen>(PieceColor::White);
    _grid[SIZE - 1][4] = std::make_unique<King>(PieceColor::White);
    _grid[SIZE - 1][5] = std::make_unique<Bishop>(PieceColor::White);
    _grid[SIZE - 1][6] = std::make_unique<Knight>(PieceColor::White);
    _grid[SIZE - 1][7] = std::make_unique<Rook>(PieceColor::White);
}