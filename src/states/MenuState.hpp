#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include "State.hpp"
#include "../ui/components/Container.hpp"

class MenuState : public State {
public:
    explicit MenuState(Context& context);

public:
    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render() override;

    void pause() override;
    void resume() override;

private:
    sf::Vector2f _buttonSize = sf::Vector2f(400, 100);
    float _padding = 20;
    sf::Font _font = sf::Font("./assets/fonts/Open_Sans/static/OpenSans-Regular.ttf");
    ui::VerticalContainer _menu = ui::VerticalContainer(_buttonSize, _padding);
};

#endif