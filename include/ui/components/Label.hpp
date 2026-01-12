#ifndef _LABEL_H_
#define _LABEL_H_

#include <SFML/Graphics.hpp>
#include <string>

namespace ui {

class Label : public sf::Drawable, public sf::Transformable {
public:
    Label(const sf::Font& font, const std::string& text, unsigned int charSize = 24);

    void setText(const std::string& text);
    void setFillColor(sf::Color color);
    void setCharSize(unsigned int size);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Text _text;
};

}

#endif