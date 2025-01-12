#pragma once

#include "app.hpp"
#include "multiline_text.hpp"
#include "state.hpp"

class ChatSelectionState : public State {
    public:
    ChatSelectionState(AppDataRef data) : data(data) {
        this->moreButtons =
            sf::Sprite(this->data->assets.getTexture(cst["moreButtonName"]));
    }
    ~ChatSelectionState() = default;

    void init() override;
    void handleInput() override;
    void update(float dt) override;
    void draw(float dt) override;

    private:
    void newChat(std::string name);
    void deleteChat(std::string name);

    AppDataRef data;

    std::map<std::string, InputBox> conversations;
    std::map<std::string, sf::Sprite> deleteButtons;
    std::map<std::string, sf::IntRect> convAreas;
    std::map<std::string, sf::RectangleShape> convBackgrounds;

    sf::Sprite moreButtons;
    bool moreButtonActive = true;
};
