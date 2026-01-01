#ifndef _BOARD_OBSERVER_H_
#define _BOARD_OBSERVER_H_

#include <SFML/Graphics.hpp>
#include "../game_logic/Piece.hpp"

class BoardObserver {
public:
    virtual ~BoardObserver() = default;
    virtual void onBoardInit() = 0;
    virtual void onPieceMoved(const sf::Vector2i& src, const sf::Vector2i& dest) = 0;
    virtual void onPieceCaptured(const Piece* piece) = 0;
    virtual void onPromotion(const sf::Vector2i& sqr, PieceType type) = 0;
};

#endif