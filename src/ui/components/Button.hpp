#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Component.hpp"
#include "RoundedRect.hpp"
#include <functional>
#include <string>

namespace ui {

class Button : public Component {
public:
    Button(sf::Vector2f size, unsigned int textSize,
        const std::string& text, const sf::Font& font);

    unsigned int getTextSize() const;
    sf::Vector2f getSize() const override;

    Button& setCallback(std::function<void()> callback);
    Button& setTextSize(unsigned int size);
    Button& setTextScale(float scale);
    Button& setBoxColor(sf::Color color);
    Button& setTextColor(sf::Color color);
    Button& setCornerRadius(float radius);
    Button& setDepthOffset(float offset);
    void setSize(sf::Vector2f size) override;

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   
private:
    sf::Color _topColor = sf::Color(38, 37, 34);    // Dark gray
    sf::Color _bottomColor = sf::Color(29, 28, 26);
    sf::Color _textColor = sf::Color::White;
    float _depthOffset = 10.f;

    RoundedRect _body;
    RoundedRect _shadow;
    sf::Text _text;

    void centerText();
    void scaleAndCenterText(float scale = 0.5f);

    std::function<void()> _callback;
};

}

#endif