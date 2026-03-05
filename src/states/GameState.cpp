#include "GameState.hpp"
#include "MenuState.hpp"
#include "SettingsState.hpp"
#include "../core/StateManager.hpp"
#include "../core/LayoutManager.hpp"
#include <iostream>

GameState::GameState(Context& context, std::unique_ptr<Player> opponent)
: State(context), _game(std::make_shared<ChessGame>(std::move(opponent), PieceColor::White)),
_boardView(std::make_shared<ui::BoardView>(*(context.textures), *(context.soundPlayer), _game->board()))
{
    _boardView->setSize({_context.window->getSize().y * 0.85f, _context.window->getSize().y * 0.85f});
    _boardView->setPosition(_context.layoutManager->calculatePosition(Anchor::Left, _boardView->getSize()));
};

void GameState::init() {
    _game->addBoardObserver(_boardView);
    _game->addBoardObserver(_game);
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
    _context.sounds->load(ui::SoundId::GameStart, "./assets/sounds/game-start.wav");
    _context.sounds->load(ui::SoundId::GameEnd, "./assets/sounds/game-end.wav");
    _context.sounds->load(ui::SoundId::Move, "./assets/sounds/move-self.wav");
    _context.sounds->load(ui::SoundId::Capture, "./assets/sounds/capture.wav");
    _context.sounds->load(ui::SoundId::Check, "./assets/sounds/move-check.wav");
    _context.sounds->load(ui::SoundId::Illegal, "./assets/sounds/illegal.wav");
    _context.sounds->load(ui::SoundId::Castle, "./assets/sounds/castle.wav");
    _context.sounds->load(ui::SoundId::Promote, "./assets/sounds/promote.wav");
    _context.sounds->load(ui::SoundId::TenSeconds, "./assets/sounds/tenseconds.wav");
    // Connect to _boardView's hook 
    _boardView->_onMoveRequest = [this](const Move& move) {
        if (_game->attemptMove(move)) {
            return true;
        }
        std::cerr << "Invalid move";
        return false;
    };
    _game->reset();
}

void GameState::handleEvent(const sf::Event& event) {
    sf::Vector2f mouseWorldPos = _context.window->mapPixelToCoords(sf::Mouse::getPosition());
    if (_context.window) {
        if (_game->isLocalMove()) {
            _boardView->handleEvent(event, *(_context.window), mouseWorldPos);
        }
    }

    if (event.is<sf::Event::Resized>()) {
        sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(_context.window->getSize()));
        _context.window->setView(sf::View(visibleArea));
        _boardView->setPosition(_context.layoutManager->calculatePosition(Anchor::Left, _boardView->getSize()));
    }

    if (const auto& keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            _context.states->pushState(std::make_unique<SettingsState>(_context));
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            _context.states->popState();
        }
    }
}

void GameState::update(sf::Time dt) {
    if (_boardView->getState() == ui::State::Hovered && _context.cursors->handOpened) {
        _context.window->setMouseCursor(*(_context.cursors->handOpened));
    } else if (_boardView->getState() == ui::State::Pressed && _context.cursors->handClosed) {
        _context.window->setMouseCursor(*_context.cursors->handClosed);
    } else if (_context.cursors->arrow) {
        _context.window->setMouseCursor(*(_context.cursors->arrow));
    }

    // TODO
    GameStatus status = _game->status();
    if (status != GameStatus::Active) {
        if (status == GameStatus::WhiteWon) {
            std::cerr << "White won!\n";
        }
        else if (status == GameStatus::BlackWon) {
            std::cerr << "Black won!\n";
        }
        else {
            std::cerr << "Draw!";
        }
        return;
    }
    
    if (!_game->isLocalMove()) {
        _game->opponentMove();
    }

    _boardView->update(dt);
}

void GameState::render() {
    _context.window->draw(*_boardView);
}

void GameState::pause() {
    // Stop the timer, music...?
}

void GameState::resume() {
    // ...
}