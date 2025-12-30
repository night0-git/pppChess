#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "State.hpp"

class GameState : public State {
public:
    explicit GameState(Context& context);

public:
    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;

    void pause() override;
    void resume() override;

private:
    sf::Text _text;
    sf::Font _font = sf::Font("./assets/fonts/Open_Sans/static/OpenSans-Regular.ttf");
};

#endif