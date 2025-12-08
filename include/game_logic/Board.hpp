#ifndef _BOARD_H_
#define _BOARD_H_

#include "Piece.hpp"
#include <memory>
#include <array>
using std::array, std::unique_ptr;

class Board {
public:
    Board();

private:
    static const int SIZE = 8;
    array<array<unique_ptr<Piece>, SIZE>, SIZE> _grid;
};

#endif