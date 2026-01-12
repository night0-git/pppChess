#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Component.hpp"
#include "RoundedRect.hpp"
#include <functional>
#include <string>

namespace ui {

// enum class ButtonState { Idle, Hovered, Pressed };

class Button : public Component {
public:
    Button(sf::Vector2f pos, sf::Vector2f size,
        const std::string& text, const sf::Font& font);

    Button& setCallback(std::function<void()> callback);

    Button& setSize(sf::Vector2f size);
    Button& setTextSize(unsigned int size);
    Button& setTextScale(float scale);

    Button& setBoxColor(sf::Color color);
    Button& setTextColor(sf::Color color);

    Button& setCornerRadius(float radius);
    Button& setDepthOffset(float offset);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void setPosition(sf::Vector2f pos) override;
   
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Color _topColor = sf::Color(38, 37, 34);    // Dark gray
    sf::Color _bottomColor = sf::Color(29, 28, 26);
    sf::Color _textColor = sf::Color::White;
    float _depthOffset = 16.f;

    RoundedRect _body;
    RoundedRect _shadow;
    sf::Text _text;

    // ButtonState _state = ButtonState::Idle;
    bool _isPressed = false;

    void centerText();
    void scaleAndCenterText(float scale = 0.5f);

    std::function<void()> _callback;
};

}

#endif