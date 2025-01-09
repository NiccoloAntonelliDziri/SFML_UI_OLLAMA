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
        if (this->inputBox.getText() == "") {
            // Set transparency once again
            this->inputBox.setColor(cst.get<sf::Color>("textColorNotActive"));
            this->inputBox.write(cst["inputDefaultText"]);
        }

        // If mouse is in inputBox
        if (event.type == sf::Event::KeyPressed) {
            std::cout << "Scrolling offset: " << this->scrollingOffset
                      << std::endl;
            std::cout << "maxNumberLinesChatToDisplay: "
                      << cst.get<int>("maxNumberLinesChatToDisplay")
                      << std::endl;
            int maxLines = cst.get<int>("maxNumberLinesChatToDisplay");

            if (event.key.code == sf::Keyboard::Up) {

                if (this->data->input.isMouseInArea(this->inputBoxArea,
                                                    this->data->window)) {
                    this->inputBox.scrollUp();
                } else {
                    this->inputBox.setColor(
                        cst.get<sf::Color>("textColorNotActive"));
                    if (this->scrollingOffset >= 0 &&
                        this->scrollingOffset<
                            maxLines &&this->chatBox.getTotalNumberLines()>
                            maxLines) {

                        this->chatBox.scrollUp();
                        this->scrollingOffset++;
                    }
                }
            } else if (event.key.code == sf::Keyboard::Down) {
                if (this->data->input.isMouseInArea(this->inputBoxArea,
                                                    this->data->window)) {
                    this->inputBox.scrollDown();
                } else {
                    this->inputBox.setColor(
                        cst.get<sf::Color>("textColorNotActive"));
                    if (this->scrollingOffset > 0 &&
                        this->scrollingOffset <=
                            cst.get<int>("maxNumberLinesChatToDisplay")) {
                        this->chatBox.scrollDown();
                        this->scrollingOffset--;
                    }
                    // this->chatBox.scrollDown();
                }
            }
        }
        // Mouse wheel scrolling
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                if (this->data->input.isMouseInArea(this->inputBoxArea,
                                                    this->data->window)) {
                    this->inputBox.setColor(
                        cst.get<sf::Color>("textColorNotActive"));
                    if (event.mouseWheelScroll.delta > 0) {
                        this->inputBox.scrollUp();
                    } else {
                        this->inputBox.scrollDown();
                    }
                } else {
                    if (event.mouseWheelScroll.delta > 0) {
                        if (this->scrollingOffset > 0) {
                            this->chatBox.scrollUp();
                            this->scrollingOffset++;
                        }
                    } else {
                        if (this->scrollingOffset <
                            cst.get<int>("maxNumberLinesChatToDisplay")) {
                            this->chatBox.scrollDown();
                            this->scrollingOffset--;
                        }
                    }
                }
            }
        }

        if (event.type == sf::Event::TextEntered) {

            this->inputBox.setColor(cst.get<sf::Color>("textColor"));
            if (this->inputBox.getText() == cst["inputDefaultText"]) {
                this->inputBox.write("");
            }

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

                    this->chatBox.scrollUp();
                    this->userMessageBox.write(this->promptInput);
                    this->chatBox.addMessage(this->userMessageBox);
                    this->chatBox.scrollUp();
                    this->llmMessageBox.write("...");
                    this->chatBox.addMessage(this->llmMessageBox);

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
    // this->data->window.draw(this->llmMessageBox);
}

void OllamaState::update(float dt) {
    if (llm::isStreaming) {
        // affiche seulement quand un nouveau token arrive
        if (this->streamingCounter < llm::streamingCounter) {
            std::cout << "LLM: " << llm::response.str() << std::endl;
            this->streamingCounter++;
            this->llmMessageBox.write(llm::response.str());
            this->chatBox.back() = this->llmMessageBox;
        }
        if ((int)this->currentMessageLineCounter <
            this->llmMessageBox.getNumberLines()) {

            this->chatBox.scrollUp(0, this->chatBox.size() - 1);
            this->currentMessageLineCounter++;
        }
    }
    if (llm::done) {
        this->ollamathread.reset();
        this->messages.push_back(
            ollama::message("assistant", llm::response.str()));
        llm::response.str("");
        llm::response.clear();
        this->streamingCounter = 0;
        this->currentMessageLineCounter = 2;
        llm::done = false;

        for (const auto &message : this->messages) {
            std::cout << message << std::endl;
        }
    }
}
