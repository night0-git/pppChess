#include "GameState.hpp"
#include "MenuState.hpp"
#include "SettingsState.hpp"
#include "../core/StateManager.hpp"
#include "../core/LayoutManager.hpp"
#include "../player/RemotePlayer.hpp"
#include "../player/BotPlayer.hpp"
#include "../game_logic/Board.hpp"
#include <iostream>

GameState::GameState(Context& context)
: State(context), _game(std::make_shared<ChessGame>(PieceColor::White)),
_boardView(std::make_shared<ui::BoardView>(*(context.textures), *(context.soundPlayer), _game->board()))
{
    // Because setSize() does not yet manage pieceviews so we call it first
    _boardView->setSize({_context.window->getSize().y * 0.85f, _context.window->getSize().y * 0.85f});
    _boardView->setPosition(_context.layoutManager->calculatePosition(Anchor::Center, _boardView->getSize(), _boardView->getOrigin()));
    init();
};

GameState::GameState(Context& context, std::unique_ptr<Player> opponent)
: State(context), _game(std::make_shared<ChessGame>(std::move(opponent), PieceColor::White)),
_boardView(std::make_shared<ui::BoardView>(*(context.textures), *(context.soundPlayer), _game->board()))
{
    // Because setSize() does not yet manage pieceviews so we call it first
    _boardView->setSize({_context.window->getSize().y * 0.85f, _context.window->getSize().y * 0.85f});
    _boardView->setPosition(_context.layoutManager->calculatePosition(Anchor::Center, _boardView->getSize(), _boardView->getOrigin()));
    init();

    // Start connection for online mode
    if (_game->nonLocalOpponent() == std::type_index(typeid(RemotePlayer))) {
        _context.socket->setBlocking(true);
        _context.listener->setBlocking(false);
        _opponentAddr = dynamic_cast<RemotePlayer*>(_game->opponent())->address();
        // Start the listener if there is no listener to connect to yet
        if (_context.socket->connect(_opponentAddr.value(), 5000, sf::seconds(0.2)) != sf::Socket::Status::Done) {
            _isFirstOnlinePlayer = true;
            if (_context.listener->listen(5000) != sf::Socket::Status::Done) {
                throw std::runtime_error("Cannot connect to port 5000.");
            }
        }
        else {
            _connectionEstablished = true;
            // Change color because the first player claimed the default color already
            _game->changeLocalColor();

            _boardView->flip();
        }
    }
};

GameState::~GameState() {
    if (_opponentThread.joinable()) {
        _opponentThread.join();
    }
}

void GameState::init() {
    _game->addBoardObserver(_boardView);
    _game->addBoardObserver(_game);
    _context.textures->load(ui::TextureId::WPawn, "./assets/pieces/WPawn.png");
    _context.textures->load(ui::TextureId::BPawn, "./assets/pieces/BPawn.png");
    _context.textures->load(ui::TextureId::WRook, "./assets/pieces/WRook.png");
    _context.textures->load(ui::TextureId::BRook, "./assets/pieces/BRook.png");
    _context.textures->load(ui::TextureId::WKnight, "./assets/pieces/WKnight.png");
    _context.textures->load(ui::TextureId::BKnight, "./assets/pieces/BKnight.png");
    _context.textures->load(ui::TextureId::WBishop, "./assets/pieces/WBishop.png");
    _context.textures->load(ui::TextureId::BBishop, "./assets/pieces/BBishop.png");
    _context.textures->load(ui::TextureId::WQueen, "./assets/pieces/WQueen.png");
    _context.textures->load(ui::TextureId::BQueen, "./assets/pieces/BQueen.png");
    _context.textures->load(ui::TextureId::WKing, "./assets/pieces/WKing.png");
    _context.textures->load(ui::TextureId::BKing, "./assets/pieces/BKing.png");
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
        if (_game->status() != GameStatus::Active) {
            return false;
        }
        // This allows the player interact with the board without
        // making a move if it's not their turn in non local mode
        if (_game->nonLocalOpponent() && !_game->isLocalMove()) {
            return false;
        }
        // Prevent player from moving before finding an opponent
        if (_game->nonLocalOpponent() == std::type_index(typeid(RemotePlayer))
        && !_connectionEstablished) {
            return false;
        }
        if (_game->attemptMove(move)) {
            if (_game->nonLocalOpponent() == std::type_index(typeid(RemotePlayer))) {
                sf::Packet packet;
                packet << move;
                // This will block the program, may consider making _socket non blocking
                if (_context.socket->send(packet) != sf::Socket::Status::Done) {
                    std::cerr << "Cannot send packet.";
                }
            }
            return true;
        }
        return false;
    };
    // This call will start connecting _boardView to _board
    _game->reset();
}

void GameState::handleEvent(const sf::Event& event) {
    // Manage layout
    if (event.is<sf::Event::Resized>()) {
        sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(_context.window->getSize()));
        _context.window->setView(sf::View(visibleArea));
        _boardView->setPosition(_context.layoutManager->calculatePosition(Anchor::Center, _boardView->getSize(), _boardView->getOrigin()));
    }

    sf::Vector2f mouseWorldPos = _context.window->mapPixelToCoords(sf::Mouse::getPosition(*_context.window));

    bool wasLocalMove = _game->isLocalMove();
    _boardView->handleEvent(event, *(_context.window), mouseWorldPos);
    // Rotate the board if a valid move has been made
    if (_game->isLocalMove() != wasLocalMove && !_game->nonLocalOpponent()) {
        _boardView->flip();
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
    // Handle mouse cursor
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
        // The game has ended just now
        if (!_gameEnded) {
            _gameEnded = true;
            if (status == GameStatus::WhiteWon) {
                std::cerr << "White won!\n";
            }
            else if (status == GameStatus::BlackWon) {
                std::cerr << "Black won!\n";
            }
            else {
                std::cerr << "Draw!\n";
            }
        }
        _boardView->update(dt);
        return;
    }
    
    // Try to establish a connection in multiplayer mode
    if (_game->nonLocalOpponent() == std::type_index(typeid(RemotePlayer)) && !_connectionEstablished) {
        if (_isFirstOnlinePlayer) {
            std::cerr << "Listening for connection...";
            if (_context.listener->accept(*(_context.socket)) == sf::Socket::Status::Done) {
                std::cout << "Found opponent!\n";
                _connectionEstablished = true;
                _context.listener->close();
            }
        }
        else if (_context.socket->connect(_opponentAddr.value(), 5000) == sf::Socket::Status::Done) {
            _connectionEstablished = true;
        }
    }
    
    if (_connectionEstablished && !_context.socket->isBlocking()) {
        _context.socket->setBlocking(true);
    }

    // Non local player to move
    if (!_game->isLocalMove() && !_isOpponentThinking && _game->nonLocalOpponent()) {
        _isOpponentThinking = true;

        // Join the previous turn before joining the new one
        if (_opponentThread.joinable()) {
            _opponentThread.join();
        }
        
        _opponentThread = std::thread([this]() {
            _game->opponentMove();
            _isOpponentThinking = false;
        });
    }

    _boardView->update(dt);
}

void GameState::render() {
    _context.window->clear(sf::Color(30, 30, 30));
    _context.window->draw(*_boardView);
}

void GameState::pause() {
    // Stop the timer, music...?
}

void GameState::resume() {
    // ...
}