#pragma once

#include "raylib.h"
#include <string>

class Player; // forward declaration (IMPORTANT)

class Renderer {
public:
    ~Renderer();
    int sw, sh;
    int cardW = 80;

    Renderer(int w, int h);

    void drawMenu(Rectangle startBtn, Rectangle quitBtn);

    void drawPlayerHand(Player& p, int x, int y, int spacing, bool interactive);

    void drawPause();

    void drawGameOver(Player players[4]);
  
private:
    Texture2D menuBg;
    Texture2D menuBg2;
    
};