#include "asset_manager.hpp"

#include <iostream>

void AssetManager::loadTexture(std::string name, std::string filename) {
    sf::Texture tex;
    if (tex.loadFromFile(filename))
        this->textures[name] = tex;
    else
        std::cerr << "Error while loading texture: " << filename << std::endl;
}

void AssetManager::setTextureSmooth(std::string name, bool smooth) {
    this->textures.at(name).setSmooth(smooth);
}

void AssetManager::setSpriteTransparency(sf::Sprite &sprite, sf::Uint8 alpha) {
    sf::Color spriteColor = sprite.getColor();
    spriteColor.a = alpha;
    sprite.setColor(spriteColor);
}

void AssetManager::loadFont(std::string name, std::string filename) {
    sf::Font font;
    if (font.loadFromFile(filename))
        this->fonts[name] = font;
    else
        std::cerr << "Error while loading font: " << filename << std::endl;
}

void AssetManager::setTextStyle(
    sf::Text &t, // reference vers le texte à changer le style
    unsigned int characterSize, const sf::Color &color, sf::Uint32 style,
    const sf::Color &outlineColor, float outlineThickness) {
    t.setCharacterSize(characterSize);
    t.setFillColor(color);
    t.setStyle(style);
    t.setOutlineColor(outlineColor);
    t.setOutlineThickness(outlineThickness);
}

void AssetManager::loadSound(std::string name, std::string filename) {
    if (this->soundbuffer[name].loadFromFile(filename))
        this->sounds[name].setBuffer(this->soundbuffer.at(name));
    else
        std::cerr << "Error while loading sound: " << filename << std::endl;
}
void AssetManager::play(std::string name) { this->sounds.at(name).play(); }
