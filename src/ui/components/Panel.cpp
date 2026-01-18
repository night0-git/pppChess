#include "../../../include/ui/components/Panel.hpp"
using ui::Panel;
using ui::VerticalPanel;
using ui::HorizontalPanel;

Panel::Panel(sf::Vector2f unitSize, float spacing)
: _unitSize(unitSize), _size(unitSize), _spacing(spacing) {}

sf::Vector2f Panel::getSize() const {
    return _size;
}

sf::Vector2f Panel::getUnitSize() const {
    return _unitSize;
}

float Panel::getSpacing() const {
    return _spacing;
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

    for (const auto& component : _components) {
        target.draw(*component, states);
    }
}

VerticalPanel::VerticalPanel(sf::Vector2f unitSize, float spacing)
: Panel(unitSize, spacing) {}

void VerticalPanel::addComponent(std::unique_ptr<Component> component, bool keepRatio) {
    sf::Vector2f oldSize = component->getSize();
    if (keepRatio) {
        float scale = _unitSize.x / oldSize.x;
        component->setSize(sf::Vector2f(oldSize.x, oldSize.y) * scale);
    }
    else {
        component->setSize(_unitSize);
    }

    component->setPosition(_cursor);
    _cursor += sf::Vector2f(0, component->getSize().y + _spacing);
    _size.y = _cursor.y - _spacing;
    _components.push_back(std::move(component));
}

void VerticalPanel::setSize(sf::Vector2f size) {
    if (_size.x == 0 || _size.y == 0) return;

    float scaleX = size.x / _size.x;
    float scaleY = size.y / _size.y;

    _spacing *= scaleY; 
    _unitSize.x *= scaleX;
    _unitSize.y *= scaleY;

    _cursor = {0, 0};
    for (auto& component : _components) {
        component->setPosition(_cursor);
        sf::Vector2f oldSize = component->getSize();
        component->setSize({oldSize.x * scaleX, oldSize.y * scaleY});
        _cursor += sf::Vector2f(0, component->getSize().y + _spacing);
    }

    _size = size;
}

void VerticalPanel::setUnitSize(sf::Vector2f size) {
    if (size == _unitSize) return;

    _cursor = {0, 0};
    for (auto& component : _components) {
        component->setPosition(_cursor);
        sf::Vector2f oldSize = component->getSize();
        if (oldSize.y == _unitSize.y) {
            component->setSize(size);
        } else {
            float scale = size.x / _unitSize.x;
            component->setSize(oldSize * scale);
        }
        _cursor += {0, component->getSize().y + _spacing};
    }
    _unitSize = size;
    float height = (_components.empty()) ? 0 : _cursor.y - _spacing;
    _size = {size.x, height};
}

void VerticalPanel::setSpacing(float spacing) {
    if (spacing == _spacing) return;

    _cursor = {0, 0};
    for (auto& component : _components) {
        component->setPosition(_cursor);
        _cursor += sf::Vector2f(0, component->getSize().y + spacing);
    }
    _spacing = spacing;
    _size.y = _cursor.y - _spacing;
}

HorizontalPanel::HorizontalPanel(sf::Vector2f unitSize, float spacing)
: Panel(unitSize, spacing) {}

void HorizontalPanel::addComponent(std::unique_ptr<Component> component, bool keepRatio) {
    sf::Vector2f oldSize = component->getSize();
    if (keepRatio) {
        float scale = _unitSize.y / oldSize.y;
        component->setSize(sf::Vector2f(oldSize.x, oldSize.y) * scale);
    }
    else {
        component->setSize(_unitSize);
    }

    component->setPosition(_cursor);
    _cursor += sf::Vector2f(component->getSize().x + _spacing, 0);
    _size.x = _cursor.x - _spacing;

    _components.push_back(std::move(component));
}

void HorizontalPanel::setSize(sf::Vector2f size) {
    if (_size.x == 0 || _size.y == 0) return;

    float scaleX = size.x / _size.x;
    float scaleY = size.y / _size.y;

    _spacing *= scaleX; 
    _unitSize.x *= scaleX;
    _unitSize.y *= scaleY;

    _cursor = {0, 0};
    for (auto& component : _components) {
        component->setPosition(_cursor);
        sf::Vector2f oldSize = component->getSize();
        component->setSize({oldSize.x * scaleX, oldSize.y * scaleY});
        _cursor += sf::Vector2f(component->getSize().x + _spacing, 0);
    }

    _size = size;
}

void HorizontalPanel::setUnitSize(sf::Vector2f size) {
    if (size == _unitSize) return;

    _cursor = {0, 0};
    for (auto& component : _components) {
        component->setPosition(_cursor);
        sf::Vector2f oldSize = component->getSize();
        if (oldSize.x == _unitSize.x) {
            component->setSize(size);
        } else {
            float scale = size.y / _unitSize.y;
            component->setSize(oldSize * scale);
        }
        _cursor += {component->getSize().x + _spacing, 0};
    }
    _unitSize = size;
    float width = (_components.empty()) ? 0 : _cursor.x - _spacing;
    _size = {width, size.y};
}

void HorizontalPanel::setSpacing(float spacing) {
    if (spacing == _spacing) return;

    _cursor = {0, 0};
    for (auto& component : _components) {
        component->setPosition(_cursor);
        _cursor += sf::Vector2f(component->getSize().x + spacing, 0);
    }
    _spacing = spacing;
    _size.x = _cursor.x - _spacing; 
}