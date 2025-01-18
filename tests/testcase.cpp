#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include <typeinfo>

#include "../src/app.hpp"
#include "../src/ollama_state.hpp"

// NEVER TEST PRIVATE FUNCTIONS AND VARIABLES

TEST_CASE("Asset Manager") {
    AssetManager asset_manager;

    SUBCASE("Load Texture") {
        asset_manager.loadTexture("test", "assets/images/chat.png");
        CHECK_NOTHROW(asset_manager.getTexture("test"));
    }
    SUBCASE("Load Font") {
        asset_manager.loadFont(
            "test", "assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf");
        CHECK_NOTHROW(asset_manager.getFont("test"));
    }
    SUBCASE("Load Sound") {
        asset_manager.loadSound(
            "test", "assets/sounds/574592__tissman__menu-move3.wav");
        sleep(1);
        CHECK_NOTHROW(asset_manager.play("test"));
    }
    SUBCASE("Set Texture Smooth") {
        asset_manager.loadTexture("test", "assets/images/chat.png");
        CHECK_NOTHROW(asset_manager.setTextureSmooth("test", true));
    }
    SUBCASE("Set Sprite Transparency") {
        asset_manager.loadTexture("test", "assets/images/chat.png");
        sf::Sprite sprite;
        CHECK_NOTHROW(asset_manager.setSpriteTransparency(sprite, 100));
    }
    SUBCASE("Set Text Style") {
        asset_manager.loadFont(
            "test", "assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf");
        sf::Text text;
        text.setFont(asset_manager.getFont("test"));
        CHECK_NOTHROW(asset_manager.setTextStyle(text));
    }
}

TEST_CASE("Input Manager") {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
    InputManager input_manager;

    SUBCASE("Get Mouse Position") {
        CHECK_NOTHROW(input_manager.getMousePosition(window));
    }
    SUBCASE("Is Sprite Clicked") {
        sf::Sprite sprite;
        sf::Event event;
        CHECK(input_manager.isSpriteClicked(sprite, sf::Mouse::Button::Left,
                                            event, window) == false);
    }
    SUBCASE("Is Mouse Clicked In Area") {
        sf::IntRect area(0, 0, 100, 100);
        sf::Event event;
        CHECK(input_manager.isMouseClickedInArea(area, sf::Mouse::Button::Left,
                                                 event, window) == false);
    }
    SUBCASE("Is Mouse Clicked Outside Area") {
        sf::IntRect area(0, 0, 100, 100);
        sf::Event event;
        CHECK(input_manager.isMouseClickedOutsideArea(
                  area, sf::Mouse::Button::Left, event, window) == false);
    }
    SUBCASE("Is Window Closed") {
        sf::Event event(sf::Event::Closed);
        CHECK(input_manager.isWindowClosed(event) == true);
    }
}

// Il y a pas de musique dans l'application
TEST_CASE("Music Manager") {
    MusicManager music_manager;

    SUBCASE("Is Playing") { CHECK(music_manager.isPlaying() == false); }
    // SUBCASE("Play") { CHECK_NOTHROW(music_manager.play()); }
    SUBCASE("Set Volume") { CHECK_NOTHROW(music_manager.setVolume(50)); }
    SUBCASE("Mute") { CHECK_NOTHROW(music_manager.mute()); }
    SUBCASE("Unmute") { CHECK_NOTHROW(music_manager.unMute()); }
    SUBCASE("Is Muted") { CHECK(music_manager.isMuted() == false); }
    SUBCASE("Enable Loop") { CHECK_NOTHROW(music_manager.enableLoop(true)); }
}

TEST_CASE("Scroll Text In Place") {
    AssetManager asset_manager;
    asset_manager.loadFont("name",
                           "assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf");
    ScrollTextInPlace scroll_text_in_place(asset_manager.getFont("name"));

    SUBCASE("Write") {
        CHECK_NOTHROW(scroll_text_in_place.write("test"));
        CHECK(scroll_text_in_place.getText() == "test");
    }
    SUBCASE("Scroll Up") {
        scroll_text_in_place.write("test\ntest\ntest\ntest\ntest\ntest\ntest");
        CHECK_NOTHROW(scroll_text_in_place.scrollUp());
    }
    SUBCASE("Scroll Down") {
        scroll_text_in_place.write("test\ntest\ntest\ntest\ntest\ntest\ntest");
        CHECK_NOTHROW(scroll_text_in_place.scrollDown());
    }
    SUBCASE("Set Max Lines To Display") {
        CHECK_NOTHROW(scroll_text_in_place.setMaxLinesToDisplay(5));
        CHECK(scroll_text_in_place.getMaxLinesToDisplay() == 5);
    }
}

