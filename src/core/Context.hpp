#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

class StateManager;
class LayoutManager;
namespace ui {
    template <typename Id, typename Resource> class ResourceManager;
    class SoundPlayer;
    enum class TextureId;
    enum class SoundId;
}

struct Cursors {
    std::optional<sf::Cursor> loadCursorFromFile(const std::string& filename, sf::Vector2u hotspot) const {
        sf::Image img;
        if (!img.loadFromFile("./assets/cursors/" + filename)) {
            std::cerr << "Failed to load cursor: " + filename << "\n";
            return std::nullopt;
        }
        return sf::Cursor::createFromPixels(img.getPixelsPtr(),
                                            img.getSize(),
                                            hotspot);
    }
    std::optional<sf::Cursor> arrow = loadCursorFromFile("cursor_none.png", {8, 4});
    std::optional<sf::Cursor> hand = loadCursorFromFile("hand_point.png", {7, 3});
    std::optional<sf::Cursor> handOpened = loadCursorFromFile("hand_open.png", {15, 2});
    std::optional<sf::Cursor> handClosed = loadCursorFromFile("hand_closed.png", {16, 5});
};

struct Settings {
    float masterVolume = 50.f;
    float uiScale = 1.f;
};

struct Context {
    sf::RenderWindow* window;
    StateManager* states;
    LayoutManager* layoutManager;
    ui::ResourceManager<ui::TextureId, sf::Texture>* textures;
    ui::ResourceManager<ui::SoundId, sf::SoundBuffer>* sounds;
    ui::SoundPlayer* soundPlayer;
    sf::Clock* clock;
    Settings* settings;
    Cursors* cursors;
    sf::TcpSocket *socket;
    sf::TcpListener *listener;
};

#endif