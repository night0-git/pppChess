#ifndef _BOARD_H_
#define _BOARD_H_

#include "Piece.h"
#include <memory>
#include <array>

using std::array, std::unique_ptr;

class Board {
public:
    Board();

private:
    array<array<unique_ptr<Piece>, 8>, 8> _grid;
};

#endif