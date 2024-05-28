#include "libraries/chess.hpp"
#include <map>
#include <string>

using namespace chess;

void printBoard(Board &board, Color color);
Square getSquareByColor(Color color, int row, int col);

std::map<chess::Piece, std::string> piece_map =
{
    {Piece::BLACKPAWN, "p"},
    {Piece::BLACKKNIGHT, "k"},
    {Piece::BLACKROOK, "r"},
    {Piece::BLACKBISHOP, "b"},
    {Piece::BLACKQUEEN, "q"},
    {Piece::BLACKKING, "k"},
    {Piece::WHITEPAWN, "P"},
    {Piece::WHITEKNIGHT, "K"},
    {Piece::WHITEROOK, "R"},
    {Piece::WHITEBISHOP, "B"},
    {Piece::WHITEQUEEN, "Q"},
    {Piece::WHITEKING, "K"},
    {Piece::NONE, " "}
};

int main() {
    Board board = Board(constants::STARTPOS);

    printBoard(board, Color::WHITE);

    return 0;
}

/*
    Depending on who's perspective the printed board should be based off of
    we provide a color argument which is the color of the human playing the bot
*/
void printBoard(Board& board, Color color) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece piece = board.at(getSquareByColor(color, row, col));
            std::cout << " " << piece_map[piece]; 
        }
        std::cout << std::endl;
    }
}

Square getSquareByColor(Color color, int row, int col) {
    if (color == Color::WHITE) {
        return Square(63- (row * 8 + col));
    } else {
        return Square(row * 8 + col);
    }
}