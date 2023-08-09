#include "chess.hpp"
#include <iostream>
#include <chrono>
#include <climits>

#  define PAWN_VALUE                             100
#  define KNIGHT_VALUE                           300
#  define BISHOP_VALUE                           300
#  define ROOK_VALUE                             500
#  define QUEEN_VALUE                            900
#  define KING_VALUE                           90000

# define WHITE_COLOR chess::Color::WHITE
# define BLACK_COLOR chess::Color::BLACK

int evaluate(chess::Board &board);
int minimax(chess::Board &board, int depth, int alpha, int beta, bool isMaxPlayer);

chess::Color computerColor;

int main() {
    chess::Board board = chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    char side;

    std::cout << "Which side is the computer playing? W/B: ";
    std::cin >> side;

    if ((char) tolower(side) == 'w') {
        computerColor = chess::Color::WHITE;
        std::cout << "white" << std::endl;
    } else {
        computerColor = chess::Color::BLACK;
        std::cout << "black" << std::endl;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        chess::utils::printBitboard(board.all());
        std::cout << "FEN: " << board.getFen() << std::endl;

        if (board.sideToMove() == computerColor) {
            uint64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            
            chess::Movelist moves;
            chess::movegen::legalmoves(moves, board);

            if (computerColor == chess::Color::WHITE) {
                int bestValue = INT_MIN;
                chess::Move bestMove;
                    
                for (int i = 0; i < moves.size(); i++) {
                    const auto move = moves[i];
                    board.makeMove(move);
                    int value = minimax(board, 6, INT_MIN, INT_MAX, true);
                    board.unmakeMove(move);
                    
                    if (value > bestValue) {
                        bestMove = move;
                        bestValue = value;
                        std::cout << "Best Value: " << bestValue << " Best Move: " << chess::uci::moveToLan(board, bestMove) << std::endl;
                    }
                }
                uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                int result = end - start;
                std::cout << "Final Best Move: " << chess::uci::moveToLan(board, bestMove) << std::endl;
                std::cout << "Time: " << result << std::endl;
                board.makeMove(bestMove);
            } else {
                int bestValue = INT_MAX;
                chess::Move bestMove;

                for (int i = 0; i < moves.size(); i++) {
                    const auto move = moves[i];
                    board.makeMove(move);
                    int value = minimax(board, 6, INT_MIN, INT_MAX, false);
                    board.unmakeMove(move);

                    if (bestValue > value) {
                        bestMove = move;
                        bestValue = value;
                        std::cout << "Best Value: " << bestValue << " Best Move: " << chess::uci::moveToLan(board, bestMove) << std::endl;
                    }
                }
                uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                int result = end - start;
                std::cout << "Final Best Move: " << chess::uci::moveToLan(board, bestMove) << std::endl;
                std::cout << "Time: " << result << std::endl;
                board.makeMove(bestMove);
            }
        } else {
            std::string stringMove;
            std::cout << "What is your move?" << std::endl;
            std::getline(std::cin, stringMove);
            board.makeMove(chess::uci::parseSan(board, stringMove));
        }
    }
    return 0;
}

int evaluate(chess::Board &board) {
    int score = 0;

    for (int i = 0; i < 63; i++) {
        chess::Piece piece = board.at(chess::Square(i));

        switch (piece) {

            case chess::Piece::WHITEPAWN:
                score = score + PAWN_VALUE;
                break;

            case chess::Piece::BLACKPAWN:
                score = score - PAWN_VALUE;
                break;

            case chess::Piece::WHITEKNIGHT:
                score = score + KNIGHT_VALUE;
                break;

            case chess::Piece::BLACKKNIGHT:
                score = score - KNIGHT_VALUE;
                break;

            case chess::Piece::WHITEBISHOP:
                score = score + BISHOP_VALUE;
                break;

            case chess::Piece::BLACKBISHOP:
                score = score - BISHOP_VALUE;
                break;

            case chess::Piece::WHITEROOK:
                score = score + ROOK_VALUE;
                break;

            case chess::Piece::BLACKROOK:
                score = score - ROOK_VALUE;
                break;

            case chess::Piece::WHITEQUEEN:
                score = score + QUEEN_VALUE;
                break;

            case chess::Piece::BLACKQUEEN:
                score = score - QUEEN_VALUE;
                break;

            case chess::Piece::WHITEKING:
                score = score + KING_VALUE;
                break;

            case chess::Piece::BLACKKING:
                score = score - KING_VALUE;
                break;

            default:
                break;
        }
    }

    return score;
}

int minimax(chess::Board &board, int depth, int alpha, int beta, bool isMaxPlayer) {
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);
    
    if (depth <= 0) {
        int value = evaluate(board);
        return value;
    }

    // In check extension
    if (board.inCheck()) {
        depth++;
    }

    // Repetition check
    if (board.isRepetition()) {
        return 0;
    }

    int bestValue;
    if (isMaxPlayer) {
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

