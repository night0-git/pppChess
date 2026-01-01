#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <SFML/Graphics.hpp>

class StateManager;
namespace ui {
    template <typename Id, typename Resource> class ResourceManager;
    enum class TextureId;
}

struct Context {
    sf::RenderWindow* window;
    StateManager* states;
    ui::ResourceManager<ui::TextureId, sf::Texture>* textures;
};

#endif