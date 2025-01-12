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
        // std::cout << std::endl;
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

    // Create default chat
    this->data->chats.addChat("default");
    this->data->chats.setActiveChat("default");
}

void OllamaState::handleInput() {
    sf::Event event;
    while (this->data->window.pollEvent(event)) {
        if (this->data->input.isWindowClosed(event)) {
            this->data->window.close();
        }
        // InputBox is empty
        if (this->inputBox.getText() == "") {
            // Set transparency once again
            this->inputBox.setColor(cst.get<sf::Color>("textColorNotActive"));
            this->inputBox.write(cst["inputDefaultText"]);
        }

        this->handleScrolling(event);

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
                    // deactivate scrolling
                    this->activateScrolling = false;

                    // CALL OLLAMA RESPONSE
                    llm::currentPrompt = this->promptInput;
                    // std::cout << "PROMPT: " << this->promptInput <<
                    // std::endl;

                    // this->messages.push_back(
                    //     ollama::message("User", this->promptInput));
                    this->data->chats.getActiveMessages().push_back(
                        ollama::message("User", this->promptInput));

                    // this->chatBox.scrollUp();
                    // "\n  " avec 2 espaces pour sauter une ligne sinon la
                    // fonction scippe le saut de ligne additionnel
                    this->userMessageBox.write(this->promptInput + "\n  ");
                    // this->chatBox.addMessage(this->userMessageBox);
                    this->data->chats.getActiveChatBox().addMessage(
                        this->userMessageBox);
                    // this->chatBox.scrollUp();
                    this->llmMessageBox.write("...");
                    // this->chatBox.addMessage(this->llmMessageBox);
                    this->data->chats.getActiveChatBox().addMessage(
                        this->llmMessageBox);

                    // start thread
                    this->ollamathread.start(
                        generate, cst["modelLLMname"],
                        // this->messages,
                        this->data->chats.getActiveMessages(),
                        on_receive_response);

                    this->promptInput.clear();
                    this->inputBox.write("");
                } else {
                    std::cout << "Thread is not ready" << std::endl;
                }

            } else {
                this->promptInput = this->inputBox.getText();
            }
            // std::cout << this->promptInput << std::endl;
        }
    }
}

void OllamaState::draw(float dt) {
    this->data->window.draw(this->inputBox);
    this->data->window.draw(this->inputBoxBackground);
    // this->data->window.draw(this->chatBox);
    this->data->window.draw(this->data->chats.getActiveChatBox());
    // this->data->window.draw(this->llmMessageBox);
}

void OllamaState::update(float dt) {
    if (llm::isStreaming) {
        // affiche seulement quand un nouveau token arrive
        if (this->streamingCounter < llm::streamingCounter) {
            // std::cout << "LLM: " << llm::response.str() << std::endl;
            this->streamingCounter++;
            this->llmMessageBox.write(llm::response.str() + "\n  ");
            if ((int)this->currentMessageLineCounter <
                this->llmMessageBox.getNumberLines()) {

                // this->chatBox.scrollUp(0, this->chatBox.size() - 1);
                this->data->chats.getActiveChatBox().scrollUp(
                    0, this->data->chats.getActiveChatBox().size() - 1);
                this->currentMessageLineCounter++;
            }
            // this->chatBox.back() = this->llmMessageBox;
            // this->chatBox.updateLinesToDraw();
            this->data->chats.getActiveChatBox().back() = this->llmMessageBox;
            this->data->chats.getActiveChatBox().updateLinesToDraw();
        }
    }
    if (llm::done) {
        this->ollamathread.reset();
        // this->messages.push_back(
        //     ollama::message(cst["modelLLMname"], llm::response.str()));
        this->data->chats.getActiveMessages().push_back(
            ollama::message(cst["modelLLMname"], llm::response.str()));
        llm::response.str("");
        llm::response.clear();
        this->streamingCounter = 0;
        this->currentMessageLineCounter = 2;
        llm::done = false;
        this->activateScrolling = true;

        // for (const auto &message : this->messages) {
        //     std::cout << message << std::endl;
        // }
    }
}
void OllamaState::handleScrolling(const sf::Event &event) {
    // If mouse is in inputBox
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {

            if (this->activateScrolling &&
                !this->data->input.isMouseInArea(this->inputBoxArea,
                                                 this->data->window)) {
                this->inputBox.setColor(
                    cst.get<sf::Color>("textColorNotActive"));

                // this->chatBox.scrollUpMsg();
                this->data->chats.getActiveChatBox().scrollUpMsg();
            } else {
                this->inputBox.scrollUp();
            }

        } else if (event.key.code == sf::Keyboard::Down) {
            if (this->activateScrolling &&
                !this->data->input.isMouseInArea(this->inputBoxArea,
                                                 this->data->window)) {
                this->inputBox.setColor(
                    cst.get<sf::Color>("textColorNotActive"));
                // this->chatBox.scrollDownMsg();
                this->data->chats.getActiveChatBox().scrollDownMsg();
            } else {
                this->inputBox.scrollDown();
            }
        }
    }

    // Mouse wheel scrolling
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            if (this->activateScrolling &&
                !this->data->input.isMouseInArea(this->inputBoxArea,
                                                 this->data->window)) {
                if (event.mouseWheelScroll.delta > 0) {
                    // this->chatBox.scrollUpMsg();
                    this->data->chats.getActiveChatBox().scrollUpMsg();
                } else {
                    // this->chatBox.scrollDownMsg();
                    this->data->chats.getActiveChatBox().scrollDownMsg();
                }
            } else {
                this->inputBox.setColor(
                    cst.get<sf::Color>("textColorNotActive"));
                if (event.mouseWheelScroll.delta > 0) {
                    this->inputBox.scrollUp();
                } else {
                    this->inputBox.scrollDown();
                }
            }
        }
    }
}
