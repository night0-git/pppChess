#ifndef SOUND_PLAYER_HPP
#define SOUND_PLAYER_HPP

#include "Resource.hpp"
#include "ResourceManager.hpp"
#include <SFML/Audio.hpp>
#include <list>

namespace ui {

class SoundPlayer {
public:
    SoundPlayer(const ResourceManager<SoundId, sf::SoundBuffer>& manager)
    : _manager(manager) {}

    void play(SoundId id) {
        _activeSounds.remove_if([](const sf::Sound& s) {
            return s.getStatus() == sf::Sound::Status::Stopped;
        });

        _activeSounds.emplace_back(sf::Sound(_manager.get(id)));
        _activeSounds.back().setVolume(_masterVolume);
        _activeSounds.back().play();
    }
    
    int& masterVolume() {
        return _masterVolume;
    }

    void setMasterVolumn(float volume) {
        _masterVolume = volume;
    }

private:
    const ResourceManager<SoundId, sf::SoundBuffer>& _manager;
    std::list<sf::Sound> _activeSounds;
    int _masterVolume = 50.f;
};

}

#endif // SOUND_PLAYER_HPP