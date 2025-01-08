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
    OllamaState(AppDataRef data) : data(data), streamingCounter(0) {

        this->inputBox = InputBox(this->data->assets.getFont(cst["fontName"]));
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

        this->chatBox =
            MessageBox(this->data->assets.getFont(cst["fontName"]), "user");
        this->chatBox.setPosition(cst.get<sf::Vector2f>("bottomChatPosition"));

        ollama::show_requests(true);
        ollama::show_replies(true);
    }
    ~OllamaState() = default;

    void init() override;
    void handleInput() override;
    void update(float dt) override;
    void draw(float dt) override;

    private:
    AppDataRef data;

    // sf::View chatView;

    std::string promptInput;
    std::string temp;

    InputBox inputBox;
    sf::RectangleShape inputBoxBackground; // Rectangle autour du texte input
    sf::IntRect inputBoxArea;              // For easy input detection

    MessageBox chatBox;

    unsigned streamingCounter;

    ThreadManager<void> ollamathread;

    ollama::messages messages;
};
