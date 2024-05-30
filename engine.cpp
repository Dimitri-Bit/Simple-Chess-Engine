#include "libraries/chess.hpp"
#include <map>
#include <string>
#include <climits>
#include <chrono>

using namespace chess;

int minimax(Board& board, int depth, int alpha, int beta, bool max);
int eval(Board& board);
void printBoard(Board &board, Color color);
void fillBoard(Board& board, std::string (&boardArr)[8][8], Color color);
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

int main() {
    Board board = Board(constants::STARTPOS);
    board.makeMove(uci::parseSan(board, "e2e4"));
    board.makeMove(uci::parseSan(board, "e7e6"));
    printBoardTwo(board, Color::WHITE);

    return 0;
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

void printBoard(Board& board, Color color) {
    std::string boardArr[8][8];
    fillBoard(board, boardArr, color);

    for (int row = 0; row < 8; row++) {
        int rowNumber = (color == Color::BLACK) ? (row + 1) : (8 - row);
        std::cout << rowNumber << " ";

        for (int col = 0; col < 8; col++) {
            std::cout << " " << boardArr[row][col];
        }
        std::cout << std::endl;
    }

    std::string letters = (color == Color::WHITE) ? "\n   A B C D E F G H\n" : "\n   H G F E D C B A\n";
    std::cout << letters << std::endl;
}

void fillBoard(Board& board, std::string (&boardArr)[8][8], Color color) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece piece = board.at(getSquareByColor(color, row, col));
            boardArr[row][(7-col)] = pieceCodeMap[piece]; // Mirror board since its like not positioned right for some reason??
        }
    }
}

Square getSquareByColor(Color color, int row, int col) {
    if (color == Color::BLACK) {
        return Square(row * 8 + col);
    } else {
        return Square(63 - (row * 8 + col));
    }
}