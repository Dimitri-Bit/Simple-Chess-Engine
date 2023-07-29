#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "thc.h"
#include <climits>
#include <chrono>

int alphaBeta(thc::ChessEvaluation &chessEvaluation, int depth, int alpha, int beta, bool maximize);
int evaluate(thc::ChessEvaluation &chessEvaluation);

bool white = false;

void display_position( thc::ChessEvaluation &cr, const std::string &description )
{
    std::string fen = cr.ForsythPublish();
    std::string s = cr.ToDebugStr();
    printf( "%s\n", description.c_str() );
    printf( "FEN (Forsyth Edwards Notation) = %s\n", fen.c_str() );
    printf( "Position = %s\n", s.c_str() );
}

int main() {
    thc::ChessEvaluation chessEvaluation;
    thc::Move mv;

    white = false; // Is computer white
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "test" << std::endl;

    while (true) {
        display_position(chessEvaluation, "");

        if (chessEvaluation.white == true) {
            std::string stringMove;
            std::cout << "What is your move? (White)" << std::endl;
            std::getline(std::cin, stringMove);
            mv.TerseIn(&chessEvaluation, stringMove.c_str());
            chessEvaluation.PlayMove(mv);
        } else {
            uint64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            std::vector<thc::Move> moves;
            chessEvaluation.GenLegalMoveListSorted(moves);

            int bestValue = INT_MIN;
            thc::Move bestMove;
            for (thc::Move move : moves) {
                chessEvaluation.PushMove(move);
                int value = alphaBeta(chessEvaluation, 6, INT_MIN, INT_MAX, true);
                chessEvaluation.PopMove(move);

                if (value > bestValue) {
                    bestMove = move;
                }
            }
            uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            int result = end - start;
            std::cout << "Best Move: " << bestMove.TerseOut() << "Time: " << result << std::endl;
            chessEvaluation.PlayMove(bestMove);
        }
    }
    return 0;
}

int alphaBeta(thc::ChessEvaluation &chessEvaluation, int depth, int alpha, int beta, bool maximize) {
    int bestValue;

    if (depth == 0) {
        return evaluate(chessEvaluation);
    }
    
    if (maximize) {
        bestValue = INT_MIN;

        std::vector<thc::Move> moves;
        chessEvaluation.GenLegalMoveListSorted(moves);

        for (thc::Move move : moves) {
            chessEvaluation.PushMove(move);
            int value = alphaBeta(chessEvaluation, depth - 1, alpha, beta, false);
            chessEvaluation.PopMove(move);
            bestValue = std::max(value, bestValue);
            alpha = std::max(alpha, bestValue);
            if (beta <= alpha) {
                break;
            }
        }
    } else {
        bestValue = INT_MAX;

        std::vector<thc::Move> moves;
        chessEvaluation.GenLegalMoveListSorted(moves);

        for (thc::Move move : moves) {
            chessEvaluation.PushMove(move);
            int value = alphaBeta(chessEvaluation, depth - 1, alpha, beta, true);
            chessEvaluation.PopMove(move);
            bestValue = std::min(value, bestValue);
            beta = std::min(beta, bestValue);

            if (beta <= alpha) {
                break;
            }
        }
    }
    return bestValue;
}

int evaluate(thc::ChessEvaluation &chessEvaluation) {
    int score = 0;

    for (int i = 0; i < 63; i++) {
        if (isupper(chessEvaluation.squares[i])) {
            score = score - 1;
        } else {
            score = score + 1;
        }
    }
    return score;
}