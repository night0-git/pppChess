#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics.hpp>

namespace ui {

class Component : public sf::Drawable, public sf::Transformable {
public:
    virtual ~Component() = default;
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

    static sf::Color lightened(const sf::Color& color, float amount = 0.2f);
    static sf::Color darkened(const sf::Color& color, float amount = 0.2f);
};

}

#endif // COMPONENT_HPP