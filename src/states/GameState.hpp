#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "State.hpp"
#include "../ui/views/BoardView.hpp"
#include "../game_logic/ChessGame.hpp"
#include <memory>
#include <atomic>
#include <thread>

class GameState : public State {
public:
    explicit GameState(Context& context);
    explicit GameState(Context& context, std::unique_ptr<Player> opponent);
    ~GameState();

public:
    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render() override;

    void pause() override;
    void resume() override;

private:
    std::shared_ptr<ChessGame> _game;
    std::shared_ptr<ui::BoardView> _boardView;

    std::atomic<bool> _isOpponentThinking = false;
    std::thread _opponentThread;
};

#endif