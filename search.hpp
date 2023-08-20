#include "evaluation.hpp"
#include <climits>

int minimax(chess::Board &board, int depth, int alpha, int beta, bool isMaxPlayer) {
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);

    // Terminal Node
    auto result = board.isGameOver().second;
    if (result == chess::GameResult::DRAW) {
        return 0;
    }

    if (depth <= 0) {
        int value = evaluate(board);
        return value;
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