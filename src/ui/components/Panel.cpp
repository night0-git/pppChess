#include "../../../include/ui/components/Panel.hpp"
using ui::Panel;

Panel::Panel(sf::Vector2f size, sf::Color color)
: _canvas(size) {
    setColor(color);
}

Panel::Panel(const sf::Texture& texture) {
    setTexture(texture);
    setSize(sf::Vector2f(texture.getSize()));
}

sf::Vector2f Panel::getSize() const {
    return _canvas.getSize();
}

void Panel::setSize(sf::Vector2f size) {
    _canvas.setSize(size);
}

void Panel::setColor(sf::Color color) {
    _canvas.setFillColor(color);
}

void Panel::setTexture(const sf::Texture& texture) {
    _canvas.setTexture(&texture);
}

void Panel::setTextureRect(const sf::IntRect& rect) {
    _canvas.setTextureRect(rect);
}


void Panel::setCornerRadius(float radius, unsigned int cornerPoints) {
    _canvas.setCornerRadius(radius, cornerPoints);
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_canvas, states);
}