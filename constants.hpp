#pragma once

const int MAX_DEPTH = 8;

/*
    White Rook = 3
    White Knight = 1
    White Bishop = 2
    White Queen = 4
    White King = 5
    White Pawn = 0

    Black Pawn = 6
    Black Rook = 9
    Black Knight = 7
    Black Bishop = 8
    Black Queen = 10
    Black King = 11
*/

namespace MATERIAL_VALUE {
    const int PAWN_VALUE = 100;
    const int KNIGHT_VALUE = 300;
    const int BISHOP_VALUE = 300;
    const int ROOK_VALUE = 500;
    const int QUEEN_VALUE = 900;
    const int KING_VALUE = 900000;

    const int VALUE[12] = {
        PAWN_VALUE,
        KNIGHT_VALUE,
        BISHOP_VALUE,
        ROOK_VALUE,
        QUEEN_VALUE,
        KING_VALUE,
        -PAWN_VALUE,
        -KNIGHT_VALUE,
        -BISHOP_VALUE,
        -ROOK_VALUE,
        -QUEEN_VALUE,
        -KING_VALUE,
    };
};