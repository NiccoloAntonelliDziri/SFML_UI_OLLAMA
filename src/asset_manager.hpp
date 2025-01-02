#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class AssetManager {
    public:
    AssetManager() = default;
    ~AssetManager() = default;

    // Charge une texture et la sauvegarde dans la map des textures
    void loadTexture(std::string name, std::string filename);

    inline sf::Texture &getTexture(std::string name) {
        return this->textures.at(name);
    }

    // Lisse la texture si `smooth` est à `true`.
    // Désactive le lissage sinon
    void setTextureSmooth(std::string name, bool smooth);

    // Rend le sprite transparant
    // valeur alpha de rgba (0 - 255)
    void setSpriteTransparency(sf::Sprite &sprite, sf::Uint8 alpha = 255);

    // Charge une police de caractères et la sauvegarde dans la map des font
    void loadFont(std::string name, std::string filename);

    inline sf::Font &getFont(std::string name) { return this->fonts.at(name); }

    // Change le style du texte
    void setTextStyle(sf::Text &t, // reference vers le texte à changer le style
                      unsigned int characterSize = 24,
                      const sf::Color &color = sf::Color::Black,
                      sf::Uint32 style = sf::Text::Regular,
                      const sf::Color &outlineColor = sf::Color::Black,
                      float outlineThickness = 1.0f);

    // Charge un son et la sauvegarde dans la map des sons.
    void loadSound(std::string name, std::string filename);

    void play(std::string name);

    private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
    std::map<std::string, sf::SoundBuffer> soundbuffer;
    std::map<std::string, sf::Sound> sounds;
};
