#pragma once

#include "../ollama/ollama.hpp"
#include "app.hpp"
#include "constants.hpp"
#include "multiline_text.hpp"
#include "state.hpp"
#include "thread_manager.hpp"

void on_receive_response(const ollama::response &response);

void load_model(const std::string &model_name);

void generate(const std::string &model, const ollama::messages messages,
              const std::function<void(const ollama::response &)> &callback);

class OllamaState : public State {
    public:
    OllamaState(AppDataRef data)
        : activateScrolling(true), data(data), streamingCounter(0),
          currentMessageLineCounter(2) {

        this->inputBox = InputBox(this->data->assets.getFont(cst["fontName"]));

        // Load the textures in the sprite
        this->chatButton.setTexture(
            this->data->assets.getTexture(cst["chatButtonName"]));
        this->enterButton.setTexture(
            this->data->assets.getTexture(cst["enterButtonName"]));

        // Set the position of the buttons
        this->chatButton.setPosition(
            cst.get<sf::Vector2f>("chatButtonPosition"));
        this->enterButton.setPosition(
            cst.get<sf::Vector2f>("enterButtonPosition"));

        // Couleur noire + transparence + fond blanc = gris
        sf::Color color = cst.get<sf::Color>("textColor");
        color.a = 128;
        this->inputBox.setColor(color);
        this->inputBox.setPosition(
            cst.get<sf::Vector2f>("inputBoxTextPosition"));
        this->inputBox.setMaxLinesToDisplay(3);

        this->inputBoxBackground.setPosition(
            cst.get<sf::Vector2f>("inputBoxPosition"));
        this->inputBoxBackground.setSize(cst.get<sf::Vector2f>("inputBoxSize"));
        this->inputBoxBackground.setFillColor(sf::Color::Transparent);
        this->inputBoxBackground.setOutlineColor(
            cst.get<sf::Color>("inputBoxOutlineColor"));
        this->inputBoxBackground.setOutlineThickness(
            cst.get<float>("inputBoxThickness"));

        this->userMessageBox =
            MessageBox(this->data->assets.getFont(cst["fontName"]), "User");
        this->userMessageBox.setPosition(
            cst.get<sf::Vector2f>("bottomChatPosition"));

        this->llmMessageBox = MessageBox(
            this->data->assets.getFont(cst["fontName"]), cst["modelLLMname"]);
        this->llmMessageBox.setPosition(
            cst.get<sf::Vector2f>("bottomChatPosition"));

        ollama::show_requests(true);
        ollama::show_replies(true);
    }
    ~OllamaState() = default;

    void init() override;
    void handleInput() override;
    void update(float dt) override;
    void draw(float dt) override;

    private:
    void handleScrolling(const sf::Event &event);

    bool activateScrolling; // For scrolling up and down the chatbox

    AppDataRef data;

    std::string promptInput;
    std::string temp;

    InputBox inputBox;
    sf::RectangleShape inputBoxBackground; // Rectangle autour du texte input
    sf::IntRect inputBoxArea;              // For easy input detection

    // ChatBox chatBox;
    // ollama::messages messages;

    MessageBox userMessageBox; // Temporary message box for user
    MessageBox llmMessageBox;  // Temporary message box for response

    unsigned streamingCounter; // For the streaming effect, updating the chatbox
    unsigned currentMessageLineCounter; // For the current message llmMessageBox

    ThreadManager<void> ollamathread;

    sf::Sprite chatButton;
    sf::Sprite enterButton;
};
