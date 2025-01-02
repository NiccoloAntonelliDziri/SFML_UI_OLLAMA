#include "settings_state.hpp"

#include "asset_manager.hpp"

void SettingsState::init() {}
void SettingsState::handleInput() {
    sf::Event event;
    while (this->data->window.pollEvent(event)) {
        if (this->data->input.isWindowClosed(event)) {
            this->data->window.close();
        }
    }
}
void SettingsState::update(float dt) {}
void SettingsState::draw(float dt) {}
