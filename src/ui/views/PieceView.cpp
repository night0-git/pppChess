#include "../../../include/ui/views/PieceView.hpp"
#include <utility>
#include <map>
using std::map, std::pair;

ui::PieceView::PieceView(const sf::Texture& texture, float tileSize,
const Piece& piece) : _sprite(texture), _tileSize(tileSize),
_piece(&piece) {
    normalizeSprite();
}

void ui::PieceView::updatePosition(const sf::Vector2i& sqr) {
    auto newPos = sf::Vector2f(sqr.y * _tileSize, sqr.x * _tileSize);
    
    this->setPosition(newPos);
}

void ui::PieceView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_sprite, states);
}

void ui::PieceView::normalizeSprite() {
    sf::Vector2u textureSize = _sprite.getTexture().getSize();
    float scaleFactor = 0.8f;
    float targetSize = _tileSize * scaleFactor;

    // Set scale
    float scale = targetSize / static_cast<float>(textureSize.y);
    _sprite.setScale({scale, scale});

    // Set position
    _sprite.setOrigin({textureSize.x / 2.f, textureSize.y / 2.f});
    _sprite.setPosition({_tileSize / 2.f, _tileSize / 2.f});
}