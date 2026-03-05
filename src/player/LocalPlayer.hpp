#ifndef LOCAL_PLAYER_HPP
#define LOCAL_PLAYER_HPP

#include "Player.hpp"
class BoardView;

class LocalPlayer : public Player {
public:
    LocalPlayer(Board& board, BoardView& boardView);

    std::optional<Move> getMove(Board& board, PieceColor color) override;

private:
    BoardView& _boardView;
};

#endif // LOCAL_PLAYER_HPP
