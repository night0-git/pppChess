#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics.hpp>

namespace ui {

class Component : public sf::Drawable, public sf::Transformable {
public:
    virtual ~Component() = default;

    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos);
    virtual void update(sf::Time dt);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

    virtual sf::Vector2f getSize() const = 0;
    virtual void setSize(sf::Vector2f size) = 0;

    static sf::Color lightened(sf::Color color, float amount = 0.2f);
    static sf::Color darkened(sf::Color color, float amount = 0.2f);
};

}

#endif // COMPONENT_HPP