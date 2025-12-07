#ifndef _APP_H_
#define _APP_H_

#include <SFML/Graphics.hpp>

class App {
public:
    App();
    void run();

private:
    void processEvents();
    void update();
    void render();

private:
    sf::RenderWindow _window;
};

#endif