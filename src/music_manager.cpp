#include "music_manager.hpp"
#include <iostream>

void MusicManager::openMusic(std::string filepath) {
    if (!music.openFromFile(filepath))
        std::cerr << "Error while opening music: " << filepath << std::endl;
}
