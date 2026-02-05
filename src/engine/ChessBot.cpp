#include "ChessBot.hpp"
#include <climits>
#include <algorithm>

ChessBot::ChessBot(int maxDepth) : _maxDepth(maxDepth) {}

Move ChessBot::getBestMove(Board& board) {
    auto moves = board.getAllValidMoves();
    Move bestMove = {{0, 0}, {0, 0}};

    PieceColor botColor = PieceColor::Black;
    bool isMaximizing = botColor == PieceColor::White;
    int bestEval = isMaximizing ? INT_MIN : INT_MAX;

    for (const auto& move : moves) {
        board.applyMoveInternal(move);
        if (board.isChecked(botColor)) {
            board.undoLastMoveInternal();
            continue;
        }

        int eval = alphaBeta(board, _maxDepth - 1, INT_MIN, INT_MAX, !isMaximizing);

        if (isMaximizing) {
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        }
        else {
            if (eval < bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        }
    }
    return bestMove;
}

int ChessBot::alphaBeta(Board& board, int depth, int alpha, int beta, bool isMaximizing) {
    if (depth == 0) {
        return evaluate(board);
    }

    auto moves = board.getAllValidMoves();

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (const auto& move : moves) {
            board.applyMoveInternal(move);

            if (board.isChecked(board.getPieceAt(move.dest)->color())) {
                board.undoLastMoveInternal();
                continue;
            }

            int eval = alphaBeta(board, depth - 1, alpha, beta, false);
            board.undoLastMoveInternal();

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha) {
                break; // Skip the rest of the moves in this branch
            }
        }
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        for (const auto& move : moves) {
            board.applyMoveInternal(move);
            
            if (board.isChecked(board.getPieceAt(move.dest)->color())) {
                board.undoLastMoveInternal();
                continue;
            }

            int eval = alphaBeta(board, depth - 1, alpha, beta, true);
            board.undoLastMoveInternal();

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            if (beta <= alpha) {
                break; // Skip the rest of the moves in this branch
            }
        }
        return minEval;
    }
}


int ChessBot::evaluate(const Board& board) {   
    int whiteVal = 0;
    int blackVal = 0;
    bool whiteHasQueen = false;
    bool blackHasQueen = false;
    int whiteMinorCount = 0;
    int blackMinorCount = 0;
    int whiteCount = 0;
    int blackCount = 0;

    for (int row = 0; row < Board::SIZE; row++) {
        for (int col = 0; col < Board::SIZE; col++) {
            auto pcs = board.getPieceAt({row, col});
            if (!pcs) continue;

            if (pcs->color() == PieceColor::White) {
                whiteVal += _pieceValue[static_cast<size_t>(pcs->type())];
                switch (pcs->type()) {
                case PieceType::Pawn:
                    whiteVal += _pawnTable[row * 8 + col];
                    break;
                case PieceType::Knight:
                    whiteMinorCount++;
                    whiteVal += _knightTable[row * 8 + col];
                    break;
                case PieceType::Bishop:
                    whiteMinorCount++;
                    whiteVal += _bishopTable[row * 8 + col];
                    break;
                case PieceType::Rook:
                    whiteVal += _rookTable[row * 8 + col];
                    break;
                case PieceType::Queen:
                    whiteHasQueen = true;
                    whiteVal += _queenTable[row * 8 + col];
                    break;
                }
                whiteCount++;
            }

            else {
                blackVal += _pieceValue[static_cast<size_t>(pcs->type())];
                switch (pcs->type()) {
                case PieceType::Pawn:
                    blackVal += _pawnTable[(7 - row) * 8 + col];
                    break;
                case PieceType::Knight:
                    blackMinorCount++;
                    blackVal += _knightTable[(7 - row) * 8 + col];
                    break;
                case PieceType::Bishop:
                    blackMinorCount++;
                    blackVal += _bishopTable[(7 - row) * 8 + col];
                    break;
                case PieceType::Rook:
                    blackVal += _rookTable[(7 - row) * 8 + col];
                    break;
                case PieceType::Queen:
                    blackHasQueen = true;
                    blackVal += _queenTable[(7 - row) * 8 + col];
                    break;
                }
                blackCount++;
            }
        }
    }
    
    // Evaluate kings
    /*
    * Endgame is marked either when:
    * - Both sides have no queens
    * - Every side which has a queen has additionally no other pieces
    * or one minorpiece maximum.
    */
    bool isEndgame = (!whiteHasQueen && !blackHasQueen) ||
    (whiteHasQueen && whiteCount <= 3 && whiteMinorCount <= 1) ||
    (blackHasQueen && blackCount <= 3 && blackMinorCount <= 1);
    auto whiteKingPos = board.whiteKingPos();
    auto blackKingPos = board.blackKingPos();
    if (isEndgame) {
        whiteVal += _kingTableEndGame[whiteKingPos.x * 8 + whiteKingPos.y];
        blackVal += _kingTableEndGame[(7 - blackKingPos.x) * 8 + blackKingPos.y];
    } else {
        whiteVal += _kingTableMiddleGame[whiteKingPos.x * 8 + whiteKingPos.y];
        blackVal += _kingTableMiddleGame[(7 - blackKingPos.x) * 8 + blackKingPos.y];
    }

    return whiteVal - blackVal;
}