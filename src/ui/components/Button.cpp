#include "../../../include/ui/components/Button.hpp"
using ui::Button, ui::State;

Button::Button(sf::Vector2f size, unsigned int textSize,
const std::string& text, const sf::Font& font) : _text(font, text) {
    _body.setSize(size);
    _body.setFillColor(_topColor);
    _body.setPosition({0, 0});

    _shadow.setOrigin({0, -_depthOffset});
    _shadow.setSize(_body.getSize());
    _shadow.setFillColor(_bottomColor);
    _shadow.setPosition({0, 0});

    _text.setFillColor(_textColor);
    _text.setCharacterSize(textSize);
    centerText();
}

unsigned int Button::getTextSize() const {
    return _text.getCharacterSize();
}

sf::Vector2f Button::getSize() const {
    return _body.getSize() + sf::Vector2f(0, _depthOffset);
}

Button& Button::setCallback(std::function<void()> callback) {
    _callback = std::move(callback);
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

Button& Button::setBoxColor(sf::Color color) {
    _body.setFillColor(color);
    _shadow.setFillColor(darkened(color));
    _topColor = color;
    _bottomColor = darkened(color);
    return *this;
}

Button& Button::setTextColor(sf::Color color) {
    _text.setFillColor(color);
    _textColor = color;
    return *this;
}

Button& Button::setCornerRadius(float radius) {
    _body.setCornerRadius(radius);
    _shadow.setCornerRadius(radius);
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

void Button::setSize(sf::Vector2f size) {
    _body.setSize(size - sf::Vector2f(0, _depthOffset));
    _shadow.setSize(_body.getSize());
    centerText();
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) {
    sf::FloatRect bounds = getTransform().transformRect(_body.getGlobalBounds());
    bool isHovered = bounds.contains(mouseWorldPos);

    if (event.is<sf::Event::MouseButtonPressed>() 
    && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left
    && isHovered) {
        _state = State::Pressed;
        _body.move({0, _depthOffset / 2});
        _text.move({0, _depthOffset / 2});
    }

    else if (event.is<sf::Event::MouseButtonReleased>()
    && event.getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left
    && _state == State::Pressed) {
        _body.move({0, -_depthOffset / 2});
        _text.move({0, -_depthOffset / 2});
        if (isHovered && _callback) {
            _callback();
        }

        _state = isHovered ? State::Hovered : State::Idle;
    }

    else if (event.is<sf::Event::MouseMoved>()) {
        if (_state != State::Pressed) {
            _state = isHovered ? State::Hovered : State::Idle;
        }
    }
}

void Button::update(sf::Time dt) {
    
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
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