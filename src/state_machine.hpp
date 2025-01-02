#pragma once

#include "state.hpp"
#include <memory>
#include <stack>

using StateRef = std::unique_ptr<State>;

class StateMachine {
    public:
    StateMachine() = default;
    ~StateMachine() = default;

    void addState(StateRef newState, bool isReplacing = true);
    void removeState();
    void ProcessStateChanges();
    StateRef &GetActiveState();

    private:
    std::stack<StateRef> states;
    StateRef newstate;

    bool isremoving;
    bool isadding;
    bool isreplacing;
};
