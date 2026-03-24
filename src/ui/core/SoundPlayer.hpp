#ifndef SOUND_PLAYER_HPP
#define SOUND_PLAYER_HPP

#include "Resource.hpp"
#include "ResourceManager.hpp"
#include <SFML/Audio.hpp>
#include <memory>

namespace ui {

class SoundPlayer {
public:
    SoundPlayer(const ResourceManager<SoundId, sf::SoundBuffer>& manager)
    : _manager(manager) {
        _pool.resize(16);
    }

    void play(SoundId id) {
        for (auto& sound : _pool) {
            if (!sound) {
                sound = std::make_unique<sf::Sound>(_manager.get(id));
            }
            else if (sound->getStatus() == sf::Sound::Status::Stopped) {
                sound->setBuffer(_manager.get(id));
            }
            // Occupied by unfinished sound
            else {
                continue;
            }
            sound->setVolume(_masterVolume);
            sound->play();
            return;
        }
    }
    
    int& masterVolume() {
        return _masterVolume;
    }

    void setMasterVolumn(float volume) {
        _masterVolume = volume;
    }

private:
    const ResourceManager<SoundId, sf::SoundBuffer>& _manager;
    std::vector<std::unique_ptr<sf::Sound>> _pool;
    int _masterVolume = 50.f;
};

}

#endif // SOUND_PLAYER_HPP