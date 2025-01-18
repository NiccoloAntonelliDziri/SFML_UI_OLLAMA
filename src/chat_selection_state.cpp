#include "chat_selection_state.hpp"

void ChatSelectionState::init() {
    // Default est crée avant dans le state ollama
    this->newChat("Cecile");

    if (!this->data->chats.chatExists("Mario"))
        this->data->chats.addChat("Mario");
    this->newChat("Mario");

    if (!this->data->chats.chatExists("Trump"))
        this->data->chats.addChat("Trump");
    this->newChat("Trump");

    if (!this->data->chats.chatExists("granite3-moe"))
        this->data->chats.addChat("granite3-moe");
    this->newChat("granite3-moe");

    if (!this->data->chats.chatExists("RapperGPT"))
        this->data->chats.addChat("RapperGPT");
    this->newChat("RapperGPT");

    if (!this->data->chats.chatExists("Chat5"))
        this->data->chats.addChat("Chat5");
    this->newChat("Chat5");

    sf::Vector2u screen = this->data->window.getSize();
    sf::Vector2u convSize = cst.get<sf::Vector2u>("conversationBoxSize");

    int verticalOffset = 5;

    // Position des conversations, des boutons delete et des boutons
    int compteur = 0;
    for (auto &name : this->data->chats.getChatNames()) {

        // Position des conversations, des boutons delete et des boutons more
        this->convBackgrounds[name].setPosition(
            cst.get<sf::Vector2f>("conversationTopLeft") +
            sf::Vector2f(0, (compteur) * ((float)screen.y / 6)));
        this->conversations[name].setPosition(
            sf::Vector2f(7, 14) + cst.get<sf::Vector2f>("conversationTopLeft") +
            sf::Vector2f(0, (compteur) * ((float)screen.y / 6)));
        this->deleteButtons[name].setPosition(
            sf::Vector2f(convSize.x + 20, verticalOffset) +
            cst.get<sf::Vector2f>("conversationTopLeft") +
            sf::Vector2f(0, (compteur) * ((float)screen.y / 6)));

        this->convAreas[name] = {
            static_cast<int>(this->convBackgrounds[name].getPosition().x),
            static_cast<int>(this->convBackgrounds[name].getPosition().y),
            static_cast<int>(this->convBackgrounds[name].getSize().x),
            static_cast<int>(this->convBackgrounds[name].getSize().y)};

        compteur++;
    }

    // If some messages are sent in the chat, activate the delete Button
    for (auto &chat : this->data->chats.getChats()) {
        if (chat.second.size() > 0) {
            this->deleteButtons[chat.first].setColor(
                cst.get<sf::Color>("textColor"));
            this->deleteButtonActive[chat.first] = true;
        } else {
            this->deleteButtons[chat.first].setColor(
                cst.get<sf::Color>("textColorNotActive"));
            this->deleteButtonActive[chat.first] = false;
        }
    }

    this->convBackgrounds[this->data->chats.getActiveChatName()]
        .setOutlineThickness(4.0f);
}

void ChatSelectionState::handleInput() {
    sf::Event event;
    while (this->data->window.pollEvent(event)) {
        if (this->data->input.isWindowClosed(event)) {
            this->data->window.close();
        }

        // Conversation clicked
        for (auto &block : this->convAreas) {
            if (this->data->input.isMouseClickedInArea(
                    block.second, sf::Mouse::Left, event, this->data->window)) {

                this->data->chats.setActiveChat(block.first);
                std::cout << "Active chat: " << block.first << std::endl;

                // go back to the chat
                this->data->machine.removeState();

                // for (auto &chat : this->data->chats.getChats()) {
                //     std::cout << chat.first << std::endl;
                // }

                this->data->window.setTitle(block.first);
                this->data->assets.play(cst["clickSoundName"]);
            }
        }

        // Poubelle clicked
        for (auto &poubelle : this->deleteButtons) {
            if (this->data->input.isSpriteClicked(poubelle.second,
                                                  sf::Mouse::Left, event,
                                                  this->data->window) &&
                this->deleteButtonActive[poubelle.first]) {

                this->deleteChat(poubelle.first);
                this->data->chats.deleteChat(poubelle.first);
                this->data->assets.play(cst["clickSoundName"]);
                this->deleteButtons[poubelle.first].setColor(
                    cst.get<sf::Color>("textColorNotActive"));
                this->deleteButtonActive[poubelle.first] = false;
            }
        }
    }
}
void ChatSelectionState::update(float dt) {}

void ChatSelectionState::draw(float dt) {
    // Iterate trough the maps
    for (auto &conv : this->conversations) {
        this->data->window.draw(this->convBackgrounds[conv.first]);
        this->data->window.draw(conv.second);
        this->data->window.draw(this->deleteButtons[conv.first]);
    }
}

void ChatSelectionState::newChat(std::string name) {
    this->conversations[name] =
        InputBox(this->data->assets.getFont(cst["fontName"]));
    this->deleteButtons[name] =
        sf::Sprite(this->data->assets.getTexture(cst["deleteButtonName"]));
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

    // Scale the Buttons
    this->data->assets.setTextureSmooth(cst["moreButtonName"], true);
    this->deleteButtons[name].scale(0.1f, 0.1f);
    this->data->assets.setTextureSmooth(cst["deleteButtonName"], true);

    // write the name of the chat
    this->conversations[name].write(name);
}

void ChatSelectionState::deleteChat(std::string name) {
    // this->conversations[name] =
    // this->deleteButtons
    // this->convAreas
    // this->convBackgrounds
}
