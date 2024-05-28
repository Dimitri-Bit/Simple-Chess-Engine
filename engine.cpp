#include "libraries/chess.hpp"
#include <map>
#include <string>

using namespace chess;

void printBoard(Board &board, Color color);
Square getSquareByColor(Color color, int row, int col);

std::map<chess::Piece, std::string> piece_map =
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

int main() {
    Board board = Board(constants::STARTPOS);
    
    printBoard(board, Color::WHITE);

    return 0;
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
            std::cout << " " << piece_map[piece];
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