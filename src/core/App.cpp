#include "../../include/core/App.hpp"

App::App() : _window(sf::VideoMode({1080, 720}), "pppChess") {
    _window.setFramerateLimit(60);
}

void App::run() {
    while (_window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void App::processEvents() {
    while (const std::optional event = _window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            _window.close();
        }
    }
}

void App::update() {

}

void App::render() {
    _window.clear();

    sf::Texture texture("assets/custom/pieces/pieces.png");
    sf::Sprite sprite(texture);
    sprite.setScale({0.5f, 0.5f});
    _window.draw(sprite);

    _window.display();
}