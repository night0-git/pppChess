#ifndef _SETTINGS_STATE_H_
#define _SETTINGS_STATE_H_

#include "State.hpp"
#include "../core/StateManager.hpp"

class SettingsState : public State {
public:
    explicit SettingsState(Context& context);

public:
    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    void pause() override;
    void resume() override;

private:
    Context& _context;
    sf::Text _text;
    sf::Font _font = sf::Font("./assets/fonts/Open_Sans/static/OpenSans-Regular.ttf");
};

#endif