#include "../../include/states/MenuState.hpp"
#include "../../include/states/GameState.hpp"
#include "../../include/states/SettingsState.hpp"
#include "../../include/core/StateManager.hpp"

MenuState::MenuState(Context& context) : State(context), _text(_font, "Menu") {}

void MenuState::init() {

}

void MenuState::handleEvent(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            _context.states->pushState(std::make_unique<GameState>(_context), false);
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            _context.states->pushState(std::make_unique<SettingsState>(_context));
        }
    }
}

void MenuState::update(sf::Time dt) {
    
}

void MenuState::render(sf::RenderWindow& window) {
    window.draw(_text);
}

void MenuState::pause() {

}

void MenuState::resume() {
    
}