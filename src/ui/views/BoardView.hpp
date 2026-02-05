#ifndef _BOARD_VIEW_H_
#define _BOARD_VIEW_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../core/Resource.hpp"
#include "../core/ResourceManager.hpp"
#include "../core/SoundPlayer.hpp"
#include "../../game_logic/Board.hpp"
#include "PieceView.hpp"
#include "../components/Component.hpp"
#include "../../core/BoardObserver.hpp"
#include <map>
#include <functional>

namespace ui {

struct BoardTheme {
    float tileSize = 150.f;
    sf::Color lightColor = sf::Color(240, 217, 181);
    sf::Color darkColor  = sf::Color(181, 136, 99);
};

class BoardView : public Component, public BoardObserver {
public:
    BoardView(const ResourceManager<TextureId, sf::Texture>& textures, SoundPlayer& sounds, const Board& board);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) override;
    void update(sf::Time dt);
    std::function<bool(Move move)> _onMoveRequest;
    sf::Vector2f getSize() const override;
    void setSize(sf::Vector2f size) override;

public:
    void onBoardInit() override;
    void onMoveEvent(const MoveResult& result) override;
    void onPromoteSelection(sf::Vector2i sqr, PieceType& type) override;

public:
    void movePieceView(Move move);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const Board& _board;
    std::map<const Piece*, PieceView> _pieceViews;

    const ResourceManager<TextureId, sf::Texture>& _textures;
    SoundPlayer& _sounds;
    
    BoardTheme _theme;

    // For event handling
    bool _isMoving = false;
    const Piece* _draggedPiece = nullptr;
    std::optional<sf::Vector2i> _selectedSqr = std::nullopt;
    bool _isDeselecting = false;
    
    sf::Vector2i localPosToSqr(const sf::Vector2f& localPos) const;
};

}

#endif