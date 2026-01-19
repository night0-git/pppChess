#ifndef PANEL_HPP
#define PANEL_HPP

#include "Component.hpp"
#include "RoundedRect.hpp"
#include <string>

namespace ui {

class Panel : public Component {
public:
    Panel(sf::Vector2f size, sf::Color color);
    Panel(const sf::Texture& texture);

    sf::Vector2f getSize() const override;

    void setSize(sf::Vector2f size) override;
    void setColor(sf::Color color);
    void setTexture(const sf::Texture& texture);
    void setTextureRect(const sf::IntRect& rect);
    void setCornerRadius(float radius, unsigned int cornerPoints = 15);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    RoundedRect _canvas;
};

}

#endif