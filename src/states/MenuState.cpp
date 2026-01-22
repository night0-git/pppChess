#include "../../include/states/MenuState.hpp"
#include "../../include/states/GameState.hpp"
#include "../../include/states/SettingsState.hpp"
#include "../../include/core/StateManager.hpp"
#include "../../include/core/LayoutManager.hpp"
#include "../../include/ui/components/Component.hpp"
#include "../../include/ui/components/Button.hpp"
#include "../../include/ui/components/Panel.hpp"

MenuState::MenuState(Context& context) : State(context),
_menu(_buttonSize, _padding) {
    sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(_context.window->getSize()));
    _context.window->setView(sf::View(visibleArea));

    auto play = std::make_unique<ui::Button>(_buttonSize, 50, "PLAY", _font);
    play->setCallback([this]() {
        _context.states->pushState(std::make_unique<GameState>(_context));
    });
    _menu.addComponent(std::move(play));

    auto settings = std::make_unique<ui::Button>(_buttonSize, 50, "SETTINGS", _font);
    settings->setCallback([this]() {
        _context.states->pushState(std::make_unique<SettingsState>(_context));
    });
    _menu.addComponent(std::move(settings));

    auto quit = std::make_unique<ui::Button>(_buttonSize, 50, "QUIT", _font);
    quit->setCallback([this]() { _context.window->close(); });
    _menu.addComponent(std::move(quit));

    _menu.setPosition(_context.layoutManager->calculatePosition(Anchor::Center, _menu.getSize()));
}

void MenuState::init() {

}

void MenuState::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Resized>()) {
        sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(_context.window->getSize()));
        _context.window->setView(sf::View(visibleArea));
        _menu.setPosition(_context.layoutManager->calculatePosition(Anchor::Center, _menu.getSize()));
    }

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            _context.states->pushState(std::make_unique<SettingsState>(_context));
        }
    }

    sf::Vector2f mouseWorldPos = _context.window->mapPixelToCoords(sf::Mouse::getPosition());
    _menu.handleEvent(event, *_context.window, mouseWorldPos);
}

void MenuState::update(sf::Time dt) {
    
}

void MenuState::render() {
    _context.window->clear(sf::Color(30, 30, 30));
    _context.window->draw(_menu);
}

void MenuState::pause() {

}

void MenuState::resume() {
    sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(_context.window->getSize()));
    _context.window->setView(sf::View(visibleArea));
}