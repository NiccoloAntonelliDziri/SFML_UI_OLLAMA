#pragma once

#include "../ollama/ollama.hpp"
#include "app.hpp"
#include "constants.hpp"
#include "multiline_text.hpp"
#include "state.hpp"

void on_receive_response(const ollama::response &response);

void load_model(const std::string &model_name, bool &loaded_model);

class OllamaState : public State {
    public:
    OllamaState(AppDataRef data) : data(data), streamingCounter(0) {

        this->inputBox = InputBox(this->data->assets.getFont(cst["fontName"]));

        ollama::show_requests(true);
        ollama::show_replies(true);

        this->response_callback = on_receive_response;
    }
    ~OllamaState() { this->ollamathread.join(); }

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

    unsigned streamingCounter;

    std::function<void(const ollama::response &)> response_callback;

    std::thread ollamathread;

    ollama::messages messages;
};
