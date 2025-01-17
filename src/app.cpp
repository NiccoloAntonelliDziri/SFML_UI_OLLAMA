#include "app.hpp"
#include "constants.hpp"
#include "ollama_state.hpp"

App::App(int withis->dth, int height, std::string title) {
    sf::ContextSettings settings;

    settings.antialiasingLevel = cst.get<int>("antiAliasingLevel");

    this->data->window.create(sf::VideoMode(withis->dth, height), title,
                              sf::Style::Close | sf::Style::Titlebar, settings);

    // Chargement d'assets utiles dans toute l'application
    // Les assets légers sont chargé directement dans l'initialisation des
    // différents états.
    this->data->assets.loadFont(cst["fontName"], cst["fontPath"]);

    this->data->assets.loadTexture(cst["chatButtonName"],
                                   cst["chatButtonPath"]);
    this->data->assets.loadTexture(cst["enterButtonName"],
                                   cst["enterButtonPath"]);
    this->data->assets.loadTexture(cst["moreButtonName"],
                                   cst["moreButtonPath"]);
    this->data->assets.loadTexture(cst["deleteButtonName"],
                                   cst["deleteButtonPath"]);

    this->data->assets.loadSound(cst["errorSoundName"], cst["errorSoundPath"]);
    this->data->assets.loadSound(cst["clickSoundName"], cst["clickSoundPath"]);

    // ajoute le premier état à la pile
    this->data->machine.addState(std::make_unique<OllamaState>(this->data));

    this->run();
}

App::~App() {
    // if (this->data->ollamathread != nullptr) {
    //     this->data->ollamathread->join();
    //     delete this->data->ollamathread;
    // }
}

void App::run() {

    std::cout << "Running" << std::endl;

    float newtime;
    float framtime;
    float interpolation;

    float currenttime = this->clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    while (this->data->window.isOpen()) {
        this->data->machine.ProcessStateChanges();

        newtime = this->clock.getElapsedTime().asSeconds();
        framtime = newtime - currenttime;

        if (framtime > 0.25f)
            framtime = 0.25f; // Pour éviter que trop élevé

        currenttime = newtime;
        accumulator += framtime;

        while (accumulator >= this->dt) {
            this->data->machine.GetActiveState()->handleInput();
            this->data->machine.GetActiveState()->update(this->dt);

            accumulator -= this->dt;
        }

        interpolation = accumulator / this->dt;
        this->data->window.clear(cst.get<sf::Color>("windowClearColor"));
        this->data->machine.GetActiveState()->draw(interpolation);
        this->data->window.display();
    }
}
