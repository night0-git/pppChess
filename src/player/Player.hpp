#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../game_logic/Board.hpp"
#include <optional>

class Player {
public:
    virtual ~Player() = default;

    virtual std::optional<Move> getMove(Board& board, PieceColor color) = 0;
};

#endif // PLAYER_HPP