/*
    A lot of the chess.hpp library usage and engine knowladge has been taken from Raphael
    GitHub: https://github.com/Orbital-Web/Raphael/tree/main
*/

#include "search.hpp"
#include "constants.hpp"
#include <iostream>
#include <chrono>

#define WHITE_COLOR chess::Color::WHITE
#define BLACK_COLOR chess::Color::BLACK

chess::Color computerColor;

void getUserInput();
chess::Move getBestMove(chess::Board &board);
uint64_t getCurrentTime();

int main() {
    chess::Board board = chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    getUserInput();

    /* Clear std::cin */
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        if (board.sideToMove() == computerColor) {
            chess::Move move = getBestMove(board);
            board.makeMove(move);
        } else {
            std::string moveString;
            std::cout << "What is your move?" << std::endl;
            std::getline(std::cin, moveString);
            board.makeMove(chess::uci::parseSan(board, moveString));
        }

        chess::utils::printBitboard(board.all());
        std::cout << "FEN: " << board.getFen() << std::endl;
    }
    return 0;
}

chess::Move getBestMove(chess::Board &board) {
    uint64_t start = getCurrentTime();
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);

    int bestScore;
    chess::Move bestMove;

    if(computerColor == WHITE_COLOR) {
        bestScore = INT_MIN;

        for (int i = 0; i < moves.size(); i++) {
            const auto move = moves[i];
            board.makeMove(move);
            int score = minimax(board, MAX_DEPTH, INT_MIN, INT_MAX, true);
            board.unmakeMove(move);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
                std::cout << "Best Score (" << bestScore << ")" << std::endl;
            }
        }
    } else {
        bestScore = INT_MAX;

        for (int i = 0; i < moves.size(); i++) {
            const auto move = moves[i];
            board.makeMove(move);
            int score = minimax(board, MAX_DEPTH, INT_MIN, INT_MAX, false);
            board.unmakeMove(move);

            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
                std::cout << "Best Score (" << bestScore << ")" << std::endl;
            }
        }
    }

    int result = getCurrentTime() - start;
    std::cout << std::endl;
    std::cout << "Best Move (" << chess::uci::moveToLan(board, bestMove) << ")" << std::endl;
    std::cout << "Best Score (" << bestScore << ")" << std::endl;
    std::cout << "Time (" << result << "ms)" << std::endl;
    std::cout << "Depth (" << MAX_DEPTH << ")\n" << std::endl;
    return bestMove;
}

void getUserInput() {
    char side;
    std::cout << "Which side is the computer playing? W/B" << std::endl;
    std::cin >> side;
    
    if((char) tolower(side) == 'w') {
        computerColor = WHITE_COLOR;
        std::cout << "Computer will be playing White" << std::endl;
    } else {
        computerColor = BLACK_COLOR;
        std::cout << "Computer will be playing Black" << std::endl;
    }
}

uint64_t getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}