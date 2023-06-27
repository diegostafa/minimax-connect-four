#include <raylib.h>

#include "connect_four.cpp"
#include "ai_algorithms.cpp"

std::string G_log = "";
int G_focusedColumn = -1;

void drawLog()
{
    const float fontSize = 30.0f;
    auto textWidth = MeasureText(G_log.c_str(), fontSize);
    DrawText(G_log.c_str(), (GetScreenWidth() / 2) - textWidth / 2, 15, fontSize, WHITE);
}

void handleColumnFocus(const ConnectFour &game)
{
    G_focusedColumn = GetMousePosition().x / (GetScreenWidth() / game.boardSize());
}

void handleUserTurn(ConnectFour &game)
{
    if (!game.isYourTurn())
        return;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        game.makeMove(G_focusedColumn);
}

void handleOpponentTurn(ConnectFour &game)
{
    if (game.isYourTurn())
        return;

    auto bestMove = minimax(game, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    if (bestMove.second == -1)
        game.makeMove(game.getPossibleMoves()[0]);
    else
        game.makeMove(bestMove.second);

    if (bestMove.first == 0)
        G_log = "I CAN DRAW";
    else if (bestMove.first == std::numeric_limits<int>::max())
        G_log = "YOU HAVE A FORCED WIN";
    else if (bestMove.first == std::numeric_limits<int>::min())
        G_log = "I HAVE A FORCED WIN";
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
    float tile_w = GetScreenWidth() / game.boardSize();
    float tile_h = GetScreenHeight() / game.boardSize();

    for (int i = 1; i < game.boardSize() - 1; i++)
    {
        for (int j = 1; j < game.boardSize() - 1; j++)
        {
            int posX = j * tile_w;
            int posY = (game.boardSize() - 1 - i) * tile_h;

            if (j == G_focusedColumn)
                DrawRectangle(
                    posX, posY,
                    tile_w, tile_h,
                    {255, 0, 0, 50});

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

int main()
{
    ConnectFour game;

    InitWindow(800, 800, "Minimax Connect4");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(30);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        handleColumnFocus(game);
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
