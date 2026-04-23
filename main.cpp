#include "raylib.h"
#include "entities.h"
#include "renderer.h"

enum GameState { MENU, PLAYING, PAUSE, GAME_OVER };

int main() {
    InitWindow(1200, 800, "Call Break");
    InitAudioDevice();
    SetTargetFPS(60);
    Music menuMusic = LoadMusicStream("game.wav");
    
    int sw = 1200;
    int sh = 800;

    Renderer renderer(sw, sh);
    GameState state = MENU;

    // ── Players (NO POINTERS) ─────────────────────────
    Player human("You", true);
    AIPlayer ai1("Bot 1"), ai2("Bot 2"), ai3("Bot 3");

    Player players[4];

    players[0] = human;
    players[1] = ai1;
    players[2] = ai2;
    players[3] = ai3;

    bool gameStarted = false;

    Rectangle startBtn = {(float)sw/2 - 100, (float)sh/2 - 50, 200, 60};
    Rectangle quitBtn  = {(float)sw/2 - 100, (float)sh/2 + 30, 200, 60};
    bool menumusic=false;

    while (!WindowShouldClose()) {
        
        Vector2 mouse = GetMousePosition();

        // ── UPDATE ─────────────────────────
        switch (state) {
            case MENU:
            if(!menumusic){
                PlayMusicStream(menuMusic);
                menumusic=true;
            }
            else{
                UpdateMusicStream(menuMusic);
            }
            
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mouse, startBtn)) {
                        state = PLAYING;
                        gameStarted = false;
                    }
                    if (CheckCollisionPointRec(mouse, quitBtn)) {
                        CloseWindow();
                        return 0;
                    }
                }
                break;

            case PLAYING:
                if (!gameStarted) {
                    Deck deck;
                    deck.shuffle();

                    for (int i = 0; i < 52; i++) {
                        players[i % 4].receiveCard(deck.cardAt(i));
                    }

                    gameStarted = true;
                }

                if (IsKeyPressed(KEY_P)) state = PAUSE;
                if (IsKeyPressed(KEY_E)) state = GAME_OVER;
                break;

            case PAUSE:
                if (IsKeyPressed(KEY_R)) state = PLAYING;
                if (IsKeyPressed(KEY_M)) state = MENU;
                break;

            case GAME_OVER:
                if (IsKeyPressed(KEY_ENTER)) state = MENU;
                break;
        }

        // ── DRAW ─────────────────────────
        BeginDrawing();
        ClearBackground(DARKGREEN);

        switch (state) {
            case MENU:
                renderer.drawMenu(startBtn, quitBtn);

                break;

            case PLAYING:
                renderer.drawPlayerHand(players[0], 300, sh - 150, 45, true);
                break;

            case PAUSE:
                renderer.drawPlayerHand(players[0], 300, sh - 150, 45, false);
                renderer.drawPause();
                break;

            case GAME_OVER:
                renderer.drawGameOver(players);
                break;
        }

        EndDrawing();
    }

    for (int i = 0; i < 4; i++) {
        players[i].unloadCards();
    }
   UnloadMusicStream(menuMusic);
    CloseWindow();
    return 0;
}