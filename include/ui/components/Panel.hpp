#ifndef _PANEL_H_
#define _PANEL_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

namespace ui {

class Panel : public sf::Drawable, public sf::Transformable {
public:
    Panel(const sf::Vector2f& size, const sf::Color& color = sf::Color(50, 50, 50, 200));

    void addComponent(std::shared_ptr<sf::Drawable> component);
    void setSize(const sf::Vector2f& size);    

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape _background;
    std::vector<std::shared_ptr<sf::Drawable>> _components;
};

}

#endif