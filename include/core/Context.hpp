#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class StateManager;
namespace ui {
    template <typename Id, typename Resource> class ResourceManager;
    class SoundPlayer;
    enum class TextureId;
    enum class SoundId;
}

struct Context {
    sf::RenderWindow* window;
    StateManager* states;
    ui::ResourceManager<ui::TextureId, sf::Texture>* textures;
    ui::ResourceManager<ui::SoundId, sf::SoundBuffer>* sounds;
    ui::SoundPlayer* soundPlayer;
    sf::Clock* clock;
};

#endif