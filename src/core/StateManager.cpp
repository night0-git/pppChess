#include "../../include/core/StateManager.hpp"

StateManager::StateManager() : _isRemoving(false), _isAdding(false), _isReplacing(false) {}

State* StateManager::currentState() {
    if(_states.empty()) {
        return nullptr;
    }
    return (_states.top()).get();
}

void StateManager::pushState(std::unique_ptr<State> newState, bool isReplacing) {
    _isAdding = true;
    _isReplacing = isReplacing;
    _newState = std::move(newState);
}

void StateManager::popState() {
    _isRemoving = true;
}

void StateManager::processStateChanges() {
    if (_isRemoving && !_states.empty()) {
        _states.pop();
        if (!_states.empty()) {
            _states.top()->resume();
        }
        _isRemoving = false;
    }

    if (_isAdding) {
        if (!_states.empty()) {
            if (_isReplacing) {
                _states.pop();
            }
            else {
                _states.top()->pause();
            }
        }
        _states.push(std::move(_newState));
        _states.top()->init();
        _isAdding = false;
    }
}

