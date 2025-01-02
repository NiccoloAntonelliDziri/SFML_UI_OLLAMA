#pragma once

#include "constants.hpp"
#include <SFML/Graphics.hpp>

class MultilineText : public sf::Drawable, public sf::Transformable {
    public:
    MultilineText() = default;
    MultilineText(sf::Font &font)
        : font(font), fontSize(cst.get<int>("fontSize")),
          color(sf::Color::Black), interlinesSpace(2), lineLimit(107) {

        // Initialised at one line
        this->lines.push_back(sf::Text());
        this->textLines.push_back("");
    }
    ~MultilineText() = default;

    void addChar(char c);

    private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::RectangleShape box;
    std::vector<sf::Text> lines;
    std::vector<std::string> textLines;
    std::string text;

    sf::Font font;
    int fontSize;
    sf::Color color;

    int interlinesSpace;
    int lineLimit;
};
