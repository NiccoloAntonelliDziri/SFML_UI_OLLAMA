#pragma once

#include "asset_manager.hpp"
#include "constants.hpp"
#include "input_manager.hpp"
#include "music_manager.hpp"
#include "state_machine.hpp"
#include <SFML/Graphics.hpp>

// The BIG struct with all the data to run everything.
// Cette structure existe une seule fois dans la class `App`
struct AppData {
    sf::RenderWindow window;
    StateMachine machine;
    InputManager input;
    AssetManager assets;
    MusicManager music;

    ollama::messages messages;
};

using AppDataRef = std::shared_ptr<AppData>; // pointeur vers structure

class App {
    public:
    App(int width, int height, std::string title);
    ~App();

    private:
    const float dt =
        1.0f / 60.0f; // 60 frame per second, dt = time between 2 frames
    sf::Clock clock;

    AppDataRef data = std::make_shared<AppData>(); // data

    void run();
};
