#include "renderer.h"
#include "entities.h"
#include "gamemanager.h"

using namespace std;

Renderer::Renderer(int w, int h) : sw(w), sh(h) {
    menuBg = LoadTexture("background.jpg");
    menuBg2 = LoadTexture("backgroundmenubg.jpg");
    cardback=LoadTexture("backhand.jpeg");

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
    UnloadTexture(cardback);
}
void Renderer::drawUserInput(Rectangle inputBox, const std::string& text, bool isActive) {
       DrawTexturePro(
        menuBg2,
        { 0, 0, (float)menuBg2.width, (float)menuBg2.height },
        { 0, 0, 1200, 800 },
        { 0, 0 },
        0.0f,
        WHITE
    );
  

    DrawText("Enter your bid (1-7):", sw/2 - 100, sh/2 - 80, 20, WHITE);

    // Input box
     DrawRectangleRec(inputBox, LIGHTGRAY);
    DrawRectangleLinesEx(inputBox, 2, isActive ? BLUE : DARKGRAY);

    // Text or placeholder
    if (text.empty()) {
        DrawText("Enter bid", inputBox.x + 10, inputBox.y + 20, 20, GRAY);
    } else {
        DrawText(text.c_str(), inputBox.x + 10, inputBox.y + 20, 20, BLACK);
    }
}


// ───────────────── PLAYER HAND ─────────────────
void Renderer::drawPlayerHand(Player* p, int x, int y, int spacing, bool interactive){
      DrawTexturePro(
        menuBg2,
        { 0, 0, (float)menuBg2.width, (float)menuBg2.height },
        { 0, 0, 1200, 800 },
        { 0, 0 },
        0.0f,
        WHITE
    );
    if(p->isRevealed){
    for (int i = 0; i < p->handSize; i++) {
       

        Vector2 pos = {(float)(x + i * spacing), (float)y};

        float scale = (float)cardW / p->hand[i].texture.width;

        bool hover = false;
        p->rects[i]={(float)(x + i * spacing), (float)y,80,p->hand[i].texture.height * scale};    
        if (interactive) {
            Rectangle r =p->rects[i];

            hover = CheckCollisionPointRec(GetMousePosition(), r);
        }
        

        DrawTextureEx(
            p->hand[i].texture,
            {pos.x, pos.y - (hover ? 20 : 0)},
            0,
            scale,
            WHITE
        );
       DrawText("You", x+400, y - 50, 20, WHITE);
    }

    }
}

// ───────────────── PAUSE ─────────────────
void Renderer::drawPause() {
    DrawText("PAUSED", sw/2 - 60, sh/2 - 40, 30, RED);
    DrawText("R - Resume", sw/2 - 70, sh/2 + 10, 20, GRAY);
    DrawText("M - Menu", sw/2 - 60, sh/2 + 40, 20, GRAY);
}

// ───────────────── GAME OVER ─────────────────
void Renderer::drawGameOver(Player* players[4]) {
    DrawText("GAME OVER", sw/2 - 80, sh/2 - 60, 30, MAROON);

    for (int i = 0; i < 4; i++) {
        string info = players[i]->name + ": " + to_string(players[i]->score);
        DrawText(info.c_str(), sw/2 - 80, sh/2 + i * 30, 20, BLACK);
    }

    DrawText("ENTER - Menu", sw/2 - 80, sh/2 + 140, 18, GRAY);
}
void Renderer::drawhiddenhand(){
    float centerX = 600.0f;   // screen center (adjust if needed)
    float topY = 35.0f;
          // vertical spacing
        // horizontal shift

    for (int i = 0; i < 3; i++) {
        float x, y;

        if (i == 0) {
            x = centerX-35;
            y = topY;
        } else if (i == 1) {
            x = centerX - 450.0f;
            y = topY + 300;
        } else { // i == 2
            x = centerX + 450.0f;
            y = topY + 300;
        }

        Vector2 pos = { x, y };

        DrawTextureEx(cardback, pos, 0.0f, 70.0f / cardback.width, WHITE);
        string text="Bot"+to_string(i+1);
        DrawText(text.c_str(), x, y+120, 20, WHITE);
        
    }
  
      
}
void Renderer::drawCall(Player* p[4]){
 
  for(int i=0;i<4;i++){
    int x,y;
    if (i==0){
       x=530;
       y=630;
    }
    else if(i==1){
       x=556;
       y=85+90;
    }
    else if(i==2){
         x=140;
         y=335+50+90;
     }
     else{
         x=1040;
         y=335+50+90;
    }
    string text=" Call: "+to_string(p[i]->bid);
    DrawText(text.c_str(), x, y, 20, WHITE);
  }

}
void Renderer::shuffleEffect(){
    Vector2 deckPos = { 400, 200 };

    float scale = 400.0f /cardback.height;

    for (int i = 0; i < 6; i++) {

        // deeper cards go slightly behind
        Vector2 offset = {
            i * 2.0f,
            i * 2.5f
        };

        // subtle depth shading (fake 3D feel)
        Color tint = WHITE;
        tint.a = 255 - i * 18;   // darker as it goes down

        DrawTextureEx(
            cardback,
            {
                deckPos.x + offset.x,
                deckPos.y + offset.y
            },
            0.0f,   // NO rotation (important for realism)
            scale,
            tint
        );
        DrawText("Dealing Cards...", deckPos.x-25, deckPos.y + 450, 50, WHITE);
       
    }
    
}
void Renderer::drawClock(float currentTime, float totalTime, int x, int y, int radius) {
    float fraction = currentTime / totalTime;
    float startAngle = -90.0f;
    float endAngle = -90.0f + (360.0f * fraction);

    // background circle
    DrawCircle(x, y, radius, DARKGRAY);

    // pie slice
    DrawCircleSector(
        {(float)x, (float)y},
        radius,
        startAngle,
        endAngle,
        36,
        fraction > 0.3f ? GREEN : RED
    );

    // border
    DrawCircleLines(x, y, radius, WHITE);

    // countdown text in center
    string text = to_string((int)currentTime);
    DrawText(text.c_str(), x - 10, y - 10, 20, WHITE);
}
void Renderer::drawTable(vector<Move>& moves) {
    // Center positions for each player's played card
    // Player 0 = bottom, 1 = top, 2 = left, 3 = right
    float cx = 600.0f, cy = 380.0f;
    float offsets[4][2] = {
        { cx - 35,  cy + 80  },   // player 0 (You) - bottom center
        { cx - 35,  cy - 130 },   // player 1 (Bot1) - top center
        { cx - 150, cy - 25  },   // player 2 (Bot2) - left center
        { cx + 80,  cy - 25  }    // player 3 (Bot3) - right center
    };

    for (int i = 0; i < (int)moves.size(); i++) {
        int pid = moves[i].player_id;
        int slot = pid - 1;  // player_id is 1-based
        Card& c = moves[i].cardPlayed;

        float scale = 70.0f / c.texture.width;
        Vector2 pos = { offsets[slot][0], offsets[slot][1] };

        DrawTextureEx(c.texture, pos, 0.0f, scale, WHITE);
    }
}