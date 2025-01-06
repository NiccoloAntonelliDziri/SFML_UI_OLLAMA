#include "ollama_state.hpp"

namespace llm {
inline std::ostringstream response;
inline std::atomic<bool> loadedModel{false};
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

void load_model(const std::string &model_name, bool &loaded_model) {
    bool b = ollama::load_model(model_name);
    loaded_model = b;
}

void generate(const std::string &model, const ollama::messages messages,
              const std::function<void(const ollama::response &)> &callback) {
    ollama::chat(model, messages, callback);
}

void OllamaState::init() {
    // Load model in memory

    this->inputBoxBackground.setPosition(
        cst.get<sf::Vector2f>("inputBoxPosition"));
    this->inputBoxBackground.setSize(cst.get<sf::Vector2f>("inputBoxSize"));
    this->inputBoxBackground.setFillColor(sf::Color::Transparent);
    this->inputBoxBackground.setOutlineColor(
        cst.get<sf::Color>("inputBoxOutlineColor"));
    this->inputBoxBackground.setOutlineThickness(
        cst.get<float>("inputBoxThickness"));

    this->inputBox.write(cst["inputDefaultText"]);

    // Couleur noire + transparence + fond blanc = gris
    sf::Color color = cst.get<sf::Color>("textColor");
    color.a = 128;
    this->inputBox.setColor(color);
    this->inputBox.setPosition(cst.get<sf::Vector2f>("inputBoxTextPosition"));
    this->inputBox.setMaxLinesToDisplay(3);

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
            this->inputBox.write("");

            // InputBox is clicked outside exit input mode
        } else if (this->data->input.isMouseClickedOutsideArea(
                       this->inputBoxArea, sf::Mouse::Left, event,
                       this->data->window)) {

            this->inputBox.setSelected(false);

            // Set transparency once again
            sf::Color color = this->inputBox.getColor();
            color.a = 128;
            this->inputBox.setColor(color);
            this->inputBox.write(cst["inputDefaultText"]);
        }

        // Scroll text up and down

        // Keyboard scrolling
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                std::cout << "UP" << std::endl;
                this->inputBox.scrollUp();
            } else if (event.key.code == sf::Keyboard::Down) {
                std::cout << "DOWN" << std::endl;
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

        if (event.type == sf::Event::TextEntered) {

            this->inputBox.typedOn(event);

            // Wait for thread to finish previous request
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) &&
                this->ollamathread.isReady() && !this->promptInput.empty()) {

                // CALL OLLAMA RESPONSE
                llm::currentPrompt = this->promptInput;
                std::cout << "PROMPT: " << this->promptInput << std::endl;

                this->messages.push_back(
                    ollama::message("user", this->promptInput));

                this->ollamathread.start(generate, "granite3-moe",
                                         this->messages,
                                         this->response_callback);

                this->promptInput.clear();
                this->inputBox.write("");
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
        this->ollamathread.join();
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
