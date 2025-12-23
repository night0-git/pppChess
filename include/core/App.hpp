#ifndef _APP_H_
#define _APP_H_

#include "StateManager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

class App {
public:
    App();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

private:
    sf::RenderWindow _window;
    StateManager _states;
    Context _context;
    sf::Clock _clock;
};

#endif