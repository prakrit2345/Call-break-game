#pragma once

#include "raylib.h"
#include "entities.h"
#include <vector>
#include <string>

using namespace std;

class Player; // forward declaration (IMPORTANT)

class Renderer {
public:
    ~Renderer();
    int sw, sh;
    int cardW = 70;

    Renderer(int w, int h);

    void drawMenu(Rectangle startBtn, Rectangle quitBtn);
    void drawUserInput(Rectangle inputBox, const std::string& text, bool isActive);

    void drawPlayerHand(Player* p, int x, int y, int spacing, bool interactive);
    void drawClock(Vector2 center, float currentTime, float totalTime);
    void shuffleEffect();

    void drawPause();
    void drawhiddenhand();
    void drawCall(Player* p[4]);

    void drawGameOver(Player* players[4]);
    void drawClock(float currentTime, float totalTime, int x, int y, int radius);
    void drawTable(vector<Move>& moves); 
  
private:
    Texture2D menuBg;
    Texture2D menuBg2;
    Texture2D cardback;
    
};