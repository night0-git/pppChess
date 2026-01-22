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
    
    void snapToPosition(sf::Vector2i sqr);
    void animateToPosition(sf::Vector2i sqr);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(sf::Time dt);
    void normalizeSprite();

private:
    sf::Vector2f sqrToPos(sf::Vector2i sqr) const;

    const Piece* _piece;
    sf::Sprite _sprite;
    float _tileSize;

    // Animation members
    bool _isAnimating = false;
    sf::Time _animationTime;
    sf::Time _animationDuration = sf::seconds(0.1f);
    sf::Vector2f _startPos;
    sf::Vector2f _targetPos;
};

}

#endif