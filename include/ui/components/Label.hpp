// #ifndef _LABEL_H_
// #define _LABEL_H_

// #include "Component.hpp"
// #include "RoundedRect.hpp"
// #include <string>

// namespace ui {

// class Label : public Component {
// public:
//     Label(const std::string& text, const sf::Font& font, unsigned int charSize = 24);

//     void setText(const std::string& text);
//     void setFillColor(sf::Color color);
//     void setCharSize(unsigned int size);

//     sf::Vector2f getSize() const override;
//     void setSize(sf::Vector2f size) override;

//     void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

// private:
//     sf::Text _text;
//     RoundedRect _background;
// };

// }

// #endif