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

        this->messageBox =
            MessageBox(this->data->assets.getFont(cst["fontName"]), "user");

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
    sf::RectangleShape inputBoxBackground;
    sf::IntRect inputBoxArea; // For easy input detection

    // ChatMessages chatMessagesBox;
    ollama::messages messages;

    MessageBox messageBox;

    unsigned streamingCounter;

    ThreadManager<void> ollamathread;
};
