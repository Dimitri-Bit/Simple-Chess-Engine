#include "libraries/chess.hpp"
#include <climits>

int alphaBeta(chess::Board& board, int depth, int alpha, int beta, int max);
int evaluate(chess::Board& board);

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
    return 2;
}