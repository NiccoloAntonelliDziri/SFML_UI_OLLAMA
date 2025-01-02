#include "multiline_text.hpp"

void MultilineText::draw(sf::RenderTarget &target,
                         sf::RenderStates states) const {

    states.transform *= getTransform();
    // no texture
    states.texture = nullptr;

    // Draw all the lines
    for (auto line : this->lines) {
        target.draw(line, states);
    }
}

void MultilineText::write(const std::string &text) {
    this->text = text;
    this->lines.clear();

    int charCounter = 0;
    int lineCounter = 0;
    std::string line = "";
    for (char c : text) {
        if (charCounter == this->numberCharacterLimit || c == '\n') {
            sf::Text t;
            t.setFont(this->font);
            t.setString(line);
            t.setCharacterSize(this->characterSize);
            t.setFillColor(this->color);
            // The 2 is for the spacing between lines
            t.setPosition(this->getPosition().x,
                          this->getPosition().y +
                              lineCounter * (this->characterSize + 2));
            this->lines.push_back(t);
            lineCounter++;
            line = "";
            charCounter = 0;
        }
        if (c != '\n') {
            line += c;
            charCounter++;
        }
    }
    // Add the last line
    if (line != "") {
        sf::Text t;
        t.setFont(this->font);
        t.setString(line);
        t.setCharacterSize(this->characterSize);
        t.setFillColor(this->color);
        // The 2 is for the spacing between lines
        t.setPosition(this->getPosition().x,
                      this->getPosition().y +
                          lineCounter * (this->characterSize + 2));
        this->lines.push_back(t);
    }
}

void MultilineText::setFont(const sf::Font &font) {
    this->font = font;
    for (auto &line : this->lines) {
        line.setFont(font);
    }
}
void MultilineText::setCharacterSize(int characterSize) {
    this->characterSize = characterSize;
    for (auto &line : this->lines) {
        line.setCharacterSize(characterSize);
    }
}
void MultilineText::setColor(const sf::Color &color) {
    this->color = color;
    for (auto &line : this->lines) {
        line.setFillColor(color);
    }
}

sf::FloatRect MultilineText::getGlobalBounds() const {
    sf::FloatRect bounds;
    for (auto line : this->lines) {
        bounds.width = std::max(bounds.width, line.getGlobalBounds().width);
        bounds.height += line.getGlobalBounds().height;
    }
    return bounds;
}
