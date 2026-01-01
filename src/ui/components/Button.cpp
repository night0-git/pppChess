#include "../../../include/ui/components/Button.hpp"

ui::Button::Button(const sf::Vector2f& size, const sf::Font& font, const std::string& text)
: _text(font, text), _shape(size) {}

void ui::Button::setCallBack(std::function<void()> callback){
    _callback = std::move(callback);
}

void ui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(_shape, states);
}