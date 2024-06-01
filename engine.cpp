#include "libraries/chess.hpp"
#include <map>
#include <string>
#include <climits>
#include <chrono>
#include <ctime>

using namespace chess;

void gameLoop(Board& board);
void playEngineWhite(Board& board);
void playEngineBlack(Board& board);
Move getEngineMove(Board& board, int depth, int color);
Move getMove(Board& board);
bool isMoveLegal(Board& board, Move& move);
void printBoard(Board &board, Color color);
void fillBoard(Board& board, std::string (&boardArr)[8][8], Color color);

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

Color playerColor;

int main() {
    Board board = Board(constants::STARTPOS);
    printBoard(board, Color::WHITE);

    return 0;
}

void gameLoop(Board& board) {
    std::cout << "What do you want to play? (w/b): ";
    char input;
    std::cin >> input;

    playerColor = ((char)tolower(input) == 'w') ? Color::WHITE : Color::BLACK; 
    std::cout << "You are playing as *" << playerColor.internal() << "*" << std::endl;
    
    if (playerColor == Color::WHITE) {
        playEngineBlack(board);
    } else {
        playEngineWhite(board);
    }
}

void playEngineWhite(Board& board) {
    bool whitesTurn = true;
    Move lastEngMove = Move::NULL_MOVE;

    while(true) {
        if (whitesTurn) { // Engine
            whitesTurn = false;

            Move engineMove(getEngineMove(board, 3, 1));
            board.makeMove(engineMove);
            lastEngMove = engineMove;
        } else {
            whitesTurn = true;

            printBoard(board, playerColor);
            if (lastEngMove != Move::NULL_MOVE) {
                std::cout << "Engine Move: " << lastEngMove.from() << lastEngMove.to() << std::endl;
            }

            board.makeMove(getMove(board));
        }
    }
}

void playEngineBlack(Board& board) {
    bool whitesTurn = true;
    Move lastEngMove = Move::NULL_MOVE;

    while (true) {
        if (whitesTurn) { // Player
            whitesTurn = false;

            printBoard(board, playerColor);
            if (lastEngMove != Move::NULL_MOVE) {
                std::cout << "Engine Move: " << lastEngMove.from() << lastEngMove.to() << std::endl;
            }

            board.makeMove(getMove(board));
        } else {
            whitesTurn = true;

            Move engineMove = getEngineMove(board, 3, -1);
            board.makeMove(engineMove);
            lastEngMove = engineMove;
        }
    }
}

Move getEngineMove(Board& board, int depth, int color) {
    int bestValue = INT_MIN;
    Move bestMove;

    Movelist moves;
    movegen::legalmoves(moves, board);

    for (int i = 0; i < moves.size(); i++) {
        const auto move = moves[i];

        board.makeMove(move);
        int value = negamax(board, 5, INT_MIN, INT_MAX, color);
        board.unmakeMove(move);

        if (value > bestValue) {
            bestValue = value;
            bestMove = move;
        }
    }

    return bestMove;
}

Move getMove(Board& board) {
    while (true) {
        std::cout << "Enter move to play (SAN): ";
        std::string moveStr;
        std::cin >> moveStr;

        Move move = uci::parseSan(board, moveStr);
        
        if (isMoveLegal(board, move)) {
            return move;
        }

        std::cout << "Given move is not legal, please try again." << std::endl;
    }
}

bool isMoveLegal(Board& board, Move& move) {
    Movelist moves;
    movegen::legalmoves(moves, board);

    for (int i = 0; i < moves.size(); i++) {
        if (moves[i] == move) {
            return true;
        }
    }

    return false;
}

void printBoard(Board& board, Color color) {
    std::string boardArr[8][8];
    fillBoard(board, boardArr, color);

    std::cout << "\n\n\n";

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
            Square square = (color == Color::BLACK) ? Square(row * 8 + col) : (63 - (row * 8 + col));
            Piece piece = board.at(square);
            boardArr[row][(7-col)] = pieceCodeMap[piece]; // Mirror board since its like not positioned right for some reason??
        }
    }
}