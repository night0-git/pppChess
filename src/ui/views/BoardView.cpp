#include "../../../include/ui/views/BoardView.hpp"

ui::BoardView::BoardView(const ResourceManager<TextureId, sf::Texture>& textures, const Board& board)
: _textures(textures), _board(board) {}

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

void ui::BoardView::onPieceMoved(const sf::Vector2i& src, const sf::Vector2i& dest) {
    auto pcs = _board.getPieceAt(dest);
    _pieceViews.at(pcs).updatePosition(dest);
    // insert sliding animation
}

void ui::BoardView::onPieceCaptured(const Piece* piece) {
    _pieceViews.erase(piece);
}

void ui::BoardView::onPromotion(const sf::Vector2i& sqr, PieceType type) {
    auto pcs = _board.getPieceAt(sqr);
    const sf::Texture& texture = _textures.get(ui::getTextureId(*pcs));
    _pieceViews.insert_or_assign(pcs, PieceView(texture, _tileSize, *pcs));
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
        target.draw(view, states);
    }
}