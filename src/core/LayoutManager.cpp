#include "LayoutManager.hpp"

LayoutManager::LayoutManager(const sf::RenderWindow& window, sf::Vector2f padding)
: _window(window), _padding(padding) {}

void LayoutManager::setPadding(sf::Vector2f padding) {
    _padding = padding;
}

sf::Vector2f LayoutManager::calculatePosition(Anchor anchor, sf::Vector2f elementSize) const {
    float x = 0;
    float y = 0;
    sf::Vector2f windowSize = sf::Vector2f(_window.getSize());
    switch (anchor) {
    case Anchor::Left:
        x = _padding.x;
        y = (_window.getSize().y - elementSize.y) / 2;
        break;
    case Anchor::Right:
        x = _window.getSize().x - _padding.x;
        y = (_window.getSize().y - elementSize.y) / 2;
        break;
    case Anchor::TopLeft:
        x = _padding.x;
        y = _padding.y;
        break;
    case Anchor::BottomLeft:
        x = _padding.x;
        y = windowSize.y - _padding.y - elementSize.y;
        break;
    case Anchor::TopRight:
        x = windowSize.x - _padding.x - elementSize.x;
        y = _padding.y;
        break;
    case Anchor::BottomRight:
        x = windowSize.x - _padding.x - elementSize.x;
        y = windowSize.y - _padding.y - elementSize.y;
        break;
    case Anchor::Center:
        x = (windowSize.x - elementSize.x) / 2;
        y = (windowSize.y - elementSize.y) / 2;
        break;    
    }
    return {x, y};
}