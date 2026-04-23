#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include "raylib.h"
using namespace std;

enum GameState {
    LOGO = 0,
    MENU,
    PLAYING,
    PAUSE,
    GAME_OVER
};

int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Call Break");
    SetTargetFPS(60);

    GameState currentState = MENU;

    Rectangle startBtn = {350, 250, 200, 60};
    Rectangle quitBtn  = {350, 330, 200, 60};

    while (!WindowShouldClose()) {

        Vector2 mouse = GetMousePosition();

        // -------- UPDATE --------
        switch (currentState) {

            case MENU:
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mouse, startBtn)) {
                        currentState = PLAYING;
                    }
                    if (CheckCollisionPointRec(mouse, quitBtn)) {
                        CloseWindow();
                    }
                }
                break;

            case PLAYING:
                if (IsKeyPressed(KEY_P)) {
                    currentState = PAUSE;
                }
                if (IsKeyPressed(KEY_E)) {
                    currentState = GAME_OVER;
                }
                break;

            case PAUSE:
                if (IsKeyPressed(KEY_R)) {
                    currentState = PLAYING;
                }
                if (IsKeyPressed(KEY_M)) {
                    currentState = MENU;
                }
                break;

            case GAME_OVER:
                if (IsKeyPressed(KEY_ENTER)) {
                    currentState = MENU;
                }
                break;

            default: break;
        }

        // -------- DRAW --------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentState) {

            case MENU:
                DrawText("CALL BREAK", 300, 120, 40, BLACK);

                DrawRectangleRec(startBtn, LIGHTGRAY);
                DrawText("START", 400, 265, 20, BLACK);

                DrawRectangleRec(quitBtn, LIGHTGRAY);
                DrawText("QUIT", 410, 345, 20, BLACK);
                break;

            case PLAYING:
                DrawText("Game Running...", 320, 250, 30, DARKGREEN);
                DrawText("Press P to Pause", 320, 300, 20, GRAY);
                break;

            case PAUSE:
                DrawText("PAUSED", 370, 250, 30, RED);
                DrawText("Press R to Resume", 320, 300, 20, GRAY);
                DrawText("Press M for Menu", 320, 330, 20, GRAY);
                break;

            case GAME_OVER:
                DrawText("GAME OVER", 340, 250, 30, MAROON);
                DrawText("Press ENTER for Menu", 290, 300, 20, GRAY);
                break;

            default: break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}