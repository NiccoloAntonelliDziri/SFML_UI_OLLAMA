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
    inline int getNumberLines() const { return this->numberLines; }

    // Operator << is converted to a string
    std::ostream &operator<<(std::ostream &os) {
        os << this->text;
        return os;
    }

    friend class ChatBox;

    private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f position;

    protected:
    std::string tabToSpaces(const std::string &text) const;

    sf::Font font;
    sf::Color color;

    int numberCharacterLimit;

    std::string text;
    std::vector<sf::Text> lines;
    int numberLines; // number of lines in the text (can be different from
                     // lines.size() because of what is displayed)

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

        this->selected = true;
    }

    void typedOn(sf::Event input);
    inline void setSelected(bool s) { this->selected = s; }
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

    inline std::string getRole() const { return this->role; }
    inline void setRole(const std::string &role) { this->role = role; }

    // not the same scrollUp as in ScrollTextInPlace
    void scrollUp();
    // not the same scrollDown as in ScrollTextInPlace
    void scrollDown();

    private:
    std::string role;
};

// This class is a vector of MessageBox.
// Do not use push_back, use addMessage instead.
// Do not set the position of the ChatBox, it is not used.
class ChatBox : public std::vector<MessageBox>,
                public sf::Drawable,
                public sf::Transformable {
    public:
    ChatBox() : std::vector<MessageBox>(), totalNumberLines(0), offset(0) {
        this->maxLinesToDisplay = cst.get<int>("maxNumberLinesChatToDisplay");
    }
    ~ChatBox() = default;

    // parameter: the range of messages to scroll
    void scrollUp(int begin = 0, int end = -1);
    void scrollDown(int begin = 0, int end = -1);

    // For reading the past messages
    // scrollUpMsg and scrollDownMsg are not the same as scrollUp and scrollDown

    void scrollUpMsg();
    void scrollDownMsg();

    void updateTotalNumberLines();

    // might not be accurate if the ChatBox is not up to date.
    // aka messages have been modified after begin added.
    // Call `calculateTotalNumberLines` to update the total number of lines.
    inline int getTotalNumberLines() const { return this->totalNumberLines; }

    void updateLinesToDraw();

    // Add a message to the ChatBox.
    // Use this function instead of push_back.
    void addMessage(const MessageBox &message);

    private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    int totalNumberLines;
    int startl;
    int endl;
    std::vector<sf::Text> lines;

    int maxLinesToDisplay;

    int offset;
};
