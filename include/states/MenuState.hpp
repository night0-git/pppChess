#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include "State.hpp"
#include "../ui/components/Button.hpp"
#include "../ui/components/Panel.hpp"

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
    sf::Font _font = sf::Font("./assets/fonts/Open_Sans/static/OpenSans-Regular.ttf");
    ui::VerticalPanel _menu;
};

#endif