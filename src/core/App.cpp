#include "../../include/core/App.hpp"
#include "../../include/states/MenuState.hpp"

App::App() : _soundPlayer(_sounds), _layoutManager(_window, sf::Vector2f(30, 30)) {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int w = static_cast<unsigned int>(desktop.size.x * 0.7f);
    unsigned int h = static_cast<unsigned int>(desktop.size.y * 0.8f);
    _window.create(sf::VideoMode({w, h}), "pppChess");
    _window.setMinimumSize(sf::Vector2u(1280, 720));
    if (_cursors.arrow) _window.setMouseCursor(*_cursors.arrow);
    
    _window.setVerticalSyncEnabled(true);
    _context.window = &_window;
    _context.states = &_states;
    _context.layoutManager = &_layoutManager;
    _context.textures = &_textures;
    _context.sounds = &_sounds;
    _context.soundPlayer = &_soundPlayer;
    _context.clock = &_clock;
    _context.settings = &_settings;
    _context.cursors = &_cursors;

    _states.pushState(std::make_unique<MenuState>(_context));
}

void App::run() {
    while (_window.isOpen()) {
        _states.processStateChanges();

        sf::Time dt = _clock.restart();
        processEvents();
        update(dt);
        render();
    }
}

void App::processEvents() {
    while (const std::optional event = _window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            _window.close();
        }
        if (_states.currentState()) {
            _states.currentState()->handleEvent(event.value());
        }
    }
}

void App::update(sf::Time dt) {
    if (_states.currentState()) {
        _states.currentState()->update(dt);
    }
}

void App::render() {
    _window.clear();

    if (_states.currentState()) {
        _states.currentState()->render();
    }

    _window.display();
}