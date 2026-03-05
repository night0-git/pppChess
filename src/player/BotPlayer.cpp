#include "BotPlayer.hpp"
#include <chrono>
#include <thread>

BotPlayer::BotPlayer(int depth) : _bot(depth) {}

std::optional<Move> BotPlayer::getMove(Board& board, PieceColor color) {
    /*using ms = std::chrono::milliseconds;
    auto start = std::chrono::steady_clock::now();
    Move bestMove = _bot.getBestMove(board, color);
    auto end = std::chrono::steady_clock::now();
    
    auto elapsed_ms = std::chrono::duration_cast<ms>(end - start);
    auto minTime_ms = ms(700);
    if (elapsed_ms < minTime_ms) {
        std::this_thread::sleep_for(minTime_ms - elapsed_ms);
    }*/
    
    auto move = _bot.getBestMove(board, color);

    return move;
}