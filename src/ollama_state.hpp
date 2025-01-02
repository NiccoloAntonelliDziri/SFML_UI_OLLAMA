#pragma once

#include "app.hpp"
#include "constants.hpp"
#include "multiline_text.hpp"
#include "state.hpp"

void on_receive_response(const ollama::response &response);

class OllamaState : public State {
    public:
    OllamaState(AppDataRef data)
        : data(data), streamingCounter(0), ollamathread(nullptr) {

        this->inputBox =
            MultilineText(this->data->assets.getFont(cst["fontName"]));
    }
    ~OllamaState() {
        if (this->ollamathread != nullptr) {
            this->ollamathread->join();
            delete this->ollamathread;
        }
    }

    void init() override;
    void handleInput() override;
    void update(float dt) override;
    void draw(float dt) override;

    private:
    AppDataRef data;

    // sf::View chatView;

    std::string promptInput;
    std::string temp;
    // TextBox textBox;
    MultilineText inputBox;

    unsigned streamingCounter;

    std::function<void(const ollama::response &)> response_callback;
    std::thread *ollamathread;

    ollama::messages messages;
};
