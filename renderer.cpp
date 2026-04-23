#include "renderer.h"
#include "entities.h"

using namespace std;

Renderer::Renderer(int w, int h) : sw(w), sh(h) {
    menuBg = LoadTexture("background.jpg");
    menuBg2 = LoadTexture("background2.jpg");
    SetTextureWrap(menuBg2, TEXTURE_WRAP_CLAMP);
}


// ───────────────── MENU ─────────────────
void Renderer::drawMenu(Rectangle startBtn, Rectangle quitBtn) {

    
    
    DrawTexturePro(
        menuBg,
        { 0, 0, (float)menuBg.width, (float)menuBg.height },
        { 0, 0, 1200, 800 },
        { 0, 0 },
        0.0f,
        WHITE
    );

      // unload immediately after drawing

    DrawText("CALL BREAK", sw/2 - 120, 100, 40, GOLD);

 DrawRectangleRec(startBtn, { 0, 0, 0, 180 });           // black 70% opacity
DrawRectangleLinesEx(startBtn, 2, WHITE);                // white border
DrawText("START", startBtn.x + 60, startBtn.y + 18, 20, WHITE);

// Quit button
DrawRectangleRec(quitBtn, { 0, 0, 0, 180 });
DrawRectangleLinesEx(quitBtn, 2, WHITE);
DrawText("QUIT", quitBtn.x + 65, quitBtn.y + 18, 20, WHITE);
}
Renderer::~Renderer() {
    UnloadTexture(menuBg);
    UnloadTexture(menuBg2);
}


// ───────────────── PLAYER HAND ─────────────────
void Renderer::drawPlayerHand(Player& p, int x, int y, int spacing, bool interactive) {
      DrawTexturePro(
        menuBg2,
        { 0, 0, (float)menuBg2.width, (float)menuBg2.height },
        { 0, 0, 1200, 800 },
        { 0, 0 },
        0.0f,
        WHITE
    );
    for (int i = 0; i < p.handSize; i++) {
       

        Vector2 pos = {(float)(x + i * spacing), (float)y};

        float scale = (float)cardW / p.hand[i].texture.width;

        bool hover = false;

        if (interactive) {
            Rectangle r = {
                pos.x,
                pos.y,
                (float)cardW,
                100.0f * scale
            };

            hover = CheckCollisionPointRec(GetMousePosition(), r);
        }

        DrawTextureEx(
            p.hand[i].texture,
            {pos.x, pos.y - (hover ? 15 : 0)},
            0,
            scale,
            WHITE
        );
    }
}

// ───────────────── PAUSE ─────────────────
void Renderer::drawPause() {
    DrawText("PAUSED", sw/2 - 60, sh/2 - 40, 30, RED);
    DrawText("R - Resume", sw/2 - 70, sh/2 + 10, 20, GRAY);
    DrawText("M - Menu", sw/2 - 60, sh/2 + 40, 20, GRAY);
}

// ───────────────── GAME OVER ─────────────────
void Renderer::drawGameOver(Player players[4]) {
    DrawText("GAME OVER", sw/2 - 80, sh/2 - 60, 30, MAROON);

    for (int i = 0; i < 4; i++) {
        string info = players[i].name + ": " + to_string(players[i].score);
        DrawText(info.c_str(), sw/2 - 80, sh/2 + i * 30, 20, BLACK);
    }

    DrawText("ENTER - Menu", sw/2 - 80, sh/2 + 140, 18, GRAY);
}