#include "chess.hpp"
#include <iostream>
#include <chrono>
#include <climits>

// Values used by the Crafty Chess Engine
#  define PAWN_VALUE                             100;
#  define KNIGHT_VALUE                           325;
#  define BISHOP_VALUE                           325;
#  define ROOK_VALUE                             500;
#  define QUEEN_VALUE                           1050;
#  define KING_VALUE                           40000;

# define WHITE_COLOR chess::Color::WHITE;
# define BLACK_COLOR chess::Color::BLACK;

int evaluate(chess::Board &board);
int alphaBeta(chess::Board &board, int depth, int alpha, int beta, bool maximize);
uint64_t perft(chess::Board& board, int depth);

chess::Color computerColor;

int main() {
    chess::Board board = chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    char sideCHar;

    std::cout << "Which side is the computer playing? W/B: ";
    std::cin >> sideCHar;

    if ((char) tolower(sideCHar) == 'w') {
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

            int bestValue = INT_MIN;
            chess::Move bestMove;
            for (const auto &move : moves) {
                std::cout << chess::uci::moveToSan(board, move) << std::endl;
                board.makeMove(move);
                int value = alphaBeta(board, 3, INT_MIN, INT_MAX, true);
                board.unmakeMove(move);

                if (value > bestValue) {
                    bestMove = move;
                    bestValue = value;
                    std::cout << "Best Value: " << bestValue << "Move: " << chess::uci::moveToSan(board, move) << std::endl;
                }
            }
            uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            int result = end - start;
            std::cout << "Best Move: " << chess::uci::moveToLan(board, bestMove) << std::endl;
            std::cout << "Time: " << result << std::endl;
            board.makeMove(bestMove);
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
    int whiteScore = 0;
    int blackScore = 0;
    int score = 0;

    for (int i = 0; i < 65; i++) {
        chess::Piece piece = board.at(chess::Square(i));

        switch (piece) {
        case chess::Piece::WHITEPAWN:
            score -= PAWN_VALUE;
            break;

        case chess::Piece::BLACKPAWN:
            score += PAWN_VALUE;
            break;

        case chess::Piece::WHITEKNIGHT:
            score -= KNIGHT_VALUE;
            break;

        case chess::Piece::BLACKKNIGHT:
            score += KNIGHT_VALUE;
            break;

        case chess::Piece::WHITEBISHOP:
            score -= BISHOP_VALUE;
            break;

        case chess::Piece::BLACKBISHOP:
            score += BISHOP_VALUE;
            break;

        case chess::Piece::WHITEROOK:
            score -= ROOK_VALUE;
            break;

        case chess::Piece::BLACKROOK:
            score += ROOK_VALUE;
            break;

        case chess::Piece::WHITEQUEEN:
            score -= QUEEN_VALUE;
            break;

        case chess::Piece::BLACKQUEEN:
            score += QUEEN_VALUE;
            break;

        case chess::Piece::WHITEKING:
            score -= KING_VALUE;
            break;

        case chess::Piece::BLACKKING:
            score += KING_VALUE;
            break;
        
        default:
            break;
        }
    }

    return score;
}

int alphaBeta(chess::Board &board, int depth, int alpha, int beta, bool maximize) {
    int bestValue;

    if (depth <= 0) {
        return evaluate(board);
    }

    if (maximize) {
        bestValue = INT_MIN;

        chess::Movelist moves;
        chess::movegen::legalmoves(moves, board);

        for (const auto &move : moves) {
            board.makeMove(move);
            int value = alphaBeta(board, depth - 1, alpha, beta, false);
            board.unmakeMove(move);
            bestValue = std::max(value, bestValue);
            alpha = std::max(alpha, bestValue);

            if (beta <= alpha) {
                break;
            }
        }
    } else {
        bestValue = INT_MAX;

        chess::Movelist moves;
        chess::movegen::legalmoves(moves, board);

        for (const auto &move : moves) {
            board.makeMove(move);
            int value = alphaBeta(board, depth - 1, alpha, beta, true);
            board.unmakeMove(move);
            bestValue = std::min(value, bestValue);
            beta = std::min(beta, bestValue);

            if (beta <= alpha) {
                break;
            }
        }
    }

    return bestValue;
}

uint64_t perft(chess::Board& board, int depth) {
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);

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