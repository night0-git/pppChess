#ifndef _PIECE_VIEW_H_
#define _PIECE_VIEW_H_

#include <SFML/Graphics.hpp>
#include "../../game_logic/Piece.hpp"
#include "../core/ResourceManager.hpp"
#include "../core/Resource.hpp"

namespace ui {

class PieceView : public sf::Drawable, public sf::Transformable {
public:
    PieceView(const sf::Texture& texture, float tileSize, const Piece& piece);
    
    void updatePosition(const sf::Vector2i& sqr);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void normalizeSprite();
    const Piece* _piece;
    sf::Sprite _sprite;
    float _tileSize;
};

}

#endif