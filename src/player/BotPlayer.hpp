#ifndef BOT_PLAYER_HPP
#define BOT_PLAYER_HPP

#include "Player.hpp"
#include "../engine/ChessBot.hpp"

class BotPlayer : public Player {
public:
    BotPlayer(int depth);

    std::optional<Move> getMove(Board& board, PieceColor color) override;
    
private:
    ChessBot _bot;
};

#endif // BOT_PLAYER_HPP
