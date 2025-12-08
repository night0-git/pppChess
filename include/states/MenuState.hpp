#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include "State.hpp"
#include "../core/StateManager.hpp"

class MenuState : public State {
public:
    explicit MenuState(Context* context);

public:
    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    void pause() override;
    void resume() override;

private:
    Context* _context;
    sf::CircleShape _shape;
};

#endif