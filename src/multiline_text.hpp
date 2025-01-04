#pragma once

#include "constants.hpp"
#include <SFML/Graphics.hpp>

class MultilineText : public sf::Drawable, public sf::Transformable {
    public:
    MultilineText() = default;
    MultilineText(
        const sf::Font &font,
        int numberCharacterLimit = cst.get<int>("maxNumberCharacterLimit"),
        int characterSize = cst.get<int>("fontSize"))
        : font(font), characterSize(characterSize),
          numberCharacterLimit(numberCharacterLimit) {

        this->color = sf::Color::Black;
        this->lineSpacing = 0;
    }

    // It is pretty when the font is monospaced because the text is aligned
    // correctly.
    void write(const std::string &text);

    void setFont(const sf::Font &font);
    void setCharacterSize(int characterSize);
    void setLineSpacing(int lineSpacing);
    void setColor(const sf::Color &color);
    void setNumberCharacterLimit(int numberCharacterLimit);

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

    int numberCharacterLimit;

    // Operator << is converted to a string
    std::ostream &operator<<(std::ostream &os) {
        os << this->text;
        return os;
    }

    protected:
    std::string text;
};

class InputBox : public MultilineText {
    public:
    InputBox() = default;
    InputBox(const sf::Font &font,
             int numberCharacterLimit = cst.get<int>("maxNumberCharacterLimit"),
             int characterSize = cst.get<int>("fontSize"))
        : MultilineText(font, numberCharacterLimit, characterSize) {}

    void typedOn(sf::Event input);
    inline void setSelected(bool selected) { this->isSelected = selected; }

    private:
    bool isSelected = false;
};
