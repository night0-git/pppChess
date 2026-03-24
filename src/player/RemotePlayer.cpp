#include "RemotePlayer.hpp"
#include "../game_logic/Board.hpp"

RemotePlayer::RemotePlayer(sf::TcpSocket& socket, sf::IpAddress address)
: _socket(socket), _address(address) {
    _socket.setBlocking(false);
}

sf::IpAddress RemotePlayer::address() const {
    return _address;
}

std::optional<Move> RemotePlayer::getMove(Board& board, PieceColor color) {
    sf::Packet packet;
    sf::Socket::Status status = _socket.receive(packet);

    if (status == sf::Socket::Status::Done) {
        Move move;
        packet >> move;
        return move;
    }

    return std::nullopt;
}