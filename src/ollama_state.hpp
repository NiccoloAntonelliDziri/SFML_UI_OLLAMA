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

    unsigned streamingCounter;

    ThreadManager<void> ollamathread;

    ollama::messages messages;
};
