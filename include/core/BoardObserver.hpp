#ifndef _BOARD_OBSERVER_H_
#define _BOARD_OBSERVER_H_

#include <SFML/Graphics.hpp>
#include "../game_logic/Piece.hpp"

struct Move;

class BoardObserver {
public:
    virtual ~BoardObserver() = default;
    virtual void onBoardInit() = 0;
    virtual void onPieceMoved(const Move& move) = 0;
    virtual void onPieceCaptured(const Piece* piece) = 0;
    virtual void onPromoteSelection(const sf::Vector2i& sqr, PieceType& type) = 0;
    virtual void onPromotion(const sf::Vector2i& sqr, PieceType type, const Piece* oldPcs) = 0;
};

#endif