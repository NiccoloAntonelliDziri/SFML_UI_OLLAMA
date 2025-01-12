#pragma once

#include "app.hpp"
#include "state.hpp"

class MenuState : public State {
    public:
    MenuState(AppDataRef data) : data(data) {}
    ~MenuState() = default;

    void init() override;
    void handleInput() override;
    void update(float dt) override;
    void draw(float dt) override;

    private:
    AppDataRef data;
};
