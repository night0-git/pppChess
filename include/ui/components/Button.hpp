#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

namespace ui {

class Button : public sf::Drawable, public sf::Transformable {
public:
    Button(const sf::Vector2f& size, const sf::Font& font, const std::string& text);

    void setCallBack(std::function<void()> callback);
   
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape _shape;
    sf::Text _text;
    std::function<void()> _callback;
};

}

#endif