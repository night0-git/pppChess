#include "../../include/states/GameState.hpp"
#include "../../include/states/MenuState.hpp"
#include "../../include/states/SettingsState.hpp"

GameState::GameState(Context& context) : _context(context), _text(_font, "Game") {}

void GameState::init() {

}

void GameState::handleEvent(const sf::Event& event) {
    if (const auto& keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            _context.states->pushState(std::make_unique<SettingsState>(_context), false);
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            _context.states->popState();
        }
    }
}

void GameState::update(float dt) {

}

void GameState::render(sf::RenderWindow& window) {
    window.draw(_text);
}

void GameState::pause() {
    // Stop the timer, music...?
}

void GameState::resume() {
    // ...
}