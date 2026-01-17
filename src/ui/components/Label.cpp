// #include "../../../include/ui/components/Label.hpp"
// using ui::Label;

// Label::Label(const std::string& text, const sf::Font& font,
// unsigned int charSize) : _text(font, text, charSize) {}

// void Label::setText(const std::string& text) {
//     _text.setString(text);
// }

// void Label::setFillColor(sf::Color color) {
//     _text.setFillColor(color);
// }

// void Label::setCharSize(unsigned int size) {
//     _text.setCharacterSize(size);
// }

// void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
//     states.transform *= getTransform();

//     target.draw(_text, states);
// }