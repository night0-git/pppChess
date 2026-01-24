#include "../../include/game_logic/Board.hpp"
#include <algorithm>
#include <sstream>

Board::Board() {
    setupDefaultBoard();
}

MoveResult Board::movePiece(Move move) {
    auto [type, special] = getMoveInfo(move);
    MoveResult moveResult(true, move, special);

    if (type == MoveType::Invalid) {
        moveResult.success = false;
        // Notify when move is illegal
        if (special == SpecialMove::Illegal) {
            onMoveEvent(moveResult);
        }
        return moveResult;
    }

    if (special != SpecialMove::EnPassant) {
        _enPassantTarget.reset();
    }

    auto src = move.src;
    auto dest = move.dest;

    auto sourcePtr = getPieceAt(src);
    auto sourcePcs = takePieceAt(src);
    auto destPcs = takePieceAt(dest);

    // Update king position
    if (sourcePcs->type() == PieceType::King) {
        if (sourcePcs->color() == PieceColor::White) {
            _whiteKingPos = dest;
        } else {
            _blackKingPos = dest;
        }
    }
    sourcePcs->setMoved();
    _grid[dest.x][dest.y] = std::move(sourcePcs);
    
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
                moveResult.captured = std::move(victimPawn);
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
            moveResult.rookMove = Move(rookSrc, rookDest);
        }
    }

    else if (special == SpecialMove::Promote) {
        PieceType promoteType;
        onPromoteSelection(dest, promoteType);
        moveResult.promotedPawn = promote(dest, promoteType);
    }

    if (type == MoveType::Capture && destPcs) {
        moveResult.captured = std::move(destPcs);
    }

    if (isChecked(!sourcePtr->color())) {
        moveResult.isCheck = true;
    }

    // Notify observers
    onMoveEvent(moveResult);

    return moveResult;
}

std::unique_ptr<Piece> Board::promote(sf::Vector2i sqr, PieceType type) {
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

bool Board::isWithinBoard(sf::Vector2i sqr) {
    return (sqr.x >= 0 && sqr.x < SIZE && sqr.y >= 0 && sqr.y < SIZE);
}

bool Board::isValidMove(PieceColor srcColor, sf::Vector2i dest) const {
    if (!isWithinBoard(dest)) {
        return false;
    }

    auto destPtr = getPieceAt(dest);
    return (!destPtr || srcColor != destPtr->color());
}

std::pair<MoveType, SpecialMove> Board::getMoveInfo(Move move) {
    auto type = MoveType::Invalid;
    auto special = SpecialMove::None;
    auto src = move.src;
    auto dest = move.dest;

    auto sourcePtr = getPieceAt(src);
    auto destPtr = getPieceAt(dest);

    if (!isWithinBoard(src) || !isWithinBoard(dest) || !isValidMove(sourcePtr->color(), dest)) {
        return {type, special};
    }

    if (!sourcePtr) {
        return {type, special};
    }

    if (!isMoveSafe(move)) {
        special = SpecialMove::Illegal;
        return {type, special};
    }

    std::vector<sf::Vector2i> validMoves = sourcePtr->validMoves(*this, src);
    if (std::find(validMoves.begin(), validMoves.end(), dest) == validMoves.end()) {
        return {type, special};
    }

    type = (destPtr ? MoveType::Capture : MoveType::Move);

    bool isCastling = (sourcePtr->type() == PieceType::King && std::abs(src.y - dest.y) == 2);
    if (isCastling) {
        special = SpecialMove::Castle;
        return {type, special};
    }

    bool isEnPassantTarget = (sourcePtr->type() == PieceType::Pawn && std::abs(src.x - dest.x) == 2)
                            || (_enPassantTarget && *_enPassantTarget == dest);
    if (isEnPassantTarget) {
        type = (src.y == dest.y ? MoveType::Move : MoveType::Capture);
        special = SpecialMove::EnPassant;
        return {type, special};
    }

    bool isPromoting = (sourcePtr->type() == PieceType::Pawn
                        && (dest.x == 0 || dest.x == SIZE - 1));
    if (isPromoting) {
        special = SpecialMove::Promote;
        return {type, special};
    }

    special = SpecialMove::None;
    return {type, special};
}


const Piece* Board::getPieceAt(sf::Vector2i sqr) const {
    if (!isWithinBoard(sqr)) {
        return nullptr;
    }
    return _grid[sqr.x][sqr.y].get();
}

std::optional<sf::Vector2i> Board::enPassantTarget() const {
    return _enPassantTarget;
}

bool Board::isChecked(PieceColor color) const {
    sf::Vector2i kingSqr = color == PieceColor::White ? _whiteKingPos : _blackKingPos;
    // Knight
    static const std::vector<sf::Vector2i> knightOffsets = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    };
    for (const auto& offset : knightOffsets) {
        sf::Vector2i pos = kingSqr + offset;
        if (!isValidMove(color, pos)) continue;
        auto piece = getPieceAt(pos);
        if (piece && piece->type() == PieceType::Knight) {
            return true; // Found an attacking knight
        }
    }

    // Pawn
    int forward = (color == PieceColor::White ? -1 : 1); 
    std::vector<sf::Vector2i> pawnAttacks = {
        {kingSqr.x + forward, kingSqr.y + 1},
        {kingSqr.x + forward, kingSqr.y - 1}
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
        sf::Vector2i pos = kingSqr + dir;
        while (isValidMove(color, pos)) {
            auto piece = getPieceAt(pos);
            if (piece) {
                // Found an attacking enemy
                if (piece->color() != color && (piece->type() == PieceType::Rook || piece->type() == PieceType::Queen)) {
                    return true;
                }
                break;
            }
            pos += dir;
        }
    }

    // Diagonal check
    static const std::vector<sf::Vector2i> diagDirs = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (const auto& dir : diagDirs) {
        sf::Vector2i pos = kingSqr + dir;
        while (isValidMove(color, pos)) {
            auto piece = getPieceAt(pos);
            if (piece) {
                // Found an attacking enemy
                if (piece->color() != color && (piece->type() == PieceType::Bishop || piece->type() == PieceType::Queen)) {
                    return true;
                }
                break;
            }
            pos += dir;
        }
    }

    // King
    static const std::vector<sf::Vector2i> kingOffsets = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    for (const auto& offset : kingOffsets) {
        sf::Vector2i pos = kingSqr + offset;
        if (!isValidMove(color, pos)) continue;
        auto piece = getPieceAt(pos);
        if (piece && piece->type() == PieceType::King) {
            return true;
        }
    }

    return false;
}

