#include "libraries/chess.hpp"
#include <climits>

int minimax(chess::Board &board, int depth, int alpha, int beta, bool isMaxPlayer);
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

int minimax(chess::Board &board, int depth, int alpha, int beta, bool isMaxPlayer) {
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);

    if (depth <= 0) {
        return evaluate(board);
    }

    int bestValue;
    if (isMaxPlayer) {
        bestValue = INT_MIN;
        for (int i = 0; i < moves.size(); i++) {
            const auto move = moves[i];
            board.makeMove(move);
            int value = minimax(board, depth - 1, alpha, beta, false);
            board.unmakeMove(move);
            bestValue = std::max(bestValue, value);
            alpha = std::max(alpha, bestValue);

            if (beta <= alpha) {
                break;
            }
        }
    } else {
        bestValue = INT_MAX;
        for (int i = 0; i < moves.size(); i++) {
            const auto move = moves[i];
            board.makeMove(move);
            int value = minimax(board, depth - 1, alpha, beta, true);
            board.unmakeMove(move);
            bestValue = std::min(bestValue, value);
            beta = std::min(beta, value);

            if (beta <= alpha) {
                break;
            }
        }
    }
    return bestValue;
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