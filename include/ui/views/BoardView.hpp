#ifndef _BOARD_VIEW_H_
#define _BOARD_VIEW_H_

#include <SFML/Graphics.hpp>
#include "../core/Resource.hpp"
#include "../core/ResourceManager.hpp"
#include "../../game_logic/Board.hpp"
#include "../../../include/ui/views/PieceView.hpp"
#include "../../../include/core/BoardObserver.hpp"
#include <map>

namespace ui {

class BoardView : public sf::Drawable, public sf::Transformable, public BoardObserver {
public:
    BoardView(const ResourceManager<TextureId, sf::Texture>& textures, const Board& board);

public:
    void onBoardInit() override;
    void onPieceMoved(const sf::Vector2i& src, const sf::Vector2i& dest) override;
    void onPieceCaptured(const Piece* piece) override;
    void onPromotion(const sf::Vector2i& sqr, PieceType type) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const Board& _board;

    mutable std::map<const Piece*, PieceView> _pieceViews;
    const ResourceManager<TextureId, sf::Texture>& _textures;
    const float _tileSize = 150.f;
    const sf::Color _lightColor = sf::Color(240, 217, 181);
    const sf::Color _darkColor  = sf::Color(181, 136, 99);
};

}

#endif