#include "RemotePlayer.hpp"
#include "../game_logic/Board.hpp"

RemotePlayer::RemotePlayer(sf::TcpSocket& socket) : _socket(socket) {
    _socket.setBlocking(false);
}

std::optional<Move> RemotePlayer::getMove(Board& board, PieceColor color) {
    sf::Packet packet;
    sf::Socket::Status status = _socket.receive(packet);

    if (status == sf::Socket::Status::Done) {
        // Deserialize packet into move
        //return receivedMove;
    }

    return std::nullopt;
}