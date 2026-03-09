#ifndef CHECK_BOX_HPP
#define CHECK_BOX_HPP

#include "Component.hpp"
#include "RoundedRect.hpp"

namespace ui {

class CheckBox : public Component {
public:
    CheckBox(bool& value);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Vector2f getSize() const;
    void setSize(sf::Vector2f size);

private:
    bool& _value;
};

}

#endif