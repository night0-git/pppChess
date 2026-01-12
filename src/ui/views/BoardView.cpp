#include "../../../include/ui/views/BoardView.hpp"
#include <iostream>
#include <cmath>
using ui::BoardView;

BoardView::BoardView(const ResourceManager<TextureId, sf::Texture>& textures, const Board& board)
: _textures(textures), _board(board) {}

void BoardView::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    sf::Vector2f localPos = getTransform().getInverse().transformPoint(worldPos);
    auto dragOffset = sf::Vector2f(-_tileSize / 2.f, -_tileSize / 2.f);

    if (event.is<sf::Event::MouseButtonPressed>() && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        sf::Vector2i sqr = localPosToSqr(localPos);
        
        if (_board.isWithinBoard(sqr)) {
            // Handle click move first
            if (_selectedSqr) {
                // Check piece selection
                if (*_selectedSqr == sqr && !_isDeselecting) {
                    _isDeselecting = true;
                }
                _isMoving = true;
                if (_onMoveRequest && _onMoveRequest({*_selectedSqr, sqr})) {
                    _selectedSqr = std::nullopt;
                    return;
                }
                _isMoving = false;
            }
            auto pcs = _board.getPieceAt(sqr);
            if (pcs) {
                _selectedSqr = sqr;
                _draggedPiece = pcs;
                _pieceViews.at(pcs).setPosition(localPos + dragOffset);
            }
            else if (_selectedSqr) {
                _selectedSqr = std::nullopt;
            }
        }
    }

    else if (event.is<sf::Event::MouseMoved>()) {
        if (_draggedPiece) {
            _pieceViews.at(_draggedPiece).setPosition(localPos + dragOffset);
        }
    }

    else if (event.is<sf::Event::MouseButtonReleased>() && event.getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {
        if (_draggedPiece && _selectedSqr) {
            sf::Vector2i dest = localPosToSqr(localPos);
            // Check piece deselection
            if (*_selectedSqr == dest && _isDeselecting) {
                _pieceViews.at(_draggedPiece).snapToPosition(*_selectedSqr);
                _selectedSqr = std::nullopt;
                _isDeselecting = false;
            }
            else {
                bool moved = _onMoveRequest && _onMoveRequest({*_selectedSqr, dest});
                if (!moved) {
                    // Snap back
                    _pieceViews.at(_draggedPiece).snapToPosition(*_selectedSqr);
                }
                else {
                    _selectedSqr = std::nullopt;
                }
            }
            _draggedPiece = nullptr;
        }
    }
}

void BoardView::update(sf::Time dt) {
    for (auto& [piece, view] : _pieceViews) {
        view.update(dt);
    }
}

void BoardView::onBoardInit() {
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
                _pieceViews.at(pcs).snapToPosition({x, y});
            }
        }
    }
}

void BoardView::onPieceMoved(Move move) {
    auto pcs = _board.getPieceAt(move.dest);
    if (_isMoving) {
        _pieceViews.at(pcs).animateToPosition(move.dest);
        _isMoving = false;
    } else {        
        _pieceViews.at(pcs).snapToPosition(move.dest);
    }
}

void BoardView::onPieceCaptured(const Piece* piece) {
    _pieceViews.erase(piece);
}

void BoardView::onPromoteSelection(sf::Vector2i sqr, PieceType& type) {
    // Display and choose promotion type here
    type = PieceType::Queen;
}


void BoardView::onPromotion(sf::Vector2i sqr, PieceType type, const Piece* oldPcs) {
    // Delete the old piece texture
    _pieceViews.erase(oldPcs);

    // Add new PieceView
    auto pcs = _board.getPieceAt(sqr);
    if (pcs) {
        const sf::Texture& texture = _textures.get(ui::getTextureId(*pcs));
        _pieceViews.insert_or_assign(pcs, PieceView(texture, _tileSize, *pcs));
        auto it = _pieceViews.find(pcs);
        if (it != _pieceViews.end()) {
            it->second.snapToPosition(sqr);
        }
    }
}

void BoardView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    // Draw grid;
    sf::RectangleShape square({_tileSize, _tileSize});
    for (int col = 0; col < Board::SIZE; col++) {
        for (int row = 0; row < Board::SIZE; row++) {
            square.setPosition({col * _tileSize, row * _tileSize});
            square.setFillColor((col + row) % 2 == 0 ? _lightColor : _darkColor);
            target.draw(square, states);
            if (_selectedSqr == sf::Vector2i(row, col)) {
                // Draw an overlay for selected square
                sf::Color yellow = {255, 235, 59, 100};
                square.setFillColor(yellow);
                target.draw(square, states);
            }
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

sf::Vector2i BoardView::localPosToSqr(const sf::Vector2f& localPos) const {
    int col = static_cast<int>(std::floor(localPos.x / _tileSize));
    int row = static_cast<int>(std::floor(localPos.y / _tileSize));
    return sf::Vector2i(row, col);
}