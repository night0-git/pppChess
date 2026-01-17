#include "../../../include/ui/components/Panel.hpp"
using ui::Panel;
using ui::VerticalPanel;
using ui::HorizontalPanel;

Panel::Panel(sf::Vector2f size, sf::Color color)
: _canvas(size) {
    _canvas.setFillColor(color);
}

void Panel::setColor(sf::Color color) {
    _canvas.setFillColor(color);
}

void Panel::setSize(sf::Vector2f size) {
    _canvas.setSize(size);
    for (auto& component : _components) {
        component->setSize(size);
    }
}

void Panel::handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) {
    sf::Vector2f newMousePos = mouseWorldPos;
    
    if (event.is<sf::Event::MouseButtonPressed>() ||
        event.is<sf::Event::MouseMoved>() ||
        event.is<sf::Event::MouseButtonReleased>()) {
        newMousePos -= getPosition();
    }

    for (auto& component : _components) {
        component->handleEvent(event, window, newMousePos);
    }
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_canvas, states);
    for (const auto& component : _components) {
        target.draw(*component, states);
    }
}

VerticalPanel::VerticalPanel(sf::Vector2f size, sf::Color color)
: Panel(size, color) {}

void VerticalPanel::addComponent(std::unique_ptr<Component> component) {
    component->setPosition(_cursor);
    _cursor += sf::Vector2f(0, component->getSize().y + _spacing);
    _components.push_back(std::move(component));
}

void VerticalPanel::setSpacing(float spacing) {
    float offset = spacing - _spacing;
    _spacing = spacing;
    if (_components.size() <= 1) return;

    for (int i = 1; i < _components.size(); i++) {
        auto newPos = _components[i]->getPosition() + sf::Vector2f(0, i * offset);
        _components[i]->setPosition(newPos);
    }
}

sf::Vector2f VerticalPanel::getSize() const {
    float width = 0;
    float height = 0;
    for (const auto& component : _components) {
        auto size = component->getSize();
        height += size.y;
        if (size.x > width) {
            width = size.x;
        }
    }
    if (!_components.empty()) {
        height += _spacing * (_components.size() - 1);
    }
    return sf::Vector2f(width, height);
}

HorizontalPanel::HorizontalPanel(sf::Vector2f size, sf::Color color)
: Panel(size, color) {}

void HorizontalPanel::addComponent(std::unique_ptr<Component> component) {
    component->setPosition(_cursor);
    _cursor += sf::Vector2f(component->getSize().x + _spacing, 0);
    _components.push_back(std::move(component));
}

void HorizontalPanel::setSpacing(float spacing) {
    float offset = spacing - _spacing;
    _spacing = spacing;
    if (_components.size() <= 1) return;

    for (int i = 1; i < _components.size(); i++) {
        auto newPos = _components[i]->getPosition() + sf::Vector2f(i * offset, 0);
        _components[i]->setPosition(newPos);
    }
}

sf::Vector2f HorizontalPanel::getSize() const {
    float width = 0;
    float height = 0;
    for (const auto& component : _components) {
        auto size = component->getSize();
        width += size.x;
        if (size.y > height) {
            height = size.y;
        }
    }
    
    if (!_components.empty()) {
        width += _spacing * (_components.size() - 1);
    }
    return sf::Vector2f(width, height);
}