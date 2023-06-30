#include <math.h>

#include "connect_four.cpp"

std::pair<int, ConnectFour::Move> minimax(
    const ConnectFour &game,
    int alpha,
    int beta,
    int maxDepth = 10)
{
    if (maxDepth == 0)
        return {game.evaluate(), -1};

    if (game.isGameOver())
        return {game.utility(), -1};

    if (game.isMaxTurn())
    {
        auto bestScore = std::numeric_limits<int>::min();
        auto bestMove = -1;

        for (auto &&move : game.getPossibleMoves())
        {
            auto nextState = game;
            nextState.makeMove(move);
            auto evaluatedMove = minimax(nextState, alpha, beta, maxDepth - 1);

            if (evaluatedMove.first > bestScore)
            {
                bestScore = evaluatedMove.first;
                bestMove = move;
            }

            alpha = std::max(alpha, bestScore);

            if (beta <= alpha)
                break;
        }
        return {bestScore, bestMove};
    }
    else
    {
        auto bestScore = std::numeric_limits<int>::max();
        auto bestMove = -1;

        for (auto &&move : game.getPossibleMoves())
        {
            auto nextState = game;
            nextState.makeMove(move);
            auto evaluatedMove = minimax(nextState, alpha, beta, maxDepth - 1);

            if (evaluatedMove.first < bestScore)
            {
                bestScore = evaluatedMove.first;
                bestMove = move;
            }

            beta = std::min(beta, bestScore);

            if (beta <= alpha)
                break;
        }
        return {bestScore, bestMove};
    }
}
