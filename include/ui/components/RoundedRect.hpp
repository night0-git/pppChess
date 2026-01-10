#ifndef ROUNDED_RECT_HPP
#define ROUNDED_RECT_HPP

#include <SFML/Graphics.hpp>

class RoundedRect : public sf::Shape {
public:
    RoundedRect(sf::Vector2f size = {200, 100}, float radius = 10, unsigned int cornerPoints = 15);

    sf::Vector2f getSize() const;
    void setSize(sf::Vector2f size);
    void setRadius(float radius);

    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;

private:
    sf::Vector2f _size;
    float _radius;
    unsigned int _cornerPoints;
};

#endif // ROUNDED_RECT_HPP