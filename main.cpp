#include "connect_four.cpp"
#include <math.h>
#include <string>

#include "raylib.h"

std::string G_log = "";

std::pair<int, ConnectFour::Move> minimax(
    ConnectFour &game, int alpha, int beta, int maxDepth = 10)
{
    if (maxDepth == 0)
        return {game.evaluate(), -1};

    if (game.isGameOver())
        return {game.utility(), -1};

    if (game.isYourTurn())
    {
        int bestScore = MIN_VALUE;
        int bestMove = -1;
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
        int bestScore = MAX_VALUE;
        int bestMove = -1;
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

void handleMouseInput(ConnectFour &game)
{
    if (!game.isYourTurn())
        return;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        auto colSize = GetScreenWidth() / BOARD_SIZE;
        auto column = GetMousePosition().x / colSize;
        game.makeMove(column);
    }
}

void handleAI(ConnectFour &game)
{
    if (game.isYourTurn())
        return;

    auto bestMove = minimax(game, MIN_VALUE, MAX_VALUE);
    if (bestMove.second == -1)
        game.makeMove(game.getPossibleMoves()[0]);
    else
        game.makeMove(bestMove.second);

    if (bestMove.first == 0)
        G_log = "I CAN'T DO ANYTHING BETTER THAN A DRAW";
    if (bestMove.first == MAX_VALUE)
        G_log = "PLAYER 1 HAS A FORCED WIN";
    if (bestMove.first == MIN_VALUE)
        G_log = "PLAYER 2 HAS A FORCED WIN";
}

void handleGameOver(const ConnectFour &game)
{
    if (game.isGameOver())
    {
        if (game.checkConnectFour(ConnectFour::Piece::PLAYER1))
            G_log = "PLAYER 1 WON";
        else if (game.checkConnectFour(ConnectFour::Piece::PLAYER2))
            G_log = "PLAYER 2 WON";
        else
            G_log = "DRAW";

        G_log += ", PRESS \"R\" TO RESTART";
    }
}

void handleKeyboardInput(ConnectFour &game)
{
    if (IsKeyPressed(KEY_R))
    {
        game.reset();
        G_log = "";
    }
}

void render(const ConnectFour &game)
{
    float tile_w = GetScreenWidth() / BOARD_SIZE;
    float tile_h = GetScreenHeight() / BOARD_SIZE;

    for (int i = 1; i < BOARD_SIZE - 1; i++)
    {
        for (int j = 1; j < BOARD_SIZE - 1; j++)
        {
            int posX = j * tile_w;
            int posY = (BOARD_SIZE - 1 - i) * tile_h;
            DrawRectangleLines(posX, posY, tile_w, tile_h, WHITE);

            switch (game.at(i, j))
            {
            case ConnectFour::Piece::PLAYER1:
                DrawCircle(posX + tile_w / 2, posY + tile_h / 2, std::min(tile_w, tile_h) / 3, RED);
                break;
            case ConnectFour::Piece::PLAYER2:
                DrawCircle(posX + tile_w / 2, posY + tile_h / 2, std::min(tile_w, tile_h) / 3, BLUE);
                break;
            }
        }
    }
}

void drawLog()
{
    const float fontSize = 30.0f;
    auto textWidth = MeasureText(G_log.c_str(), fontSize);
    DrawText(G_log.c_str(), (GetScreenWidth() / 2) - textWidth / 2, 15, fontSize, WHITE);
}

int main()
{
    ConnectFour game;

    int screen_w = 800;
    int screen_h = 800;

    InitWindow(screen_w, screen_h, "Minimax Connect4");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(30);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);
            if (!game.isGameOver())
            {
                handleMouseInput(game);
                handleAI(game);
                handleGameOver(game);
            }

            handleKeyboardInput(game);
            render(game);
            drawLog();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
