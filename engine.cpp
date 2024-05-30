#include "libraries/chess.hpp"
#include <map>
#include <string>
#include <climits>
#include <chrono>
#include <ctime>

using namespace chess;

uint64_t perft(Board& board, int depth);
void gameLoop(Board& board);
void playEngineWhite(Board& board);
void playEngineBlack(Board& board);
Move getEngineMove(Board& board, int depth, bool max);
Move getMove(Board& board);
bool isMoveLegal(Board& board, Move& move);
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

Color playerColor;

int main() {
    Board board = Board(constants::STARTPOS);
    gameLoop(board);

    // auto start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // std::cout << perft(board, 7) << std::endl;
    // auto end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // std::cout << (end - start) << "ms" << std::endl;

    return 0;
}

uint64_t perft(Board& board, int depth) {
    Movelist moves;
    movegen::legalmoves(moves, board);

    if (depth == 1) {
        return moves.size();
    }

    uint64_t nodes = 0;

    for (int i = 0; i < moves.size(); i++) {
        const auto move = moves[i];
        board.makeMove(move);
        nodes += perft(board, depth - 1);
        board.unmakeMove(move);
    }

    return nodes;
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

            Move engineMove(getEngineMove(board, 7, true));
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

            Move engineMove = getEngineMove(board, 7, false);
            board.makeMove(engineMove);
            lastEngMove = engineMove;
        }
    }
}

Move getEngineMove(Board& board, int depth, bool max) {
    int bestValue = (max) ? INT_MIN : INT_MAX;
    Move bestMove;

    Movelist moves;
    movegen::legalmoves(moves, board);

    for (int i = 0; i < moves.size(); i++) {
        const auto move = moves[i];
        
        board.makeMove(move);
        int value = minimax(board, depth, INT_MIN, INT_MAX, max);
        board.unmakeMove(move);

        if (max) { // Engine is white
            if (value > bestValue) {
                bestValue = value;
                bestMove = move;
            }
        } else {
            if (value < bestValue) {
                bestValue = value;
                bestMove = move;
            }
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