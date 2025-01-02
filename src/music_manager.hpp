#pragma once

#include "constants.hpp"
#include <SFML/Audio/Music.hpp>

enum class MusicStatus {
    Stopped,
    Paused,
    Playing,
    NotExisting,
};

class MusicManager {
    public:
    MusicManager()
        : volume(cst.get<float>("initialMusicVolume")), isMusicMuted(false),
          status(MusicStatus::NotExisting) {}
    ~MusicManager() = default;

    void openMusic(std::string filepath);

    inline bool isPlaying() const { return status == MusicStatus::Playing; }
    inline void play() {
        this->music.play();
        this->status = MusicStatus::Playing;
    }
    inline void setVolume(float volume) {
        this->volume = volume;
        this->music.setVolume(volume);
    }
    inline void mute() {
        this->music.setVolume(0);
        this->isMusicMuted = true;
    }
    inline void unMute() {
        this->music.setVolume(this->volume);
        this->isMusicMuted = false;
    }
    inline bool isMuted() { return this->isMusicMuted; }
    inline void enableLoop(bool enable) { this->music.setLoop(enable); }

    private:
    float volume;
    bool isMusicMuted;
    MusicStatus status;
    sf::Music music;
};
