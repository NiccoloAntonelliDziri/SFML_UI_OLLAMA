#include "chat_selection_state.hpp"

void ChatSelectionState::init() { this->newChat("Default"); }

void ChatSelectionState::handleInput() {
    sf::Event event;
    while (this->data->window.pollEvent(event)) {
        if (this->data->input.isWindowClosed(event)) {
            this->data->window.close();
        }

        if (this->data->input.isSpriteClicked(this->moreButtons,
                                              sf::Mouse::Left, event,
                                              this->data->window) &&
            this->moreButtonActive) {

            this->moreButtonActive = false;
            this->newChat("");
            // std::cout << "More button clicked" << std::endl;
            this->conversations[""].setSelected(true);
            this->conversations[""].setColor(
                cst.get<sf::Color>("textColorNotActive"));
            this->conversations[""].write(cst["Name the chat"]);
        }

        if (event.type == sf::Event::TextEntered) {

            // If end of the line is reached, do not write allow only backspace
            if (this->conversations[""].getText().size() >=
                    cst.get<int>("maxNumberCharacterConversationInput") &&
                event.text.unicode != 8) {
                continue;
            }

            this->conversations[""].typedOn(event);
            this->conversations[""].setColor(cst.get<sf::Color>("textColor"));
            if (this->conversations[""].getText() == cst["Name the chat"]) {
                this->conversations[""].write("");
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) &&
                !this->conversations[""].getText().empty()) {

                // Copy the name of the chat to the map
                this->data->chats.addChat(this->conversations[""].getText());
                this->newChat(this->conversations[""].getText());
                this->conversations.erase("");

                this->moreButtonActive = true;
            }
        }
    }
}
void ChatSelectionState::update(float dt) {
    if (this->data->chats.getNumberOfChats() > 5) {
        this->moreButtonActive = false;
    }
}

void ChatSelectionState::draw(float dt) {
    // Iterate trough the maps
    for (auto &conv : this->conversations) {
        this->data->window.draw(this->convBackgrounds[conv.first]);
        this->data->window.draw(conv.second);
        this->data->window.draw(this->deleteButtons[conv.first]);
    }
    if (this->moreButtonActive)
        this->data->window.draw(this->moreButtons);
}

void ChatSelectionState::newChat(std::string name) {
    this->conversations[name] =
        InputBox(this->data->assets.getFont(cst["fontName"]));
    this->deleteButtons[name] =
        sf::Sprite(this->data->assets.getTexture(cst["deleteButtonName"]));
    this->moreButtons =
        sf::Sprite(this->data->assets.getTexture(cst["moreButtonName"]));
    this->convAreas[name] = sf::IntRect();
    this->convBackgrounds[name] = sf::RectangleShape();

    // Style
    this->convBackgrounds[name].setSize(
        cst.get<sf::Vector2f>("conversationBoxSize"));
    this->convBackgrounds[name].setFillColor(sf::Color::Transparent);
    this->convBackgrounds[name].setOutlineColor(
        cst.get<sf::Color>("inputBoxOutlineColor"));
    this->convBackgrounds[name].setOutlineThickness(
        cst.get<float>("inputBoxThickness"));

    sf::Vector2u screen = this->data->window.getSize();
    sf::Vector2u convSize = cst.get<sf::Vector2u>("conversationBoxSize");

    int verticalOffset = 5;

    // Scale the Buttons
    this->moreButtons.scale(0.1f, 0.1f);
    this->data->assets.setTextureSmooth(cst["moreButtonName"], true);
    this->deleteButtons[name].scale(0.1f, 0.1f);
    this->data->assets.setTextureSmooth(cst["deleteButtonName"], true);

    // Position des conversations, des boutons delete et des boutons more
    this->convBackgrounds[name].setPosition(
        cst.get<sf::Vector2f>("conversationTopLeft") +
        sf::Vector2f(0, (this->data->chats.getNumberOfChats() - 1) *
                            ((float)screen.y / 6)));
    this->conversations[name].setPosition(
        sf::Vector2f(7, 14) + cst.get<sf::Vector2f>("conversationTopLeft") +
        sf::Vector2f(0, (this->data->chats.getNumberOfChats() - 1) *
                            ((float)screen.y / 6)));
    this->deleteButtons[name].setPosition(
        sf::Vector2f(convSize.x + 20, verticalOffset) +
        cst.get<sf::Vector2f>("conversationTopLeft") +
        sf::Vector2f(0, (this->data->chats.getNumberOfChats() - 1) *
                            ((float)screen.y / 6)));
    this->moreButtons.setPosition(sf::Vector2f(
        (float)screen.x / 2 - this->moreButtons.getGlobalBounds().width / 2,
        20 + verticalOffset +
            this->data->chats.getNumberOfChats() * ((float)screen.y / 6)));

    this->convAreas[name] = {
        static_cast<int>(this->convBackgrounds[name].getPosition().x),
        static_cast<int>(this->convBackgrounds[name].getPosition().y),
        static_cast<int>(this->convBackgrounds[name].getSize().x),
        static_cast<int>(this->convBackgrounds[name].getSize().y)};

    // write the name of the chat
    this->conversations[name].write(name);
}

void ChatSelectionState::deleteChat(std::string name) {
    this->conversations.erase(name);
    this->deleteButtons.erase(name);
    this->convAreas.erase(name);
    this->convBackgrounds.erase(name);
}
