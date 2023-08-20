#include "libraries/chess.hpp"
#include "constants.hpp"
#include <iostream>

# define PAWN_VALUE     100
# define KNIGHT_VALUE   300
# define BISHOP_VALUE   300
# define ROOK_VALUE     500
# define QUEEN_VALUE    900
# define KING_VALUE     90000

int evaluate(chess::Board &board) {
    int evaluation = 0;
    auto pieces = board.occ();

    while (pieces) {
        auto square = chess::builtin::poplsb(pieces);
        int piece = (int) board.at(square);
        
        evaluation = evaluation + MATERIAL_VALUE::VALUE[piece];
    }

    return evaluation;
}