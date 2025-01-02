#include "state_machine.hpp"

void StateMachine::addState(StateRef newState, bool isReplacing) {
    this->isadding = true;
    this->isreplacing = isReplacing;
    this->newstate = std::move(newState);
}

void StateMachine::removeState() { this->isremoving = true; }

void StateMachine::ProcessStateChanges() {
    if (this->isremoving && !this->states.empty()) {
        this->states.pop();

        if (!this->states.empty())
            this->states.top()->resume();

        this->isremoving = false;
    }
    if (this->isadding) {
        if (!this->states.empty()) {
            if (this->isreplacing)
                this->states.pop();
            else
                this->states.top()->resume();
        }

        this->states.push(std::move(this->newstate));
        this->states.top()->init();
        this->isadding = false;
    }
}
StateRef &StateMachine::GetActiveState() { return this->states.top(); }
