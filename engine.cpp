#include "libraries/chess.hpp"
#include <map>
#include <string>
#include <climits>
#include <chrono>

using namespace chess;

void startGame();
int eval(Board& board);
void printBoard(Board &board, Color color);
Square getSquareByColor(Color color, int row, int col);

std::map<chess::Piece, std::string> pieceCodeMap =
{
    {Piece::BLACKPAWN, "\u265F"},
    {Piece::BLACKKNIGHT, "\u265E"},
    {Piece::BLACKROOK, "\u265C"},
    {Piece::BLACKBISHOP, "\u265D"},
    {Piece::BLACKQUEEN, "\u265B"},
    {Piece::BLACKKING, "\u265A"},
    {Piece::WHITEPAWN, "\u2659"},
    {Piece::WHITEKNIGHT, "\u2658"},
    {Piece::WHITEROOK, "\u2656"},
    {Piece::WHITEBISHOP, "\u2657"},
    {Piece::WHITEQUEEN, "\u2655"},
    {Piece::WHITEKING, "\u2654"},
    {Piece::NONE, "-"}
};

std::map<chess::Piece, int> pieceMateriaMap =
{
    {Piece::WHITEPAWN, 100},
    {Piece::WHITEKNIGHT, 350},
    {Piece::WHITEBISHOP, 350},
    {Piece::WHITEROOK, 525},
    {Piece::WHITEQUEEN, 1000},
    {Piece::WHITEKING, 100000},
    {Piece::BLACKPAWN, -100},
    {Piece::BLACKKNIGHT, -350},
    {Piece::BLACKBISHOP, -350},
    {Piece::BLACKROOK, -525},
    {Piece::BLACKQUEEN, -1000},
    {Piece::BLACKKING, -100000}
};

bool white;

int main() {
    Board board = Board(constants::STARTPOS);
    startGame();

    // std::cout << eval(board) << std::endl;

    // const auto start = std::chrono::high_resolution_clock::now();

    // int result = eval(board);

    // const auto end = std::chrono::high_resolution_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << "ns\n";

    return 0;
}

void startGame() {
    Board board = Board(constants::STARTPOS);

    std::cout << "Which color would you like to play? (W/B): ";
    char color;
    std::cin >> color;

    if ((char)tolower(color) == 'w') {
        white = true;
        std::cout << "You are now playing as white" << std::endl;
    } else {
        white = false;
        std::cout << "You are now playing as black" << std::endl;
    }

    bool whitesTurn = true;
    while (true) {

    }
}

int minimax(Board& board, int depth, int alpha, int beta, bool max) {
    Movelist moves;
    movegen::legalmoves(moves, board);

    if (depth <= 1) {
        return eval(board);
    }

    int bestValue;

    if (max) { // White
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

int eval(Board& board) {
    int value = 0;

    for (int i = 0; i <= 64; i++) {
        Piece piece = board.at(i);
        if (piece != Piece::NONE) {
            value += pieceMateriaMap[piece];
        }
    }

    return value;
}

/*
    Print chess board & coordinates depending on the given color perspective
*/
void printBoard(Board& board, Color color) {
    std::cout << "   A B C D E F G H\n" << std::endl;
    for (int row = 0; row < 8; row++) {
        int rowNumber = (color == Color::BLACK) ? (row + 1) : (8 - row);
        std::cout << rowNumber << " ";
        for (int col = 0; col < 8; col++) {
            Piece piece = board.at(getSquareByColor(color, row, col));
            std::cout << " " << pieceCodeMap[piece];
        }
        std::cout << std::endl;
    }
}

// Todo: write shorthand cuz this is ugly
Square getSquareByColor(Color color, int row, int col) {
    if (color == Color::BLACK) {
        return Square(63- (row * 8 + col));
    } else {
        return Square(row * 8 + col);
    }
}