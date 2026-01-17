#include "../../include/states/MenuState.hpp"
#include "../../include/states/GameState.hpp"
#include "../../include/states/SettingsState.hpp"
#include "../../include/core/StateManager.hpp"
#include "../../include/ui/components/Component.hpp"

MenuState::MenuState(Context& context) : State(context),
_menu({1000, 1000}, sf::Color::White) {
    _menu.setSpacing(40);
    _menu.setPosition({200, 200});
    auto start = std::make_unique<ui::Button>(
        sf::Vector2f(0.f, 0.f), 
        sf::Vector2f(500.f, 150.f), 
        "Start game", 
        _font
    );
    start->setCallback([this]() {
        _context.states->pushState(std::make_unique<GameState>(_context), false);
    });
    _menu.addComponent(std::move(start));
    auto settings = std::make_unique<ui::Button>(
        sf::Vector2f(0.f, 0.f), 
        sf::Vector2f(500.f, 150.f), 
        "Settings", 
        _font
    );
    settings->setCallback([this]() {
        _context.states->pushState(std::make_unique<SettingsState>(_context), false);
    });
    _menu.addComponent(std::move(settings));
}

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

    sf::Vector2f mouseWorldPos = _context.window->mapPixelToCoords(sf::Mouse::getPosition());
    _menu.handleEvent(event, *_context.window, mouseWorldPos);
}

void MenuState::update(sf::Time dt) {
    
}

void MenuState::render() {
    _context.window->draw(_menu);
}

void MenuState::pause() {

}

void MenuState::resume() {
    
}