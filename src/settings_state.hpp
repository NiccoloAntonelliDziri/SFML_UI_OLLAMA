#pragma once

#include "app.hpp"
#include "state.hpp"

class SettingsState : public State {
    public:
    SettingsState(AppDataRef data) : data(data) {}
    ~SettingsState() = default;

    void init() override;
    void handleInput() override;
    void update(float dt) override;
    void draw(float dt) override;

    private:
    AppDataRef data;
};
