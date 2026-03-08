#include "Slider.hpp"
#include <algorithm>
using ui::Slider;

Slider::Slider(sf::Vector2f rectSize, sf::Color color, int initialVal)
: _value(initialVal), _rectSize(rectSize) {
    _bar.setCornerRadius(0);
    _bar.setSize(sf::Vector2f(rectSize.x, _barHeight));
    _bar.setFillColor(sf::Color(235, 231, 223));
    _bar.setOrigin(sf::Vector2f(0, (_barHeight - _rectSize.y) / 2));
    _circle.setRadius(_barHeight);
    _circle.setFillColor(color);
    _circle.setOrigin(_circle.getGeometricCenter());
    updateCirclePos();
}

int Slider::value() const {
    return _value;
}

void Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) {
    sf::FloatRect circleBounds = getTransform().transformRect(_circle.getGlobalBounds());
    _isCircleHovered = circleBounds.contains(mouseWorldPos);
    sf::FloatRect barBounds = getTransform().transformRect(_bar.getGlobalBounds());
    bool isBarHovered = barBounds.contains(mouseWorldPos);
    bool isHovered = _isCircleHovered || isBarHovered;

    if (event.is<sf::Event::MouseButtonPressed>() 
    && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        if (_isCircleHovered) {
            _state = State::Pressed;
        }
        else if (isBarHovered) {
            _state = State::Pressed;
            sf::Vector2f localMousePos = getInverseTransform().transformPoint(mouseWorldPos);
            float ratio = localMousePos.x / _bar.getSize().x;
            _value = std::clamp(static_cast<int>(_max * ratio), _min, _max);
            updateCirclePos();
        }
    }

    else if (event.is<sf::Event::MouseMoved>()) {
        if (_state == State::Pressed) {
            sf::Vector2f localMousePos = getInverseTransform().transformPoint(mouseWorldPos);
            float ratio = localMousePos.x / _bar.getSize().x;
            _value = std::clamp(static_cast<int>(_max * ratio), _min, _max);
            updateCirclePos();
        }
        else {
            _state = isHovered ? State::Hovered : State::Idle;
        }
    }
    
    else if (event.is<sf::Event::MouseButtonReleased>()
    && event.getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left
    && _state == State::Pressed) {
        _state = isHovered ? State::Hovered : State::Idle;
    }
}

void Slider::update(sf::Time dt) {

}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_bar, states);

    if (_state == State::Pressed || _isCircleHovered) {
        sf::CircleShape shadow(_circle.getRadius() * 1.8);
        sf::Color shadowColor = _circle.getFillColor();
        shadowColor.a = 50;
        shadow.setFillColor(shadowColor);
        shadow.setOrigin(shadow.getGeometricCenter());
        shadow.setPosition(_circle.getPosition());
        target.draw(shadow, states);
    }
    target.draw(_circle, states);
}

sf::Vector2f Slider::getSize() const {
    return _rectSize;
}

void Slider::setSize(sf::Vector2f size) {
    _bar.setSize(sf::Vector2f(size.x, _barHeight));
    _rectSize = size;
    updateCirclePos();
}

void Slider::updateCirclePos() {
    float ratio = static_cast<float>(_value) / _max;
    _circle.setPosition(sf::Vector2f(_bar.getPosition().x + _bar.getSize().x * ratio, _bar.getPosition().y + _rectSize.y / 2));
}