TEST_CASE("Message Box") {
    AssetManager asset_manager;
    asset_manager.loadFont("name",
                           "assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf");
    MessageBox message_box(asset_manager.getFont("name"), "test");

    SUBCASE("Write") {
        CHECK_NOTHROW(message_box.write("test"));
        CHECK(message_box.getText() == "test:\ntest");
    }
    SUBCASE("Get Role") { CHECK(message_box.getRole() == "test"); }
    SUBCASE("Set Role") {
        message_box.setRole("test2");
        CHECK(message_box.getRole() == "test2");
    }
    SUBCASE("Scroll Up") {
        message_box.write("test\ntest\ntest\ntest\ntest\ntest\ntest");
        CHECK_NOTHROW(message_box.scrollUp());
    }
    SUBCASE("Scroll Down") {
        message_box.write("test\ntest\ntest\ntest\ntest\ntest\ntest");
        CHECK_NOTHROW(message_box.scrollDown());
    }
}

TEST_CASE("Input Box") {
    AssetManager asset_manager;
    asset_manager.loadFont("name",
                           "assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf");
    InputBox input_box(asset_manager.getFont("name"));

    SUBCASE("Typed On") {
        sf::Event event;
        event.text.unicode = 'a';
        CHECK_NOTHROW(input_box.typedOn(event));
    }
    SUBCASE("Set Selected") {
        CHECK_NOTHROW(input_box.setSelected(true));
        CHECK(input_box.isSelected() == true);
    }
}

TEST_CASE("Chat Box") {
    AssetManager asset_manager;
    asset_manager.loadFont("name",
                           "assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf");
    ChatBox chat_box;

    SUBCASE("Add Message") {
        MessageBox message_box(asset_manager.getFont("name"), "test");
        message_box.write("test");
        chat_box.addMessage(message_box);
        // 2 = 1 message + 1 role
        CHECK(chat_box.getTotalNumberLines() == 2);
    }
    SUBCASE("Scroll Up") {
        MessageBox message_box(asset_manager.getFont("name"), "test");
        message_box.write("test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest");
        chat_box.addMessage(message_box);
        chat_box.scrollUpMsg();
        CHECK(chat_box.getOffset() == 0);
    }
    SUBCASE("Scroll Down") {
        MessageBox message_box(asset_manager.getFont("name"), "test");
        message_box.write("test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n"
                          "test\ntest\ntest");
        chat_box.addMessage(message_box);
        chat_box.scrollDownMsg();
        CHECK(chat_box.getOffset() == -1);
    }
}

TEST_CASE("Constants") {
    Constants constants("config.json");

    CHECK(constants.get<std::string>("titlebar") == "C'est incroyable!");
    CHECK(constants.get<int>("screenInitialWidth") == 1280);
    CHECK(constants["titlebar"] == "C'est incroyable!");
    CHECK(constants.get<sf::Color>("windowClearColor") ==
          sf::Color(255, 255, 255, 255));
    CHECK(constants.get<sf::Vector2f>("chatButtonPosition") ==
          sf::Vector2f(10, 555));
}

int testFunction(int a, int b) { return a + b; }

void testFunctionVoid() { return; }

TEST_CASE("Thread Manager") {
    ThreadManager<int> thread_manager;
    thread_manager.start(testFunction, 1, 2);
    CHECK(thread_manager.isReady() == false);
    CHECK(thread_manager.getResult() == 3);
    CHECK(thread_manager.isReady() == true);

    ThreadManager<void> thread_manager_void;
    thread_manager_void.start(testFunctionVoid);
    CHECK(thread_manager_void.isReady() == false);
    thread_manager_void.getResult();
    CHECK(thread_manager_void.isReady() == true);
}
