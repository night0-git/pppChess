#include "../../../include/ui/components/Panel.hpp"
using ui::Panel;

Panel::Panel(const sf::Vector2f& size, const sf::Color& color)
: _background(size) {
    _background.setFillColor(color);
}

void Panel::addComponent(std::shared_ptr<Component> component) {
    _components.push_back(std::move(component));
}

void Panel::setSize(const sf::Vector2f& size) {
    _background.setSize(size);
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_background, states);
    for (const auto& component : _components) {
        target.draw(*component, states);
    }
}