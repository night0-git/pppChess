#include "../../../include/ui/components/Label.hpp"

ui::Label::Label(const sf::Font& font, const std::string& text,
unsigned int charSize) : _text(font, text, charSize) {}

void ui::Label::setText(const std::string& text) {
    _text.setString(text);
}

void ui::Label::setFillColor(const sf::Color& color) {
    _text.setFillColor(color);
}

void ui::Label::setCharSize(unsigned int size) {
    _text.setCharacterSize(size);
}

void ui::Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_text, states);
}