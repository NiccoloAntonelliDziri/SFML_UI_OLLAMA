#pragma once

#include <SFML/Graphics.hpp>

class InputManager {
    public:
    InputManager() = default;
    ~InputManager() = default;

    // Retourne la position de la souris dans la fenêtre
    inline sf::Vector2i getMousePosition(sf::RenderWindow &window) const {
        return sf::Mouse::getPosition(window);
    }

    // Retourne `true` si `sprite` est clocké avec le `button` de la souris
    bool isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button,
                         sf::Event &event, sf::RenderWindow &window);

    // Retourne `true` si `button` est clické dans le rectange `area`
    bool isMouseClickedInArea(sf::IntRect area, sf::Mouse::Button button,
                              sf::Event &event, sf::RenderWindow &window);

    // Retourne `true` si `button` est clické en dehors du rectange `area`
    bool isMouseClickedOutsideArea(sf::IntRect area, sf::Mouse::Button button,
                                   sf::Event &event, sf::RenderWindow &window);

    bool isMouseInArea(sf::IntRect area, sf::RenderWindow &window);

    // Retourne `true` si une action permettant
    // de fermer l'application est déclanchée.
    bool isWindowClosed(sf::Event &event);

    private:
    bool lockMouseClicked;
};
