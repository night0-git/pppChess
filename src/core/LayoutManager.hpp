#ifndef LAYOUT_MANAGER_HPP
#define LAYOUT_MANAGER_HPP

#include <SFML/Graphics.hpp>

enum class Anchor {
    Top, Bottom, Left, Right, TopLeft, BottomLeft,
    TopRight, BottomRight, Center
};

class LayoutManager {
public:
    LayoutManager(const sf::RenderWindow& window, sf::Vector2f padding);

    void setPadding(sf::Vector2f padding);
    sf::Vector2f calculatePosition(Anchor anchor, sf::Vector2f elementSize, sf::Vector2f origin = {0, 0}) const;
    sf::Vector2f calculatePosition(Anchor anchor, sf::Vector2f elementSize, float padding, sf::Vector2f origin = {0, 0}) const;

private:
    sf::Vector2f _padding;
    const sf::RenderWindow& _window;
};

#endif // LAYOUT_MANAGER_HPP