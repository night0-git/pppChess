#ifndef _BOARD_OBSERVER_H_
#define _BOARD_OBSERVER_H_

#include <SFML/Graphics.hpp>
#include "../game_logic/Piece.hpp"

struct Move;
struct MoveResult;

class BoardObserver {
public:
    virtual ~BoardObserver() = default;
    virtual void onBoardInit() = 0;
    virtual void onMoveEvent(const MoveResult& result) = 0;
    virtual void onPromoteSelection(sf::Vector2i sqr, PieceType& type) = 0;
};

#endif