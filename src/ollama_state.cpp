#include "ollama_state.hpp"

#include "app.hpp"
#include "textbox.hpp"

namespace llm {
std::ostringstream response;
std::atomic<bool> done{false};
std::atomic<bool> isStreaming{false};
std::atomic<unsigned> streamingCounter{0};
std::string currentPrompt;
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

void OllamaState::init() {
    sf::Vector2u screen = this->data->window.getSize();

    sf::Vector2f size(screen.x - 2 * 100, 3 * (20 + 2));
    // this->textBox = TextBox(20, size, sf::Color::Black, true);

    sf::Vector2f pos(100, screen.y - 200);
    // this->textBox.setPosition(pos);
    // this->textBox.setFont(
    //     this->data->assets.getFont(constants::font::ubuntuMonoRegular));
    // this->textBox.setLimit(true, 2 * 107);

    // Calculer les valeurs en fonction de la taille de la fenetre
    // ajuster ces valeurs pour placer la view au bon endroit
    // this->chatView = sf::View({200.f, 200.f}, {300.f, 200.f});
    // Viewpoint centré, a moitié de taille de la fenetre
    // this->chatView.setViewport(sf::FloatRect({0.25f, 0.25f}, {0.5f, 0.5f}));

    this->response_callback = on_receive_response;
}

void OllamaState::handleInput() {
    sf::Event event;
    while (this->data->window.pollEvent(event)) {
        if (this->data->input.isWindowClosed(event)) {
            this->data->window.close();
        }
        if (event.type == sf::Event::TextEntered) {

            // this->textBox.typedOn(event);
            //
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            //     // CALL OLLAMA RESPONSE
            //     llm::currentPrompt = this->promptInput;
            //     std::cout << "PROMPT: " << this->promptInput << std::endl;
            //
            //     this->data->messages.push_back(
            //         ollama::message("user", this->promptInput));
            //
            //     this->ollamathread = new std::thread([this] {
            //         ollama::generate("granite3-moe", llm::currentPrompt,
            //                          this->response_callback);
            //     });
            //
            //     this->promptInput.clear();
            //     this->textBox.reset();
            // }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
            //     this->promptInput =
            //         this->promptInput.substr(0, this->promptInput.length() -
            //         1);
            // } else if (this->promptInput.length() <=
            // this->textBox.getLimit()) {
            //     this->temp = event.text.unicode;
            //     if (this->temp[0] != ENTER_KEY)
            //         this->promptInput += this->temp[temp.length() - 1];
            //
            //     if (this->promptInput.length() ==
            //         this->textBox.getLimit() / 2) {
            //         // this->promptInput += 'A';
            //         std::cout << "NEWLINE" << std::endl;
            //         this->textBox.addLine();
            //     }
            // }
            // std::cout << this->promptInput << std::endl;
        }
    }
}
void OllamaState::draw(float dt) {
    // this->textBox.draw(this->data->window);

    // this->data->window.setView(this->chatView);
    //
    // this->data->window.draw(this->circle);
    //
    // this->data->window.setView(this->data->window.getDefaultView());
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
        llm::response.str("");
        llm::response.clear();
        this->streamingCounter = 0;
        llm::done = false;
    }
}
