#ifndef _APP_H_
#define _APP_H_

#include "Context.hpp"
#include "StateManager.hpp"
#include "LayoutManager.hpp"
#include "../ui/core/Resource.hpp"
#include "../ui/core/ResourceManager.hpp"
#include "../ui/core/SoundPlayer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    LayoutManager _layoutManager;

    ui::ResourceManager<ui::TextureId, sf::Texture> _textures;
    ui::ResourceManager<ui::SoundId, sf::SoundBuffer> _sounds;
    ui::SoundPlayer _soundPlayer;

    sf::Clock _clock;
    Cursors _cursors;
    Settings _settings;
    
    sf::TcpSocket _socket;
    sf::TcpListener _listener;

    Context _context;
};

#endif