#pragma once

#include "constants.hpp"
#include <SFML/Graphics.hpp>

class MultilineText : public sf::Drawable, public sf::Transformable {
    public:
    MultilineText() = default;
    MultilineText(const sf::Font &font, int numberCharacterLimit = 10,
                  int characterSize = 20)
        : font(font), characterSize(characterSize),
          numberCharacterLimit(numberCharacterLimit) {

        this->color = sf::Color::Black;
    }

    // It is pretty when the font is monospaced because the text is aligned
    // correctly.
    void write(const std::string &text);

    void setFont(const sf::Font &font);
    void setCharacterSize(int characterSize);
    void setColor(const sf::Color &color);

    sf::FloatRect getGlobalBounds() const;

    inline sf::Font getFont() const { return this->font; }
    inline int getCharacterSize() const { return this->characterSize; }
    inline sf::Color getColor() const { return this->color; }
    inline int getNumberCharacterLimit() const {
        return this->numberCharacterLimit;
    }
    inline std::string getText() const { return this->text; }

    private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Font font;
    int characterSize;
    int lineSpacing;
    sf::Color color;

    std::vector<sf::Text> lines;
    std::string text;

    int numberCharacterLimit;
};
