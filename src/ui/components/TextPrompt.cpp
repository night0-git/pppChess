#include "TextPrompt.hpp"
#include <algorithm>
using ui::TextPrompt;

#include <iostream>
TextPrompt::TextPrompt(sf::Vector2f size, const sf::Font& font, const std::string& prompt, sf::Color boxColor, sf::Color textColor, float offset)
: _prompt(font, prompt), _text(font, ""), _offset(offset) {
    _body.setCornerRadius(0);
    _body.setSize(size);
    _body.setFillColor(boxColor);

    _prompt.setFillColor(sf::Color(147, 150, 150));
    _text.setFillColor(textColor);
    _cursor.setFillColor(sf::Color::Black);
    _cursor.setOutlineColor(sf::Color::White);
    normalizeText();
}

void TextPrompt::clear() {
    _string.clear();
    _text.setString(_string);
    updateCursorPos(0);
}

void TextPrompt::handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) {
    sf::FloatRect bounds = getTransform().transformRect(_body.getGlobalBounds());
    bool isHovered = bounds.contains(mouseWorldPos);

    if (event.is<sf::Event::MouseButtonPressed>() 
    && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        _state = (isHovered ? State::Pressed : State::Idle);
    }
    
    if (_state == State::Pressed) {
        if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
            auto unicode = textEvent->unicode;
            // Backspace
            if (textEvent->unicode == 8 && _cursorIdx > 0) {
                _string.erase(_cursorIdx - 1, 1);
                _text.setString(_string);
                updateCursorPos(_cursorIdx - 1);
            }
            // Printable characters
            if (unicode != 8 && unicode <= 128) {
                _string.insert(_cursorIdx, unicode);
                _text.setString(_string);
                updateCursorPos(_cursorIdx + 1);
            }
            delayCursorBlinking();
        }
        
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scan::Left) {
                updateCursorPos(_cursorIdx - 1);
                delayCursorBlinking();
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
                updateCursorPos(_cursorIdx + 1);
                delayCursorBlinking();
            }
        }
    }
}

void TextPrompt::update(sf::Time dt) {
    _cursorElapsed += dt;
    
    if (_cursorElapsed >= _cursorBlinkInterval) {
        _isCursorShowing = !_isCursorShowing;
        _cursorElapsed = sf::Time::Zero;
    }
}

void TextPrompt::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_body, states);
    if (_state != ui::State::Pressed && _text.getString().isEmpty()) {
        target.draw(_prompt, states);
    }
    target.draw(_text, states);
    if (_state == ui::State::Pressed && _isCursorShowing) {
        target.draw(_cursor, states);
    }
}

sf::Vector2f TextPrompt::getSize() const {
    return _body.getSize();
}

void TextPrompt::setSize(sf::Vector2f size) {
    _body.setSize(size);
    normalizeText();
}

void TextPrompt::normalizeText() {
    _prompt.setCharacterSize(_body.getSize().y * 0.3);
    if (_prompt.getString().isEmpty()) {
        _prompt.setString(" ");
    }
    _prompt.setOrigin({0, _prompt.getLocalBounds().getCenter().y});
    _prompt.setPosition({_offset, _body.getSize().y / 2});

    _text.setCharacterSize(_body.getSize().y * 0.5);
    // Make sure text has at least one character to determine the bounds
    sf::String oldStr = _text.getString();
    _text.setString("1");
    _text.setOrigin({0, _text.getLocalBounds().getCenter().y});
    _text.setString(oldStr);
    _text.setPosition({_offset, _body.getSize().y / 2});

    _cursor.setSize({2, _body.getSize().y * 0.6f});
    _cursor.setOrigin({0, _cursor.getSize().y / 2});
    _cursor.setPosition({_text.findCharacterPos(0).x, _body.getSize().y / 2});
}

void TextPrompt::updateCursorPos(int idx) {
    _cursorIdx = std::clamp(idx, 0, static_cast<int>(_string.getSize()));

    _cursor.setPosition({_text.findCharacterPos(_cursorIdx).x, _body.getSize().y / 2});
}


// Prevent the cursor from blinking while typing
void TextPrompt::delayCursorBlinking() {
    _isCursorShowing = true;
    _cursorElapsed = sf::Time::Zero;
}