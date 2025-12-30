#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include "State.hpp"

class MenuState : public State {
public:
    explicit MenuState(Context& context);

public:
    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    void pause() override;
    void resume() override;

private:
    sf::Text _text;
    sf::Font _font = sf::Font("./assets/fonts/Open_Sans/static/OpenSans-Regular.ttf");
};

#endif