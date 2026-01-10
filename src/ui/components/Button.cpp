#include "../../../include/ui/components/Button.hpp"
using ui::Button;

Button::Button(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& text, const sf::Font& font)
: _text(font, text) {
    _body.setSize(size - sf::Vector2f(0, _depthOffset));
    _body.setFillColor(_topColor);
    _body.setPosition(pos);

    _shadow.setOrigin({0, -_depthOffset});
    _shadow.setSize(_body.getSize());
    _shadow.setFillColor(_bottomColor);
    _shadow.setPosition(pos);

    _text.setFillColor(_textColor);
    scaleAndCenterText();
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    sf::FloatRect bounds = getTransform().transformRect(_body.getGlobalBounds());
    bool isHovered = bounds.contains(worldPos);

    if (event.is<sf::Event::MouseButtonPressed>() && isHovered) {
        _body.move({0, _depthOffset / 2});
        _text.move({0, _depthOffset / 2});
        _isPressed = true;
    }

    if (event.is<sf::Event::MouseButtonReleased>() && _isPressed) {
        _body.move({0, -_depthOffset / 2});
        _text.move({0, -_depthOffset / 2});
        _isPressed = false;
        if (isHovered && _callback) {
            _callback();
        }
    }
}

Button& Button::setCallback(std::function<void()> callback) {
    _callback = std::move(callback);
    return *this;
}

Button& Button::setSize(const sf::Vector2f& size) {
    _body.setSize(size - sf::Vector2f(0, _depthOffset));
    _shadow.setSize(_body.getSize());
    scaleAndCenterText();
    return *this;
}

Button& Button::setTextSize(unsigned int size) {
    _text.setCharacterSize(size);
    centerText();
    return *this;
}

Button& Button::setTextScale(float scale) {
    scaleAndCenterText(scale);
    return *this;
}

Button& Button::setBoxColor(const sf::Color& color) {
    _body.setFillColor(color);
    _shadow.setFillColor(darkened(color));
    _topColor = color;
    _bottomColor = darkened(color);
    return *this;
}

Button& Button::setTextColor(const sf::Color& color) {
    _text.setFillColor(color);
    _textColor = color;
    return *this;
}

Button& Button::setCornerRadius(float radius) {
    _body.setRadius(radius);
    _shadow.setRadius(radius);
    return *this;
}

Button& Button::setDepthOffset(float offset) {
    float newHeight = _body.getSize().y - (offset - _depthOffset); 
    _depthOffset = offset;
    _shadow.setOrigin({0, -_depthOffset});
    _body.setSize({_body.getSize().x, newHeight});
    _shadow.setSize(_body.getSize());
    centerText();
    return *this;
}

void Button::setPosition(const sf::Vector2f& pos) {
    _body.setPosition(pos);
    _shadow.setPosition(pos);
    _text.setPosition(_body.getGlobalBounds().getCenter());
}

void ui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_shadow, states);
    target.draw(_body, states);
    target.draw(_text, states);
}

void Button::centerText() {
    sf::Vector2f textCenter = _text.getLocalBounds().getCenter();
    _text.setOrigin(textCenter);
    _text.setPosition(_body.getGlobalBounds().getCenter());
}

void Button::scaleAndCenterText(float scale) {
    unsigned int charSize = static_cast<unsigned int>(_body.getSize().y * scale);
    _text.setCharacterSize(charSize);
    centerText();
}