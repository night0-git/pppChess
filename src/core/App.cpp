#include "../../include/core/App.hpp"
#include "../../include/states/MenuState.hpp"

App::App() : _window(sf::VideoMode({1080, 720}), "pppChess") {
    _window.setFramerateLimit(60);
    _context.window = &_window;
    _context.states = &_states;
    _context.textures = &_textures;
    _context.clock = &_clock;
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