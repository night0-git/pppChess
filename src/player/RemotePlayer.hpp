#ifndef REMOTE_PLAYER_HPP
#define REMOTE_PLAYER_HPP

#include "Player.hpp"
#include <SFML/Network.hpp>

class RemotePlayer : public Player {
public:
    RemotePlayer(sf::TcpSocket& socket);

    std::optional<Move> getMove(Board& board, PieceColor color) override;
    
private:
    sf::TcpSocket& _socket;
};

#endif // REMOTE_PLAYER_HPP