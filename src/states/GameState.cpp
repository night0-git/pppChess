#include "../../include/states/GameState.hpp"
#include "../../include/states/MenuState.hpp"
#include "../../include/states/SettingsState.hpp"
#include "../../include/core/StateManager.hpp"

GameState::GameState(Context& context) : State(context),
_boardView(std::make_shared<ui::BoardView>(*(context.textures), _board)) {}

void GameState::init() {
    _board.addObserver(_boardView);
    _context.textures->load(ui::TextureId::WPawn, "./assets/custom/pieces/WPawn.png");
    _context.textures->load(ui::TextureId::BPawn, "./assets/custom/pieces/BPawn.png");
    _context.textures->load(ui::TextureId::WRook, "./assets/custom/pieces/WRook.png");
    _context.textures->load(ui::TextureId::BRook, "./assets/custom/pieces/BRook.png");
    _context.textures->load(ui::TextureId::WKnight, "./assets/custom/pieces/WKnight.png");
    _context.textures->load(ui::TextureId::BKnight, "./assets/custom/pieces/BKnight.png");
    _context.textures->load(ui::TextureId::WBishop, "./assets/custom/pieces/WBishop.png");
    _context.textures->load(ui::TextureId::BBishop, "./assets/custom/pieces/BBishop.png");
    _context.textures->load(ui::TextureId::WQueen, "./assets/custom/pieces/WQueen.png");
    _context.textures->load(ui::TextureId::BQueen, "./assets/custom/pieces/BQueen.png");
    _context.textures->load(ui::TextureId::WKing, "./assets/custom/pieces/WKing.png");
    _context.textures->load(ui::TextureId::BKing, "./assets/custom/pieces/BKing.png");
    _board.boardInit();
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

void GameState::update(sf::Time dt) {

}

void GameState::render(sf::RenderWindow& window) {
    window.draw(*_boardView);
}

void GameState::pause() {
    // Stop the timer, music...?
}

void GameState::resume() {
    // ...
}