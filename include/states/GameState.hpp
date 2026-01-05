#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "State.hpp"
#include "../game_logic/ChessGame.hpp"
#include "../ui/views/BoardView.hpp"
#include <memory>

class GameState : public State {
public:
    explicit GameState(Context& context);

public:
    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render() override;

    void pause() override;
    void resume() override;

private:
    ChessGame _game;
    std::shared_ptr<ui::BoardView> _boardView;
};

#endif