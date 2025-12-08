#include "../../include/states/MenuState.hpp"

MenuState::MenuState(Context* context) : _context(context) {}

void MenuState::init() {
    _shape = sf::CircleShape(50.f);
}

void MenuState::handleEvent(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            // TODO: Create game state
            // GameState game = std::make_unique<GameState>(_context);
            // TODO: Change state
            // _context->states->pushState(std::move(game));
        }
    }

}

void MenuState::update(float dt) {

}

void MenuState::render(sf::RenderWindow& window) {
    window.draw(_shape);
}

void MenuState::pause() {

}

void MenuState::resume() {
    
}