bool Board::isMoveSafe(Move move) {
    auto srcPtr = getPieceAt(move.src);
    if (!srcPtr || !isValidMove(srcPtr->color(), move.dest)) {
        return false;
    }

    auto destPcs = takePieceAt(move.dest);
    sf::Vector2i oldKingPos = (srcPtr->color() == PieceColor::White) ? _whiteKingPos : _blackKingPos;
    
    if (srcPtr->type() == PieceType::King) {
        if (srcPtr->color() == PieceColor::White) _whiteKingPos = move.dest;
        else _blackKingPos = move.dest;
    }
    _grid[move.dest.x][move.dest.y] = std::move(_grid[move.src.x][move.src.y]);
    
    bool safe = !isChecked(srcPtr->color());

    // Revert move
    _grid[move.src.x][move.src.y] = std::move(_grid[move.dest.x][move.dest.y]);
    _grid[move.dest.x][move.dest.y] = std::move(destPcs);
    // Restore king pos
    if (srcPtr->color() == PieceColor::White) _whiteKingPos = oldKingPos;
    else _blackKingPos = oldKingPos;

    return safe;
}

bool Board::hasLegalMoves(PieceColor color) {
    auto allMoves = getAllValidMoves(color);
    for (const auto& move : allMoves) {
        if (isMoveSafe(move)) {
            return true; 
        }
    }
    return false;
}

bool Board::insufficientMaterial() const {
    short whitePieces = 0;
    short blackPieces = 0;
    short whiteKnights = 0;
    short whiteBishops = 0;
    short blackKnights = 0;
    short blackBishops = 0;

    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            auto pcs = getPieceAt({x, y});
            if (!pcs) continue;

            if (pcs->type() == PieceType::Pawn || 
                pcs->type() == PieceType::Rook || 
                pcs->type() == PieceType::Queen) {
                return false; // Major pieces or Pawns always allow checkmate
            }

            if (pcs->color() == PieceColor::White) {
                whitePieces++;
                if (pcs->type() == PieceType::Bishop)
                    whiteBishops++;
                else if (pcs->type() == PieceType::Knight)
                    whiteKnights++;
            } else {
                blackPieces++;
                if (pcs->type() == PieceType::Bishop)
                    blackBishops++;
                else if (pcs->type() == PieceType::Knight)
                    blackKnights++;
            }
        }
    }

    // King vs King
    if (whitePieces == 1 && blackPieces == 1) return true;

    // King vs King + 1 minor
    if ((whitePieces == 1 && blackPieces == 2) || 
        (whitePieces == 2 && blackPieces == 1)) {
        return true;
    }

    // King + 1 minor vs King + 1 minor
    if (whitePieces == 2 && blackPieces == 2) {
        return true; 
    }

    // King + 2 knights vs King
    if ((whitePieces == 1 && blackPieces == 3 && blackKnights == 2) ||
        (blackPieces == 1 && whitePieces == 3 && whiteKnights == 2)) {
        return true;
    }

    return false;
}

std::vector<Move> Board::getAllValidMoves(PieceColor color) {
    std::vector<Move> allValidMoves;
    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            auto pcs = getPieceAt({x, y});
            if (pcs && pcs->color() == color) {
                std::vector<sf::Vector2i> dests = pcs->validMoves(*this, {x, y});
                // Convert to Move objects
                for (auto& dest : dests) {
                    allValidMoves.push_back({{x, y}, dest});
                }
            }
        }
    }
    return allValidMoves;
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

void Board::onMoveEvent(const MoveResult& result) {
    for (auto it = _observers.begin(); it != _observers.end(); ) {
        if (auto observer = it->lock()) {
            observer->onMoveEvent(result);
            it++;
        }
        else {
            it = _observers.erase(it);
        }
    }
}

void Board::onPromoteSelection(sf::Vector2i sqr, PieceType& type) {
    for (auto it = _observers.begin(); it != _observers.end(); ) {
        if (auto observer = it->lock()) {
            observer->onPromoteSelection(sqr, type);
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

std::unique_ptr<Piece> Board::takePieceAt(sf::Vector2i sqr) {
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