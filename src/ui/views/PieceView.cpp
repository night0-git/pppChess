#include "PieceView.hpp"
#include <utility>
#include <map>
#include <iostream>
using std::map, std::pair;
using ui::PieceView;

PieceView::PieceView(const sf::Texture& texture, float tileSize,
const Piece& piece) : _sprite(texture), _tileSize(tileSize),
_piece(&piece) {
    normalizeSprite();
    std::cerr << getOrigin().x << " " << getOrigin().y << "\n";
}

void PieceView::snapToSquare(sf::Vector2i sqr) {
    this->setPosition(sqrToPos(sqr));
}

void PieceView::animateToPosition(sf::Vector2i sqr) {
    _startPos = getPosition();
    _targetPos = sqrToPos(sqr);
    _animationTime = sf::Time::Zero;
    _isAnimating = true;
}

void PieceView::update(sf::Time dt) {
    if (!_isAnimating) {
        return;
    }

    _animationTime += dt;
    float progress = _animationTime.asSeconds() / _animationDuration.asSeconds();

    if (progress >= 1.f) {
        _isAnimating = false;
        setPosition(_targetPos);
    } else {
        // Linear interpolation for smooth movement
        sf::Vector2f newPos = _startPos + (_targetPos - _startPos) * progress;
        setPosition(newPos);
    }
}

void PieceView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_sprite, states);
}

void PieceView::normalizeSprite() {
    sf::Vector2u textureSize = _sprite.getTexture().getSize();
    float scaleFactor = 0.8f;
    float targetSize = _tileSize * scaleFactor;

    // Set scale
    float scale = targetSize / static_cast<float>(textureSize.y);
    _sprite.setScale({scale, scale});

    _sprite.setOrigin({textureSize.x / 2.f, textureSize.y / 2.f});
}

sf::Vector2f PieceView::sqrToPos(sf::Vector2i sqr) const {
    return sf::Vector2f((sqr.y + 0.5) * _tileSize, (sqr.x + 0.5) * _tileSize);
}
