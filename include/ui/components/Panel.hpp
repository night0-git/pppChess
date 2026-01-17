#ifndef _PANEL_H_
#define _PANEL_H_

#include "Component.hpp"
#include "RoundedRect.hpp"
#include <string>
#include <vector>
#include <memory>

namespace ui {

class Panel : public Component {
public:
    Panel(sf::Vector2f size, sf::Color color = sf::Color(50, 50, 50, 0));
    virtual ~Panel() = default;

    virtual void addComponent(std::unique_ptr<Component> component) = 0;
    
    virtual void setSpacing(float spacing) = 0;
    void setColor(sf::Color color);

    sf::Vector2f getSize() const override = 0;
    void setSize(sf::Vector2f size) override;

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
    RoundedRect _canvas;
    std::vector<std::unique_ptr<Component>> _components;
    float _spacing = 20.f;
    sf::Vector2f _cursor = sf::Vector2f(0, 0);
};

class VerticalPanel : public Panel {
public:
    VerticalPanel(sf::Vector2f size, sf::Color color = sf::Color(50, 50, 50, 0));

    void addComponent(std::unique_ptr<Component> component) override;
    void setSpacing(float spacing) override;
    sf::Vector2f getSize() const override;
};

class HorizontalPanel : public Panel {
public:
    HorizontalPanel(sf::Vector2f size, sf::Color color = sf::Color(50, 50, 50, 0));

    void addComponent(std::unique_ptr<Component> component) override;
    void setSpacing(float spacing) override;
    sf::Vector2f getSize() const override;
};

}

#endif