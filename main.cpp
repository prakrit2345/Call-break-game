#include "raylib.h"
#include <iostream>
#include "entities.h"
#include "renderer.h"
#include "gamemanager.h"

using namespace std;

enum GameState { MENU, SHUFFLING, USER_INPUT, PLAYING, PAUSE, GAME_OVER };

int main() {
    InitWindow(1200, 800, "Call Break");
    InitAudioDevice();
    SetTargetFPS(60);
    Music menuMusic = LoadMusicStream("game.wav");
    Music shuffleMusic = LoadMusicStream("card_deal.mp3");

    int sw = 1200;
    int sh = 800;
    static bool isActive = false;
    static string text = "";
    static bool submitted = false;

    Renderer renderer(sw, sh);
    GameManager gameManager;
    GameState state = MENU;

    Player human("You", 1, true);
    AIPlayer ai1("Bot 1", 2), ai2("Bot 2", 3), ai3("Bot 3", 4);

    Player* players[4];
    players[0] = &human;
    players[1] = &ai1;
    players[2] = &ai2;
    players[3] = &ai3;

    bool gameStarted = false;

    Rectangle startBtn = {(float)sw/2 - 100, (float)sh/2 - 50, 200, 60};
    Rectangle quitBtn  = {(float)sw/2 - 100, (float)sh/2 + 30, 200, 60};
    Rectangle inputBox = {
        sw / 2.0f - 300 / 2.0f,
        sh / 2.0f - 40 / 2.0f,
        300,
        40
    };

    bool dealingPhase = true;
    float gameTimer = 0.0f;
    bool menumusic = false;
    bool shufflemusic = false;

    while (!WindowShouldClose()) {

        Vector2 mouse = GetMousePosition();

        // ── UPDATE ─────────────────────────
        switch (state) {
            case MENU:
                if (!menumusic) {
                    PlayMusicStream(menuMusic);
                    menumusic = true;
                } else {
                    UpdateMusicStream(menuMusic);
                }
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mouse, startBtn)) {
                        state = SHUFFLING;
                        StopMusicStream(menuMusic);
                        gameStarted = false;
                        gameTimer = 0.0f;
                        dealingPhase = true;
                        for (int i = 0; i < 4; i++) {
                            players[i]->tricksWon = 0;
                            players[i]->score = 0;
                        }
                    }
                    if (CheckCollisionPointRec(mouse, quitBtn)) {
                        CloseWindow();
                        return 0;
                    }
                }
                break;

            case SHUFFLING:
                if (!shufflemusic) {
                    PlayMusicStream(shuffleMusic);
                    shufflemusic = true;
                } else {
                    if (dealingPhase) {
                        UpdateMusicStream(shuffleMusic);
                    } else {
                        StopMusicStream(shuffleMusic);
                    }
                }
                if (!gameStarted) {
                    Deck deck;
                    deck.shuffle();
                    for (int i = 0; i < 52; i++) {
                        players[i % 4]->receiveCard(deck.cardAt(i));
                    }
                    gameStarted = true;
                }
                if (dealingPhase) {
                    gameTimer += GetFrameTime();
                    if (gameTimer > 10.0f) {
                        dealingPhase = false;
                        players[0]->isRevealed = true;
                    }
                }
                if (IsKeyPressed(KEY_P)) state = PAUSE;
                if (IsKeyPressed(KEY_Z)) state = USER_INPUT;
                if (IsKeyPressed(KEY_E)) state = GAME_OVER;
                break;

            case USER_INPUT:
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(GetMousePosition(), inputBox)) {
                        isActive = true;
                    } else {
                        isActive = false;
                    }
                }
                if (isActive) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if (key >= '0' && key <= '9' && text.length() < 1) {
                            text = (char)key;
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE)) {
                        text = "";
                    }
                    if (IsKeyPressed(KEY_ENTER) && !text.empty()) {
                        players[0]->bid = text[0] - '0';
                        submitted = true;
                    }
                }
                players[1]->bid = ai1.chooseBid();
                players[2]->bid = ai2.chooseBid();
                players[3]->bid = ai3.chooseBid();

                if (submitted) {
                    state = PLAYING;
                }
                break;

            case PLAYING:
                gameManager.updateGame(players);
                if (gameManager.gameOver) state = GAME_OVER;
                if (IsKeyPressed(KEY_P)) state = PAUSE;
                if (IsKeyPressed(KEY_E)) state = GAME_OVER;
                break;

            case PAUSE:
                if (IsKeyPressed(KEY_R)) state = PLAYING;
                if (IsKeyPressed(KEY_M)) state = MENU;
                break;

            case GAME_OVER:
                if (IsKeyPressed(KEY_ENTER)) {
                    state = MENU;
                    menumusic = false;
                    shufflemusic = false;
                    submitted = false;
                    text = "";
                    isActive = false;
                    gameStarted = false;
                    for (int i = 0; i < 4; i++) {
                        players[i]->unloadCards();
                    }
                }
                break;
        }

        // ── DRAW ─────────────────────────
        BeginDrawing();
        ClearBackground(DARKGREEN);

        switch (state) {
            case MENU:
                renderer.drawMenu(startBtn, quitBtn);
                break;

            case SHUFFLING:
                renderer.drawPlayerHand(players[0], 150, sh - 150, 70, false);
                if (dealingPhase) {
                    renderer.shuffleEffect();
                }
                break;

            case USER_INPUT:
                renderer.drawUserInput(inputBox, text, isActive);
                break;

            case PLAYING:
                renderer.drawPlayerHand(players[0], 150, sh - 150, 70, true);
                renderer.drawhiddenhand();
                renderer.drawTable(gameManager.roundManager.moves); 
                if (submitted) {
                    renderer.drawCall(players);
                }
                renderer.drawClock(
                    gameManager.timeManager.currentTime,
                    gameManager.timeManager.totalTime,
                    600, 700, 40
                );
                break;

            case PAUSE:
                renderer.drawPlayerHand(players[0], 300, sh - 150, 80, false);
                renderer.drawPause();
                break;

            case GAME_OVER:
                renderer.drawGameOver(players);
                break;
        }

        EndDrawing();
    }

    for (int i = 0; i < 4; i++) {
        players[i]->unloadCards();
    }
    UnloadMusicStream(menuMusic);
    UnloadMusicStream(shuffleMusic);
    CloseWindow();
    return 0;
}