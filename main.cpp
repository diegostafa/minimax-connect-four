#include <string>

#include <raylib.h>

#include "minimax.cpp"
#include "connect_four.cpp"

#define SCREENPADDING 100

std::vector<std::pair<int, int>> G_winConnection;
std::string G_log = "";

void drawLog()
{
    const float fontSize = 30.0f;
    const int textWidth = MeasureText(G_log.c_str(), fontSize);
    DrawText(G_log.c_str(), (GetScreenWidth() / 2) - textWidth / 2, 15, fontSize, WHITE);
}

int getFocusedColumn(const ConnectFour &game)
{
    const int colW = (GetScreenWidth() - SCREENPADDING * 2) / game.COLS;
    const int colFocus = ((int)GetMousePosition().x - SCREENPADDING) / colW;
    if (colFocus >= 0 && colFocus < game.COLS)
        return colFocus;

    return -1;
}

void handleUserTurn(ConnectFour &game)
{
    if (!game.isMaxTurn())
        return;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        game.makeMove(getFocusedColumn(game));
}

void handleOpponentTurn(ConnectFour &game)
{
    if (game.isMaxTurn())
        return;

    const auto bestMove = minimax(game, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    if (bestMove.second == -1)
    {
        auto possibleMoves = game.getPossibleMoves();
        game.makeMove(possibleMoves[rand() % possibleMoves.size() - 1]);
    }
    else
        game.makeMove(bestMove.second);

    if (bestMove.first == std::numeric_limits<int>::max())
        G_log = "YOU HAVE A FORCED WIN";
    else if (bestMove.first == std::numeric_limits<int>::min())
        G_log = "I HAVE A FORCED WIN";
    else
        G_log = "";
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
        G_winConnection = game.getWinConnection();
    }
}

void handleKeyboardInput(ConnectFour &game)
{
    if (IsKeyPressed(KEY_R))
    {
        game.reset();
        G_log = "";
        G_winConnection.clear();
    }
}

void render(const ConnectFour &game)
{
    const auto screenW = GetScreenWidth();
    const auto screenH = GetScreenHeight();
    const auto tileW = (screenW - SCREENPADDING * 2) / game.COLS;
    const auto tileH = (screenH - SCREENPADDING * 2) / game.ROWS;
    const auto focusedColumn = getFocusedColumn(game);

    if (focusedColumn != -1)
        DrawRectangle(SCREENPADDING + tileW * focusedColumn, SCREENPADDING, tileW, screenH - SCREENPADDING * 2, {255, 0, 0, 50});

    for (int i = 0; i <= game.ROWS; i++)
        for (int j = 0; j <= game.COLS; j++)
        {
            const int leftX = SCREENPADDING + tileW * j;
            const int leftY = SCREENPADDING + tileH * i;

            DrawLine(SCREENPADDING, leftY, screenW - SCREENPADDING, leftY, WHITE);
            DrawLine(leftX, SCREENPADDING, leftX, screenH - SCREENPADDING, WHITE);

            if (i < game.ROWS && j < game.COLS)
            {
                const int centerX = leftX + tileW / 2;
                const int centerY = leftY + tileH / 2;

                switch (game.pieceAt(i, j))
                {
                case ConnectFour::Piece::PLAYER1:
                    DrawCircle(centerX, centerY, (float)std::min(tileW, tileH) / 3, RED);
                    break;
                case ConnectFour::Piece::PLAYER2:
                    DrawCircle(centerX, centerY, (float)std::min(tileW, tileH) / 3, BLUE);
                    break;
                default:
                    break;
                }
            }
        }

    for (auto &&w : G_winConnection)
    {
        const int leftX = SCREENPADDING + tileW * w.second;
        const int leftY = SCREENPADDING + tileH * w.first;
        const int centerX = leftX + tileW / 2;
        const int centerY = leftY + tileH / 2;
        DrawRectangle(leftX, leftY, tileW, tileH, {255, 203, 0, 100});
        DrawCircle(centerX, centerY, (float)std::min(tileW, tileH) / 3, GOLD);
    }
}

int main()
{
    ConnectFour game;

    InitWindow(800, 800, "Minimax Connect4");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(120);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        handleKeyboardInput(game);

        if (!game.isGameOver())
        {
            handleUserTurn(game);
            handleOpponentTurn(game);
            handleGameOver(game);
        }

        render(game);

        drawLog();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
