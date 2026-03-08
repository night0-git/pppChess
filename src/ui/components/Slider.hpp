#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "Component.hpp"
#include "RoundedRect.hpp"

namespace ui {

class Slider : public Component {
public:
    Slider(sf::Vector2f rectSize, sf::Color color, int initialVal = 50);
    
    int value() const;

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Vector2f getSize() const override;
    void setSize(sf::Vector2f size) override;

   
private:
    int _min = 0;
    int _max = 100;
    int _value;
    
    sf::Vector2f _rectSize;
    float _barHeight = 15;
    RoundedRect _bar;
    sf::CircleShape _circle;

    bool _isCircleHovered;
    void updateCirclePos();
};

}

#endif