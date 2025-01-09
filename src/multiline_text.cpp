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
    this->text = this->tabToSpaces(text);

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
            // never begin a line with a space
            if (charCounter == 0 && c == ' ') {
                continue;
            }
            line += c;
            charCounter++;
        }
    }
    // Add the last line
    if (line != "") {
        sf::Text t;
        t.setFont(this->font);
        // remove beginning space
        if (line[0] == ' ') {
            line = line.substr(1);
        }
        t.setString(line);
        t.setCharacterSize(this->characterSize);
        t.setFillColor(this->color);
        t.setPosition(this->getPosition().x,
                      this->getPosition().y +
                          lineCounter *
                              (this->characterSize + this->lineSpacing));
        this->lines.push_back(t);
    }

    this->numberLines = this->lines.size();
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

std::string MultilineText::tabToSpaces(const std::string &text) const {
    std::string result = "";
    for (char c : text) {
        if (c == '\t') {
            result += "    ";
        } else {
            result += c;
        }
    }
    return result;
}

/* ScrollTextInPlace */

void ScrollTextInPlace::write(const std::string &text) {
    this->text = this->tabToSpaces(text);

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
            // never begin a line with a space
            if (charCounter == 0 && c == ' ') {
                continue;
            }
            line += c;
            charCounter++;
        }
    }
    // Add the last line
    if (line != "") {
        sf::Text t;
        t.setFont(this->font);
        // remove beginning space
        if (line[0] == ' ') {
            line = line.substr(1);
        }
        t.setString(line);
        t.setCharacterSize(this->characterSize);
        t.setFillColor(this->color);
        t.setPosition(this->getPosition().x,
                      this->getPosition().y +
                          lineCounter *
                              (this->characterSize + this->lineSpacing));
        this->lines.push_back(t);
    }

    this->numberLines = this->lines.size();

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
    if (this->getNumberLines() <= this->maxLinesToDisplay) {
        return;
    }
    if (this->getNumberLines() - offset <= this->maxLinesToDisplay) {
        return;
    }

    this->offset++;
    this->write(this->text);
}
void ScrollTextInPlace::scrollDown() {
    if (this->maxLinesToDisplay == -1) {
        return;
    }
    if (this->getNumberLines() <= this->maxLinesToDisplay) {
        return;
    }
    if (this->offset <= 0) {
        return;
    }
    this->offset--;
    this->write(this->text);
}

/* InputBox */

void InputBox::typedOn(sf::Event input) {
    if (!this->selected) {
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
            this->selected = false;
            // Peut-être un jour implémenter les flèches pour se déplacer dans
            // le texte Ou des caractères spéciaux ou des raccourcis clavier ou
            // la touche Suppr
        } else {
            this->write(this->text + static_cast<char>(charTyped));
        }
    }
}

/* MessageBox */
void MessageBox::write(const std::string &text) {
    this->text = this->role + ":\n" + this->tabToSpaces(text);

    this->lines.clear();

    int charCounter = 0;
    int lineCounter = 0;
    std::string line = "";
    for (char c : this->text) {
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

            // scroll up so that the last line is always at the bottom

            lineCounter++;
            line = "";
            charCounter = 0;
        }
        if (c != '\n') {
            // never begin a line with a space
            if (charCounter == 0 && c == ' ') {
                continue;
            }
            line += c;
            charCounter++;
        }
    }
    // Add the last line
    if (line != "") {
        sf::Text t;
        t.setFont(this->font);
        // remove beginning space
        if (line[0] == ' ') {
            line = line.substr(1);
        }
        t.setString(line);
        t.setCharacterSize(this->characterSize);
        t.setFillColor(this->color);
        t.setPosition(this->getPosition().x,
                      this->getPosition().y +
                          lineCounter *
                              (this->characterSize + this->lineSpacing));
        this->lines.push_back(t);
    }
    if (this->lines.size() < 1) {
        std::cout << "No lines input" << std::endl;
        return;
    }

    this->numberLines = this->lines.size();

    // Déplacement des lignes pour que la dernière soit toujours en bas alignée
    for (auto &line : this->lines) {
        line.move(0, (1 - (int)this->lines.size()) *
                         (this->characterSize + this->lineSpacing));
    }
}

void MessageBox::scrollUp() {
    for (auto &line : this->lines) {
        line.move(0, -this->characterSize - this->lineSpacing);
    }
}

void MessageBox::scrollDown() {
    for (auto &line : this->lines) {
        line.move(0, this->characterSize + this->lineSpacing);
    }
}

/* ChatBox */

void ChatBox::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    // no texture
    states.texture = nullptr;

    // Draw all the messages
    for (auto i = this->begin(); i != this->end(); i++) {
        target.draw(*i, states);
    }
}
void ChatBox::scrollUp(int begin, int end) {
    if (end == -1) {
        end = this->size();
    }
    for (int i = begin; i < end; i++) {
        this->at(i).scrollUp();
    }
}
void ChatBox::scrollDown(int begin, int end) {
    if (end == -1) {
        end = this->size();
    }
    for (int i = begin; i < end; i++) {
        this->at(i).scrollDown();
    }
}
void ChatBox::addMessage(const MessageBox &message) {
    int msgLines = message.getNumberLines();
    this->totalNumberLines += msgLines;
    if (msgLines == 0) {
        return;
    }
    for (int i = 0; i < msgLines; i++) {
        this->scrollUp();
    }
    this->push_back(message);
}
