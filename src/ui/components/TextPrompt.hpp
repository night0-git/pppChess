#ifndef TEXT_PROMPT_HPP
#define TEXT_PROMPT_HPP

#include "Component.hpp"
#include "RoundedRect.hpp"
#include <string>
#include <SFML/Window.hpp>

namespace ui {

class TextPrompt : public Component {
public:
    TextPrompt(sf::Vector2f size, const sf::Font& font, const std::string& prompt, sf::Color boxColor = sf::Color::White, sf::Color textColor = sf::Color::Black, float offset = 15);
    
    void clear();

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Vector2f getSize() const override;
    void setSize(sf::Vector2f size) override;

private:
    float _offset;
    RoundedRect _body;
    sf::Text _prompt;
    sf::String _string;
    sf::Text _text;
    sf::RectangleShape _cursor;
    void normalizeText();

    // Could be from 0 to n, if index is n then the cursor is placed
    // at the end of the string.
    // Corresponds to character's index
    int _cursorIdx = 0;
    bool _isCursorShowing = true;
    sf::Time _cursorBlinkInterval = sf::seconds(0.7);
    sf::Time _cursorElapsed = sf::Time::Zero;

    void updateCursorPos(int idx);
    void delayCursorBlinking();
};

}

#endif  // TEXT_PROMPT_HPP