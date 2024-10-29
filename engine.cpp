#include <iostream>
#include <string>
#include <climits>
#include "library/chess.hpp"

void debugMsg(std::string message);
chess::Move getBestMove(chess::Board& board, chess::Color& playerColor);
int minimax(chess::Board& board, int depth, int alpha, int beta, bool maximize);
int evaluate(chess::Board& board);
bool isMoveLegal(chess::Board& board, chess::Move& move);
bool isGameOver(chess::Board& board);
std::string inputToStr(std::string msg);
chess::Move readUciMove(const chess::Board& board);

bool debug = false;
const int DEPTH = 4;

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

// Note: The way the eval function sees the board, in the starting position, the black figures are on the bottom!
const int wPawnTable[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10,-20,-20, 10, 10,  5,
    5, -5,-10,  0,  0,-10, -5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5,  5, 10, 25, 25, 10,  5,  5,
   10, 10, 20, 30, 30, 20, 10, 10,
   50, 50, 50, 50, 50, 50, 50, 50,
    0,  0,  0,  0,  0,  0,  0,  0
};

const int bPawnTable[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
  -50,-50,-50,-50,-50,-50,-50,-50, 
  -10,-10,-20,-30,-30,-20,-10,-10,
   -5, -5,-10,-25,-25,-10, -5, -5,
    0,  0,  0,-20,-20,  0,  0,  0,
   -5,  5, 10,  0,  0, 10,  5, -5,
   -5,-10,-10, 20, 20,-10,-10, -5,
    0,  0,  0,  0,  0,  0,  0,  0
};

const int* positionalValues[2] = {
    wPawnTable,
    bPawnTable
};

int main() {
    if (inputToStr("Do you want to enable debug messages? y/n")[0] == 'y') {
        std::cout << "Debug messages are turned on." << std::endl;
        debug = true;
    } else {
        std::cout << "Debug messages will be turned off." << std::endl;
    }

    chess::Color playerColor;
    switch(inputToStr("Which color would you like to play as? w/b")[0]) {
        case 'w':
            playerColor = chess::Color::WHITE;
            std::cout << "Playing as white." << std::endl;
        break;

        case 'b':
            playerColor = chess::Color::BLACK;
            std::cout << "Playing as black." << std::endl;
        break;

        default:
            playerColor = chess::Color::WHITE;
            std::cout << "Invalid input, defaulting to white." << std::endl;
        break;
    }

    chess::Board board = chess::Board(chess::constants::STARTPOS);
    while(!isGameOver(board)) {
        if (board.sideToMove() == playerColor) {
            std::cout << board << std::endl;
            chess::Move move = readUciMove(board);
            if (!isMoveLegal(board, move)) {
                std::cout << "Illegal move, try again." << std::endl;
                continue;
            }
            board.makeMove(move);
        } else {
            chess::Move move = getBestMove(board, playerColor);
            std::cout << "\nEngine Move: " << chess::uci::moveToUci(move, false) << "\n" << std::endl;
            board.makeMove(move);
        }
    }

    return 0;
}

void debugMsg(std::string message) {
    if (debug) {
        std::cout << "# " << message << std::endl;
    }
}

chess::Move getBestMove(chess::Board& board, chess::Color& playerColor) {
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);
    chess::Move bestMove = moves[0];

    if (playerColor == chess::Color::WHITE) { // engine black
        int bestScore = INT_MAX;
        for (chess::Move move : moves) {
            board.makeMove(move);
            int score = minimax(board, DEPTH, INT_MIN, INT_MAX, false);
            board.unmakeMove(move);
            debugMsg("s: " + std::to_string(score) + " m: " + chess::uci::moveToUci(move, false));
            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
    } else {
        int bestScore = INT_MIN;
        for (chess::Move move : moves) {
            board.makeMove(move);
            int score = minimax(board, DEPTH, INT_MIN, INT_MAX, true);
            board.unmakeMove(move);
            debugMsg("s: " + std::to_string(score) + " m: " + chess::uci::moveToUci(move, false));
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
    }

    return bestMove;
}

int minimax(chess::Board& board, int depth, int alpha, int beta, bool maximize) {
    if (depth == 0 || isGameOver(board)) {
        return evaluate(board);
    }

    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);

    if (maximize) {
        int maxEval = INT_MIN;
        for (const auto& move : moves) {
            board.makeMove(move);
            int eval = minimax(board, depth - 1, alpha, beta, false);
            board.unmakeMove(move);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha) {
                break;
            }
        }

        if (maxEval > (KING + 1) || maxEval < -(KING + 1)) {
            debugMsg("Max Eval??: " + maxEval);
        }

        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto& move : moves) {
            board.makeMove(move);
            int eval = minimax(board, depth - 1, alpha, beta, true);
            board.unmakeMove(move);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if (beta <= alpha) {
                break;
            }
        }

        if (minEval > (KING + 1) || minEval < -(KING + 1)) {
            debugMsg("Min Eval??: " + minEval);
        }

        return minEval;
    }
}

int evaluate(chess::Board& board) {
    int eval = 0;

    for (int i = 0; i < 64; i++) {
        auto square = (chess::Square)i;
        int piece = (int)board.at(square);

        if (piece != 12) { // 12 = space
            eval += materialValues[piece];

            if (piece == 0) {
                eval += positionalValues[0][i];
            }

            if (piece == 6) {
                eval += positionalValues[1][i];
            }
        }
    }

    return eval;
}

bool isMoveLegal(chess::Board& board, chess::Move& move) {
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);

    for (const auto &m : moves) {
        if (m == move) {
            return true;
        }
    }
    return false;
}

bool isGameOver(chess::Board& board) {
    std::pair<chess::GameResultReason, chess::GameResult> gameResults = board.isGameOver();
    if (gameResults.second == chess::GameResult::NONE) {
        return false;
    }
    return true;
}

std::string inputToStr(std::string msg) {
    std::cout << msg << std::endl;
    std::string input;
    std::cin >> input;
    return input;
}

chess::Move readUciMove(const chess::Board& board) {
    std::string moveStr = inputToStr("Enter move:");
    chess::Move move = chess::uci::uciToMove(board, moveStr);
    return move;
}