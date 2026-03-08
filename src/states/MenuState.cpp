#include "MenuState.hpp"
#include "GameState.hpp"
#include "SettingsState.hpp"
#include "../core/StateManager.hpp"
#include "../core/LayoutManager.hpp"
#include "../ui/components/Component.hpp"
#include "../ui/components/Button.hpp"
#include "../ui/components/Panel.hpp"
#include "../player/BotPlayer.hpp"

MenuState::MenuState(Context& context) : State(context) {
    sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(_context.window->getSize()));
    _context.window->setView(sf::View(visibleArea));

    auto playBot = std::make_unique<ui::Button>(_buttonSize, 50, "PLAY BOT", _font);
    playBot->setCallback([this]() {
        _context.states->pushState(std::make_unique<GameState>(_context, std::make_unique<BotPlayer>(3)));
    });
    _menu.addComponent(std::move(playBot));

    auto playLocal = std::make_unique<ui::Button>(_buttonSize, 50, "PLAY LOCAL", _font);
    playLocal->setCallback([this]() {
        _context.states->pushState(std::make_unique<GameState>(_context));
    });
    _menu.addComponent(std::move(playLocal));

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
    if ((_menu.getState() == ui::State::Hovered || _menu.getState() == ui::State::Pressed) && _context.cursors->hand) {
        _context.window->setMouseCursor(*(_context.cursors->hand));
    } else if (_context.cursors->arrow) {
        _context.window->setMouseCursor(*(_context.cursors->arrow));
    }
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