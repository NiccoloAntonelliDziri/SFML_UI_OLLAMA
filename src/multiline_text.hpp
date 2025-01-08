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
        : font(font), numberCharacterLimit(numberCharacterLimit),
          characterSize(characterSize) {

        this->color = sf::Color::Black;
        this->lineSpacing = 0;
        this->text = "";
        this->position = sf::Vector2f(0, 0);
    }

    // It is pretty when the font is monospaced because the text is aligned
    // correctly.
    void write(const std::string &text);

    void setFont(const sf::Font &font);
    void setCharacterSize(int characterSize);
    void setLineSpacing(int lineSpacing);
    void setColor(const sf::Color &color);
    void setNumberCharacterLimit(int numberCharacterLimit);
    void setPosition(const sf::Vector2f &position);
    void setPosition(float x, float y);

    sf::FloatRect getGlobalBounds() const;

    inline sf::Font getFont() const { return this->font; }
    inline int getCharacterSize() const { return this->characterSize; }
    inline sf::Color getColor() const { return this->color; }
    inline int getNumberCharacterLimit() const {
        return this->numberCharacterLimit;
    }
    inline std::string getText() const { return this->text; }
    inline sf::Vector2f getPosition() const { return this->position; }

    // Operator << is converted to a string
    std::ostream &operator<<(std::ostream &os) {
        os << this->text;
        return os;
    }

    friend class ChatMessages;

    private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f position;

    protected:
    sf::Font font;
    sf::Color color;

    int numberCharacterLimit;

    std::string text;
    std::vector<sf::Text> lines;

    int characterSize;
    int lineSpacing;
};

// This class is a MultilineText that can be scrolled.
// Aka not showing all the text at once.
// scroll is automatic
// scrollUp and scrollDown are manual
class ScrollTextInPlace : public MultilineText {
    public:
    ScrollTextInPlace() = default;
    ScrollTextInPlace(
        const sf::Font &font,
        int numberCharacterLimit = cst.get<int>("maxNumberCharacterLimit"),
        int characterSize = cst.get<int>("fontSize"))
        : MultilineText(font, numberCharacterLimit, characterSize) {
        this->maxLinesToDisplay = -1; // -1 means all lines
        this->offset = 0;
    }
    // This function is the same as write, but not write all the lines depending
    // on the maxLinesToDisplay
    void write(const std::string &text);

    void scrollUp();
    void scrollDown();

    // When set to -1, it will display all the lines
    inline void setMaxLinesToDisplay(int maxLinesToDisplay) {
        this->maxLinesToDisplay = maxLinesToDisplay;
    }
    inline int getMaxLinesToDisplay() const { return this->maxLinesToDisplay; }

    private:
    int maxLinesToDisplay;
    int offset;
};

class InputBox : public ScrollTextInPlace {
    public:
    InputBox() = default;
    InputBox(const sf::Font &font,
             int numberCharacterLimit = cst.get<int>("maxNumberCharacterLimit"),
             int characterSize = cst.get<int>("fontSize"))
        : ScrollTextInPlace(font, numberCharacterLimit, characterSize) {

        this->selected = false;
    }

    void typedOn(sf::Event input);
    inline void setSelected(bool selected) { this->selected = selected; }
    inline bool isSelected() const { return this->selected; }

    private:
    bool selected;
};

class MessageBox : public MultilineText {
    public:
    MessageBox() = default;
    MessageBox(
        const sf::Font &font, std::string role,
        int numberCharacterLimit = cst.get<int>("maxNumberCharacterLimit"),
        int characterSize = cst.get<int>("fontSize"))
        : MultilineText(font, numberCharacterLimit, characterSize), role(role) {
    }

    void write(const std::string &text);

    // not the same scrollUp or scrollDown as in ScrollTextInPlace

    void scrollUp();
    void scrollDown();

    inline std::string getRole() const { return this->role; }
    inline void setRole(const std::string role) { this->role = role; }

    private:
    std::string role;
};

class ChatMessages : public std::vector<MessageBox>,
                     public sf::Drawable,
                     public sf::Transformable {
    public:
    ChatMessages() = default;
    ChatMessages(const std::initializer_list<MessageBox> &messages) {
        for (MessageBox message : messages) {
            this->push_back(message);
        }
    }
    ~ChatMessages() = default;

    void scrollUp();
    void scrollDown();

    private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
