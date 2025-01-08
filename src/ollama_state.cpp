#include "ollama_state.hpp"

namespace llm {
inline std::ostringstream response;
inline std::atomic<bool> done{false};
inline std::atomic<bool> isStreaming{false};
inline std::atomic<unsigned> streamingCounter{0};
inline std::string currentPrompt;
} // namespace llm

void on_receive_response(const ollama::response &response) {
    // std::cout << response << std::flush;

    llm::isStreaming = true;
    llm::streamingCounter++;
    llm::response << response << std::flush;

    if (response.as_json()["done"] == true) {
        llm::done = true;
        llm::isStreaming = false;
        llm::streamingCounter = 0;
        std::cout << std::endl;
    }
}

void generate(const std::string &model, const ollama::messages messages,
              const std::function<void(const ollama::response &)> &callback) {
    ollama::chat(model, messages, callback);
}

void OllamaState::init() {
    this->inputBox.write(cst["inputDefaultText"]);

    this->inputBoxArea = {
        static_cast<int>(this->inputBoxBackground.getPosition().x),
        static_cast<int>(this->inputBoxBackground.getPosition().y),
        static_cast<int>(this->inputBoxBackground.getSize().x),
        static_cast<int>(this->inputBoxBackground.getSize().y)};
}

void OllamaState::handleInput() {
    sf::Event event;
    while (this->data->window.pollEvent(event)) {
        if (this->data->input.isWindowClosed(event)) {
            this->data->window.close();
        }
        // InputBox is clicked enter input mode
        if (this->data->input.isMouseClickedInArea(this->inputBoxArea,
                                                   sf::Mouse::Left, event,
                                                   this->data->window)) {

            this->inputBox.setSelected(true);
            this->inputBox.setColor(cst.get<sf::Color>("textColor"));

            // if text is default, clear it
            if (this->inputBox.getText() == cst["inputDefaultText"]) {
                this->inputBox.write("");
            }

            // InputBox is clicked outside exit input mode
        } else if (this->data->input.isMouseClickedOutsideArea(
                       this->inputBoxArea, sf::Mouse::Left, event,
                       this->data->window)) {

            this->inputBox.setSelected(false);

            // Set transparency once again
            sf::Color color = this->inputBox.getColor();
            color.a = 128;
            this->inputBox.setColor(color);

            // If input is empty, write default text
            if (this->inputBox.getText().empty()) {
                this->inputBox.write(cst["inputDefaultText"]);
            }
        }

        // Scroll text up and down

        // If inputBox is selected
        if (this->inputBox.isSelected()) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    this->inputBox.scrollUp();
                } else if (event.key.code == sf::Keyboard::Down) {
                    this->inputBox.scrollDown();
                }
            }

            // Mouse wheel scrolling
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    if (event.mouseWheelScroll.delta > 0) {
                        this->inputBox.scrollUp();
                    } else {
                        this->inputBox.scrollDown();
                    }
                }
            }
        } else { // if inputBox is not selected
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    std::cout << "UP" << std::endl;
                    this->chatBox.scrollUp();

                } else if (event.key.code == sf::Keyboard::Down) {
                    std::cout << "DOWN" << std::endl;
                    this->chatBox.scrollDown();
                }
            }
        }

        if (event.type == sf::Event::TextEntered) {

            this->inputBox.typedOn(event);

            // If Key pressed and prompt is not empty and model is loaded
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) &&
                !this->promptInput.empty()) {

                // If thread is ready to start
                if (this->ollamathread.isReady()) {

                    // CALL OLLAMA RESPONSE
                    llm::currentPrompt = this->promptInput;
                    std::cout << "PROMPT: " << this->promptInput << std::endl;

                    this->messages.push_back(
                        ollama::message("user", this->promptInput));

                    this->chatBox.write(this->promptInput);

                    this->ollamathread.start(generate, cst["modelLLMname"],
                                             this->messages,
                                             on_receive_response);

                    this->promptInput.clear();
                    this->inputBox.write("");
                } else {
                    std::cout << "Thread is not ready" << std::endl;
                }

            } else {
                this->promptInput = this->inputBox.getText();
            }
            std::cout << this->promptInput << std::endl;
        }
    }
}
void OllamaState::draw(float dt) {
    this->data->window.draw(this->inputBox);
    this->data->window.draw(this->inputBoxBackground);
    this->data->window.draw(this->chatBox);
}

void OllamaState::update(float dt) {
    if (llm::isStreaming) {
        // affiche seulement quand un nouveau token arrive
        if (this->streamingCounter < llm::streamingCounter) {
            std::cout << "LLM: " << llm::response.str() << std::endl;
            this->streamingCounter++;
        }
    }
    if (llm::done) {
        this->ollamathread.reset();
        this->messages.push_back(
            ollama::message("assistant", llm::response.str()));
        llm::response.str("");
        llm::response.clear();
        this->streamingCounter = 0;
        llm::done = false;

        for (const auto &message : this->messages) {
            std::cout << message << std::endl;
        }
    }
}
