#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

// IMPLEMENTER LA LOGIQUE POUR ALLER A LA LIGNE
// ET BIEN LIMITER CHAQUE LIGNE A LA FIN DE LA BOITE
class TextBox {
    public:
    TextBox() = default;
    TextBox(int size, sf::Vector2f boxSize, sf::Color color, bool selected)
        : currentLine(0), textbox(nullptr), color(color), boxSize(boxSize),
          size(size), position({0, 0}) {
        // textbox contient que la première ligne
        this->lines.push_back(sf::Text());
        this->textLines.push_back(std::ostringstream());
        this->textbox = &lines[0];

        this->textbox->setCharacterSize(this->size);
        this->textbox->setFillColor(this->color);
        this->isSelected = selected;

        this->box.setOutlineColor(sf::Color::Black);
        this->box.setOutlineThickness(1.9f);
        this->box.setFillColor(sf::Color::Transparent);
        this->setSize(this->boxSize);

        this->setLimit(true, 30);

        if (selected)
            this->textbox->setString("_");
        else
            this->textbox->setString("");
    }

    void reset();
    inline void setFont(sf::Font &font) {
        this->font = font;
        this->textbox->setFont(font);
    }
    inline void setPosition(sf::Vector2f pos) {
        this->position = pos;
        this->textbox->setPosition(pos);
        this->box.setPosition(pos);
    }
    inline void setSize(sf::Vector2f boxSize) { this->box.setSize(boxSize); }
    inline void setLimit(bool tof) { this->hasLimit = tof; }
    inline void setLimit(bool tof, int lim) {
        this->hasLimit = tof;
        this->limit = lim;
    }
    inline int getLimit() { return this->limit; }
    void setSelected(bool selected);
    inline std::string getText() { return this->text.str(); }

    // TODO héritage depuis un sf:drawable ou sf::text pour dessiner this
    //  directement avec window.draw(this) et pas l'inverse
    void draw(sf::RenderWindow &window);
    void typedOn(sf::Event input);

    void addLine();

    private:
    sf::RectangleShape box;

    sf::Font font;
    std::vector<sf::Text> lines;
    std::vector<std::ostringstream> textLines;
    int currentLine;
    sf::Text *textbox;
    std::ostringstream text;
    bool isSelected = false;
    bool hasLimit = false;
    int limit;
    sf::Color color;
    sf::Vector2f boxSize;
    int size;
    sf::Vector2f position;

    void inputLogic(int charType);
    void deleteLastChar();
};
