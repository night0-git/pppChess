#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_

#include "../states/State.hpp"
#include <memory>
#include <stack>

class StateManager {
public:
    StateManager();
    State* currentState();

public:
    // isReplacing = true -> replaces the current state
    void pushState(std::unique_ptr<State> newState, bool isReplacing = true);
    void popState();
    void processStateChanges();


private:
    std::stack<std::unique_ptr<State>> _states;
    // Temp storage to process changes at the start of a new loop to prevent crash
    std::unique_ptr<State> _newState;
    bool _isRemoving;
    bool _isAdding;
    bool _isReplacing;
};

struct Context {
    sf::RenderWindow* window;
    StateManager* states;
};

#endif