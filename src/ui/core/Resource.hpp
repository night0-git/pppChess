#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "../../game_logic/Piece.hpp"
#include <map>
#include <utility>
using std::map, std::pair;

namespace ui {

enum class TextureId {
    WPawn, WRook, WKnight, WBishop, WKing, WQueen,
    BPawn, BRook, BKnight, BBishop, BKing, BQueen,
    LSquare, DSquare,
    Error
};

enum class SoundId {
    GameStart, GameEnd,
    Move, Capture, Check, Illegal, Castle, Promote,
    TenSeconds
};

inline TextureId getPieceId(const Piece& piece) {
    static map<pair<PieceType, PieceColor>, TextureId> textureMap = {
        {{PieceType::Pawn, PieceColor::White}, TextureId::WPawn},
        {{PieceType::Pawn, PieceColor::Black}, TextureId::BPawn},
        {{PieceType::Rook, PieceColor::White}, TextureId::WRook},
        {{PieceType::Rook, PieceColor::Black}, TextureId::BRook},
        {{PieceType::Knight, PieceColor::White}, TextureId::WKnight},
        {{PieceType::Knight, PieceColor::Black}, TextureId::BKnight},
        {{PieceType::Bishop, PieceColor::White}, TextureId::WBishop},
        {{PieceType::Bishop, PieceColor::Black}, TextureId::BBishop},
        {{PieceType::Queen, PieceColor::White}, TextureId::WQueen},
        {{PieceType::Queen, PieceColor::Black}, TextureId::BQueen},
        {{PieceType::King, PieceColor::White}, TextureId::WKing},
        {{PieceType::King, PieceColor::Black}, TextureId::BKing},
    };

    auto it = textureMap.find({piece.type(), piece.color()});
    return (it != textureMap.end()) ? it->second : ui::TextureId::Error;
}

}

#endif