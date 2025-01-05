#include "input_manager.hpp"

bool InputManager::isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button,
                                   sf::Event &event, sf::RenderWindow &window) {

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == button) {
            sf::Vector2f origin = sprite.getOrigin();
            sf::IntRect tempRect(sprite.getPosition().x - origin.x,
                                 sprite.getPosition().y - origin.y,
                                 sprite.getGlobalBounds().width,
                                 sprite.getGlobalBounds().height);
            // sf::IntRect tempRect(sprite.getPosition().x,
            //                      sprite.getPosition().y,
            //                      sprite.getGlobalBounds().width,
            //                      sprite.getGlobalBounds().height);
            if (tempRect.contains(sf::Mouse::getPosition(window))) {
                return true;
            }
        }
    }
    return false;
}

bool InputManager::isMouseClickedInArea(sf::IntRect area,
                                        sf::Mouse::Button button,
                                        sf::Event &event,
                                        sf::RenderWindow &window) {

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == button) {
            if (area.contains(sf::Mouse::getPosition(window)))
                return true;
        }
    }
    return false;
}

bool InputManager::isMouseClickedOutsideArea(sf::IntRect area,
                                             sf::Mouse::Button button,
                                             sf::Event &event,
                                             sf::RenderWindow &window) {

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == button) {
            if (!area.contains(sf::Mouse::getPosition(window)))
                return true;
        }
    }
    return false;
}

bool InputManager::isWindowClosed(sf::Event &event) {
    if (event.type == sf::Event::Closed)
        return true;
    // possibilité d'ajouter d'autres conditions
    return false;
}
