#ifndef _APP_H_
#define _APP_H_

#include "Context.hpp"
#include "StateManager.hpp"
#include "../ui/core/Resource.hpp"
#include "../ui/core/ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class App {
public:
    App();
    void run();

private:
    void processEvents();
    void update(sf::Time dt);
    void render();

private:
    sf::RenderWindow _window;
    StateManager _states;
    ui::ResourceManager<ui::TextureId, sf::Texture> _textures;
    sf::Clock _clock;

    Context _context;
};

#endif