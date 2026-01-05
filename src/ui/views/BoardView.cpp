#include "../../../include/ui/views/BoardView.hpp"
#include <iostream>
#include <stdexcept>
ui::BoardView::BoardView(const ResourceManager<TextureId, sf::Texture>& textures, const Board& board)
: _textures(textures), _board(board), _draggedPiece(nullptr) {}

void ui::BoardView::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    sf::Vector2f localPos = getTransform().getInverse().transformPoint(worldPos);
    auto dragOffset = sf::Vector2f(-_tileSize / 2.f, -_tileSize / 2.f);

    if (event.is<sf::Event::MouseButtonPressed>() && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        sf::Vector2i sqr = localPosToSqr(localPos);
        
        if (_board.isWithinBoard(sqr)) {
            auto pcs = _board.getPieceAt(sqr);
            if (pcs) {
                _draggedPiece = pcs;
                _sourceSquare = sqr;
                _pieceViews.at(pcs).setPosition(localPos + dragOffset);
            }
        }
    }

    else if (event.is<sf::Event::MouseMoved>()) {
        if (_draggedPiece) {
            _pieceViews.at(_draggedPiece).setPosition(localPos + dragOffset);
        }
    }

    else if (event.is<sf::Event::MouseButtonReleased>() && event.getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {
        if (_draggedPiece) {
            sf::Vector2i dest = localPosToSqr(localPos);
            bool isValidMove = false;
            if (_onMoveRequest) {
                if (_onMoveRequest({_sourceSquare, dest})) {
                    isValidMove = true;
                }
            }
            if (!isValidMove) {
                // Snap back
               _pieceViews.at(_draggedPiece).updatePosition(_sourceSquare);
            }
            _draggedPiece = nullptr;
        }
    }
}

void ui::BoardView::onBoardInit() {
    _pieceViews.clear();
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 0; y < Board::SIZE; y++) {
            auto pcs = _board.getPieceAt({x, y});
            if (pcs) {
                const sf::Texture& texture = _textures.get(ui::getTextureId(*pcs));
                // Use piecewise_construct for multi-argument constructors in maps
                _pieceViews.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(pcs),
                                    std::forward_as_tuple(texture, _tileSize, *pcs));
                _pieceViews.at(pcs).updatePosition({x, y});
            }
        }
    }
}

void ui::BoardView::onPieceMoved(const Move& move) {
    auto pcs = _board.getPieceAt(move.dest);
    _pieceViews.at(pcs).updatePosition(move.dest);
    // insert sliding animation
}

void ui::BoardView::onPieceCaptured(const Piece* piece) {
    _pieceViews.erase(piece);
}

void ui::BoardView::onPromoteSelection(const sf::Vector2i& sqr, PieceType& type) {
    // Display and choose promotion type here
    type = PieceType::Queen;
}


void ui::BoardView::onPromotion(const sf::Vector2i& sqr, PieceType type, const Piece* oldPcs) {
    // Delete the old piece texture
    _pieceViews.erase(oldPcs);

    // Add new PieceView
    auto pcs = _board.getPieceAt(sqr);
    if (pcs) {
        const sf::Texture& texture = _textures.get(ui::getTextureId(*pcs));
        _pieceViews.insert_or_assign(pcs, PieceView(texture, _tileSize, *pcs));
        auto it = _pieceViews.find(pcs);
        if (it != _pieceViews.end()) {
            it->second.updatePosition(sqr);
        }
    }
}

void ui::BoardView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    // Drawgrid;
    sf::RectangleShape square({_tileSize, _tileSize});
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 0; y < Board::SIZE; y++) {
            square.setPosition({x * _tileSize, y * _tileSize});
            square.setFillColor((x + y) % 2 == 0 ? _lightColor : _darkColor);
            target.draw(square, states);
        }
    }
    // Draw piece
    for (const auto& [piece, view] : _pieceViews) {
        if (piece != _draggedPiece) {
            target.draw(view, states);
        }
    }
    if (_draggedPiece) {
        auto it = _pieceViews.find(_draggedPiece);
        if (it != _pieceViews.end()) {
           target.draw(_pieceViews.at(_draggedPiece), states);
        }
    }
}

sf::Vector2i ui::BoardView::localPosToSqr(const sf::Vector2f& localPos) const {
    int col = static_cast<int>(localPos.x / _tileSize);
    int row = static_cast<int>(localPos.y / _tileSize);
    return sf::Vector2i(row, col);
}