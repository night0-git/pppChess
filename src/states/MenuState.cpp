#include "../../include/states/MenuState.hpp"
#include "../../include/states/GameState.hpp"
#include "../../include/states/SettingsState.hpp"
#include "../../include/core/StateManager.hpp"
#include "../../include/ui/components/Component.hpp"
#include "../../include/ui/components/Button.hpp"
#include "../../include/ui/components/Panel.hpp"

MenuState::MenuState(Context& context) : State(context),
_menu({500, 150}, 20) {
    _menu.setPosition({200, 200});

    auto banner = std::make_unique<ui::Panel>(_banner);
    _menu.addComponent(std::move(banner), true);

    auto start = std::make_unique<ui::Button>(sf::Vector2f{500, 150}, 50, "Start game", _font);
    start->setCallback([this]() {
        _context.states->pushState(std::make_unique<GameState>(_context), false);
    });
    _menu.addComponent(std::move(start));
    auto settings = std::make_unique<ui::Button>(sf::Vector2f{500, 300}, 50, "Settings", _font);
    settings->setCallback([this]() {
        _context.states->pushState(std::make_unique<SettingsState>(_context), false);
    });
    _menu.addComponent(std::move(settings));

    // 1. Add a standard "Load Game" button
    auto load = std::make_unique<ui::Button>(sf::Vector2f{500, 150}, 50, "Load Game", _font);
    _menu.addComponent(std::move(load));

    // 2. Create a Nested Horizontal Container (e.g., Difficulty Selector)
    // We set its unit size to smaller buttons ({150, 100})
    auto difficultyContainer = std::make_unique<ui::HorizontalContainer>(sf::Vector2f{250, 300}, 10); 
    difficultyContainer->setSpacing(20);

    // Add "Easy" button to horizontal panel
    auto easy = std::make_unique<ui::Button>(sf::Vector2f{150, 100}, 50, "Easy", _font);
    difficultyContainer->addComponent(std::move(easy));

    // Add "Hard" button to horizontal panel
    auto hard = std::make_unique<ui::Button>(sf::Vector2f{150, 100}, 50, "Hard", _font);
    difficultyContainer->addComponent(std::move(hard));

    // Add the horizontal panel to the main vertical menu.
    // Important: passing 'true' for keepRatio so it calculates its own height 
    // instead of being forced to the vertical panel's unit height (150).
    _menu.addComponent(std::move(difficultyContainer)); 

    // 3. Add Exit Button
    auto exit = std::make_unique<ui::Button>(sf::Vector2f{500, 150}, 50, "Exit", _font);
    exit->setCallback([this]() { _context.window->close(); });
    _menu.addComponent(std::move(exit));
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