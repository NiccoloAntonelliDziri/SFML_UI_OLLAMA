#include "menu_state.hpp"

#include "asset_manager.hpp"

void MenuState::init() {}
void MenuState::handleInput() {
    sf::Event event;
    while (this->data->window.pollEvent(event)) {
        if (this->data->input.isWindowClosed(event)) {
            this->data->window.close();
        }
    }
}
void MenuState::update(float dt) {}
void MenuState::draw(float dt) {}
