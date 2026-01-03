#ifndef _SETTINGS_STATE_H_
#define _SETTINGS_STATE_H_

#include "State.hpp"

class SettingsState : public State {
public:
    explicit SettingsState(Context& context);

public:
    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render() override;

    void pause() override;
    void resume() override;

private:
    sf::Text _text;
    sf::Font _font = sf::Font("./assets/fonts/Open_Sans/static/OpenSans-Regular.ttf");
};

#endif