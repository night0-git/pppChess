#ifndef _PANEL_H_
#define _PANEL_H_

#include "Component.hpp"
#include <string>
#include <vector>
#include <memory>

namespace ui {

class Panel : public Component {
public:
    Panel(sf::Vector2f size, sf::Color color = sf::Color(50, 50, 50, 200));

    void addComponent(std::shared_ptr<Component> component);
    void setSize(sf::Vector2f size);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape _background;
    std::vector<std::shared_ptr<Component>> _components;
};

}

#endif