#include "libraries/chess.hpp"
#include <map>
#include <string>

using namespace chess;

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

int main() {
    Board board = Board(constants::STARTPOS);

    std::cout << eval(board) << std::endl;

    return 0;
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

Square getSquareByColor(Color color, int row, int col) {
    if (color == Color::BLACK) {
        return Square(63- (row * 8 + col));
    } else {
        return Square(row * 8 + col);
    }
}