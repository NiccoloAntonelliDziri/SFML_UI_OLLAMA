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
            t.setPosition(this->getPosition().x,
                          this->getPosition().y +
                              lineCounter *
                                  (this->characterSize + this->lineSpacing));
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
        t.setPosition(this->getPosition().x,
                      this->getPosition().y +
                          lineCounter *
                              (this->characterSize + this->lineSpacing));
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
void MultilineText::setLineSpacing(int lineSpacing) {
    this->lineSpacing = lineSpacing;
    if (this->lines.size() <= 1) {
        return;
    }
    for (int i = 1; i < (int)this->lines.size(); i++) {
        this->lines[i].setPosition(this->lines[i].getPosition().x,
                                   this->lines[i - 1].getPosition().y +
                                       this->characterSize + this->lineSpacing);
    }
}
void MultilineText::setNumberCharacterLimit(int numberCharacterLimit) {
    this->numberCharacterLimit = numberCharacterLimit;
    this->write(this->text);
}

void MultilineText::setPosition(const sf::Vector2f &position) {
    this->position = position;

    int lineCounter = 0;
    for (auto &line : this->lines) {
        line.setPosition(position.x,
                         position.y + lineCounter * (this->characterSize +
                                                     this->lineSpacing));
        lineCounter++;
    }
}

void MultilineText::setPosition(float x, float y) {
    this->setPosition(sf::Vector2f(x, y));
}

/* ScrollTextInPlace */

void ScrollTextInPlace::write(const std::string &text) {
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
            t.setPosition(this->getPosition().x,
                          this->getPosition().y +
                              lineCounter *
                                  (this->characterSize + this->lineSpacing));
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
        t.setPosition(this->getPosition().x,
                      this->getPosition().y +
                          lineCounter *
                              (this->characterSize + this->lineSpacing));
        this->lines.push_back(t);
    }

    // Si il ne faut pas les afficher toutes
    // Update la position des lignes pour qu'elles restent au meme endroit
    if (this->maxLinesToDisplay != -1) {
        int startBeforeOffset =
            std::max(0, (int)this->lines.size() - this->maxLinesToDisplay);
        int start = std::max(0, (int)this->lines.size() -
                                    this->maxLinesToDisplay - this->offset);
        int end = std::min((int)this->lines.size(),
                           startBeforeOffset + this->maxLinesToDisplay -
                               this->offset);

        std::cout << "start: " << start << " end: " << end << std::endl;
        std::cout << "lines.size(): " << this->lines.size() << std::endl;
        std::cout << "maxLinesToDisplay: " << this->maxLinesToDisplay
                  << std::endl;

        // Effacement du début et de la fin
        this->lines.erase(this->lines.begin() + end, this->lines.end());
        this->lines.erase(this->lines.begin(), this->lines.begin() + start);

        for (int i = 0; i < (int)this->lines.size(); i++) {
            this->lines[i].setPosition(
                this->lines[i].getPosition().x,
                this->getPosition().y +
                    i * (this->characterSize + this->lineSpacing));
        }
    }
}

void ScrollTextInPlace::scrollUp() {
    if (this->maxLinesToDisplay == -1) {
        return;
    }
    if (this->lines.size() + this->offset > this->maxLinesToDisplay) {
        return;
    }
    this->offset++;
    this->write(this->text);
    std::cout << "offset: " << this->offset << std::endl;
}
void ScrollTextInPlace::scrollDown() {
    if (this->maxLinesToDisplay == -1) {
        return;
    }
    if (this->offset <= 0) {
        return;
    }
    this->offset--;
    this->write(this->text);
    std::cout << "offset: " << this->offset << std::endl;
}

/* InputBox */

void InputBox::typedOn(sf::Event input) {
    if (!this->isSelected) {
        return;
    }

    int charTyped = input.text.unicode;
    if (charTyped < 128) {
        // 8 is the backspace key
        if (charTyped == 8) {
            if (this->text.length() > 0) {
                this->write(this->text.substr(0, this->text.length() - 1));
            }
        }
        // 13 is the enter key
        else if (charTyped == 13) {
            this->write("");
        }
        // 27 is the escape key
        else if (charTyped == 27) {
            this->isSelected = false;
            // Peut-être un jour implémenter les flèches pour se déplacer dans
            // le texte Ou des caractères spéciaux ou des raccourcis clavier ou
            // la touche Suppr
        } else {
            this->write(this->text + static_cast<char>(charTyped));
        }
    }
}

void InputBox::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    // no texture
    states.texture = nullptr;

    for (auto line : this->lines) {
        target.draw(line, states);
    }
}
