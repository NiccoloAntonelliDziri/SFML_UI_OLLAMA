#include "textbox.hpp"

void TextBox::reset() {
    std::string t = this->text.str();
    std::string newt = "";
    this->text.str("_");
    this->text << newt;

    this->textbox->setString(this->text.str());
    // Remove all th lines
    this->lines.clear();
    this->textLines.clear();
    this->addLine();
    this->currentLine = 0;
    this->textLines.push_back(std::ostringstream());
    this->textLines[this->currentLine].str("");
}

void TextBox::setSelected(bool selected) {
    this->isSelected = selected;
    if (!selected) {
        std::string t = this->text.str();
        std::string newt = "";
        for (int i = 0; i < t.length(); i++) {
            newt += t[i];
        }
        this->textbox->setString(newt);
    }
}

void TextBox::draw(sf::RenderWindow &window) {
    for (auto elem : this->lines) {
        window.draw(elem);
    }
    window.draw(this->box);
}

void TextBox::typedOn(sf::Event input) {
    if (this->isSelected) {
        int charTyped = input.text.unicode;
        if (charTyped < 128) {
            if (this->hasLimit) {
                if (this->text.str().length() <= this->limit)
                    this->inputLogic(charTyped);
                else if (this->text.str().length() > this->limit &&
                         charTyped == DELETE_KEY)
                    this->deleteLastChar();
            } else {
                this->inputLogic(charTyped);
            }
        }
    }
}
void TextBox::inputLogic(int charType) {
    if (charType != DELETE_KEY && charType != ENTER_KEY &&
        charType != ESCAPE_KEY) {
        this->text << static_cast<char>(charType);
        this->textLines[this->currentLine] << static_cast<char>(charType);
    } else if (charType == DELETE_KEY) {
        if (this->text.str().length() > 0) {
            this->deleteLastChar();
        }
    }
    this->textbox->setString(this->text.str() + "_");
}
void TextBox::deleteLastChar() {
    std::string t = this->text.str();
    std::string newt = "";

    std::string lt = this->textLines[this->currentLine].str();
    std::string newlt = "";
    for (int i = 0; i < t.length() - 1; i++) {
        newt += t[i];
    }
    // Gestion du cas ou le caractère supprimé est le premier d'une nouvelle
    // ligne, dans ce cas il faut supprimer la ligne et passer à celle d'avant.
    // Sauf si on est à la première ligne. Dans ce cas on ne fait rien.
    if (lt.length() == 0 && this->currentLine > 0) {
        this->lines.pop_back();
        this->textLines.pop_back();
        this->currentLine--;
        this->textbox = &this->lines[this->currentLine];
        this->textbox->setString(this->textLines[this->currentLine].str() +
                                 "_");
    } else {
        for (int i = 0; i < lt.length() - 1; i++) {
            newlt += lt[i];
        }
        this->textLines[this->currentLine].str("");
        this->textLines[this->currentLine] << newlt;
        this->textbox->setString(this->textLines[this->currentLine].str() +
                                 "_");
    }

    this->text.str("");
    this->text << newt;
}

void TextBox::addLine() {
    this->lines.push_back(sf::Text());
    this->currentLine++;
    this->textbox = &this->lines[this->currentLine];
    this->textbox->setCharacterSize(this->size);
    this->textbox->setFillColor(this->color);
    this->textbox->setFont(this->font);
    this->textbox->setPosition(this->position.x,
                               this->position.y + this->size + 2);
}
