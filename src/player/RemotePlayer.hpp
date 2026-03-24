#ifndef REMOTE_PLAYER_HPP
#define REMOTE_PLAYER_HPP

#include "Player.hpp"
#include <SFML/Network.hpp>

class RemotePlayer : public Player {
public:
    RemotePlayer(sf::TcpSocket& socket, sf::IpAddress address);
    sf::IpAddress address() const;

    std::optional<Move> getMove(Board& board, PieceColor color) override;
    
private:
    sf::TcpSocket& _socket;
    sf::IpAddress _address;
};

#endif // REMOTE_PLAYER_HPP