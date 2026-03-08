#include "SettingsState.hpp"
#include "../core/StateManager.hpp"
#include "../core/LayoutManager.hpp" 
#include "../ui/components/Button.hpp"
#include "../ui/components/Slider.hpp"
#include <memory>

SettingsState::SettingsState(Context& context) : State(context) {
    _title.setFillColor(sf::Color::White);
    _title.setPosition(_context.layoutManager->calculatePosition(Anchor::Top, _title.getGlobalBounds().size, 100));

    auto volumeText = std::make_unique<ui::Button>(sf::Vector2f(400, 100), 50, "Volume", _font);
    volumeText->setDepthOffset(0);
    volumeText->setBoxColor(sf::Color(30, 30, 30));
    volumeText->setTextColor(sf::Color::White);
    
    auto volumeSlider = std::make_unique<ui::Slider>(sf::Vector2f(400, 100), sf::Color(143, 143, 143));
    
    auto volume = std::make_unique<ui::HorizontalContainer>(sf::Vector2f((_unitSize.x - _horizontalSpacing) / 2, _unitSize.y), _horizontalSpacing); 
    volume->addComponent(std::move(volumeText));
    volume->addComponent(std::move(volumeSlider));

    _settingsMenu.addComponent(std::move(volume));
    _settingsMenu.setPosition(_context.layoutManager->calculatePosition(Anchor::Center, _settingsMenu.getSize()));
}

void SettingsState::init() {

}

void SettingsState::handleEvent(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            _context.states->popState();
        }
    }

    sf::Vector2f mouseWorldPos = _context.window->mapPixelToCoords(sf::Mouse::getPosition());
    _settingsMenu.handleEvent(event, *_context.window, mouseWorldPos);
}

void SettingsState::update(sf::Time dt) {
    if ((_settingsMenu.getState() == ui::State::Hovered || _settingsMenu.getState() == ui::State::Pressed) && _context.cursors->hand) {
        _context.window->setMouseCursor(*(_context.cursors->hand));
    } else if (_context.cursors->arrow) {
        _context.window->setMouseCursor(*(_context.cursors->arrow));
    }
}

void SettingsState::render() {
    _context.window->clear(sf::Color(30, 30, 30));
    _context.window->draw(_title);
    _context.window->draw(_settingsMenu);
}

void SettingsState::pause() {

}

void SettingsState::resume() {

}