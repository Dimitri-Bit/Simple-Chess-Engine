#include "libraries/chess.hpp"
#include <climits>

int alphaBeta(chess::Board& board, int depth, int alpha, int beta, int max);
int evaluate(chess::Board& board);

// Values from: https://www.chessprogramming.org/Simplified_Evaluation_Function
const int PAWN = 100;
const int KNIGHT = 320;
const int BISHOP = 330;
const int ROOK = 500;
const int QUEEN = 900;
const int KING = 20000;

const int materialValues[12] = {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    -PAWN,
    -KNIGHT,
    -BISHOP,
    -ROOK,
    -QUEEN,
    -KING
};

// alphaBeta(origin, depth, −∞, +∞, TRUE)
int alphaBeta(chess::Board& board, int depth, int alpha, int beta, int max) {
    if (depth == 0) {
        return evaluate(board);
    }

    int value;
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);

    if (max) { // White
        value = INT_MIN;
        for (chess::Move move : moves) {
            board.makeMove(move);
            value = std::max(value, alphaBeta(board, depth-1, alpha, beta, false));
            board.unmakeMove(move);

            if (value > beta) {
                break;
            }
            alpha = std::max(alpha, value);
        }
    } else {
        value = INT_MAX;
        for (chess::Move move : moves) {
            board.makeMove(move);
            value = std::min(value, alphaBeta(board, depth-1, alpha, beta, true));
            board.unmakeMove(move);

            if (value < alpha) {
                break;
            }
            beta = std::min(beta, value);
        }
    }

    return value;
}

int evaluate(chess::Board& board) {
    int eval = 0;

    for (int i = 0; i < 64; i++) {
        auto square = (chess::Square)i;
        int piece = (int)board.at(square);

        if (piece != 12) {
            eval += materialValues[piece];
        }
    }

    return eval;
}