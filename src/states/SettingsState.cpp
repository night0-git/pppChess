#include "../../include/states/SettingsState.hpp"
#include "../../include/core/StateManager.hpp"

SettingsState::SettingsState(Context& context) : State(context), _text(_font, "Settings") {}

void SettingsState::init() {

}

void SettingsState::handleEvent(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            _context.states->popState();
        }
    }
}

void SettingsState::update(sf::Time dt) {

}

void SettingsState::render(sf::RenderWindow& window) {
    window.draw(_text);
}

void SettingsState::pause() {

}

void SettingsState::resume() {

}