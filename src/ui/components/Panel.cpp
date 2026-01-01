#include "../../../include/ui/components/Panel.hpp"

ui::Panel::Panel(const sf::Vector2f& size, const sf::Color& color)
: _background(size) {
    _background.setFillColor(color);
}

void ui::Panel::addComponent(std::shared_ptr<sf::Drawable> component) {
    _components.push_back(std::move(component));
}

void ui::Panel::setSize(const sf::Vector2f& size) {
    _background.setSize(size);
}

void ui::Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_background, states);
    for (const auto& component : _components) {
        target.draw(*component, states);
    }
}