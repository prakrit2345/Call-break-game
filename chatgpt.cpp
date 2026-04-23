#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "raylib.h"

using namespace std;

// ---------------- CARD NAME ----------------
string getImageName(int card_num){
    string suits[] = {"spades", "clubs", "hearts", "diamonds"};

    int suit = (card_num - 1) / 13;
    int value = (card_num - 1) % 13 + 1;

    return to_string(value) + "_of_" + suits[suit] + ".png";
}

// ---------------- GLOBAL CLICK STATE ----------------
int selectedCard = -1;
int selectedPlayer = -1;

// ---------------- HELPERS ----------------
bool IsMouseOver(Rectangle rect){
    Vector2 mouse = GetMousePosition();
    return CheckCollisionPointRec(mouse, rect);
}

// ---------------- MAIN ----------------
int main(){

    srand(time(0));

    // ---------------- SHUFFLE ----------------
    int deck[52];
    for(int i = 0; i < 52; i++) deck[i] = i + 1;

    for(int i = 51; i > 0; i--){
        int j = rand() % (i + 1);
        swap(deck[i], deck[j]);
    }

    // ---------------- DEAL ----------------
    int p1[13], p2[13], p3[13], p4[13];
    int r = 0;

    for(int i = 0; i < 52; i++){
        int k = i % 4;

        if(k == 0) p1[r] = deck[i];
        else if(k == 1) p2[r] = deck[i];
        else if(k == 2) p3[r] = deck[i];
        else {
            p4[r] = deck[i];
            r++;
        }
    }

    // ---------------- WINDOW ----------------
    InitWindow(1200, 800, "Call Break - Clickable Cards");
    SetTargetFPS(60);

    // ---------------- LOAD TEXTURES ----------------
    Texture2D cards_p1[13], cards_p2[13], cards_p3[13], cards_p4[13];

    for(int i = 0; i < 13; i++){
        cards_p1[i] = LoadTexture(("cards/" + getImageName(p1[i])).c_str());
        cards_p2[i] = LoadTexture(("cards/" + getImageName(p2[i])).c_str());
        cards_p3[i] = LoadTexture(("cards/" + getImageName(p3[i])).c_str());
        cards_p4[i] = LoadTexture(("cards/" + getImageName(p4[i])).c_str());
    }

    int cardW = 70;
    float scale = (float)cardW / cards_p1[0].width;
    int spacing = 45;

    // ---------------- GAME LOOP ----------------
    while(!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(DARKGREEN);

        DrawText("CALL BREAK - CLICK CARDS", 450, 10, 20, WHITE);

        Vector2 mouse = GetMousePosition();

        selectedCard = -1;
        selectedPlayer = -1;

        // ================= PLAYER 1 (BOTTOM) =================
        DrawText("Player 1", 20, 700, 20, WHITE);

        int startX1 = 250;
        int y1 = 620;

        for(int i = 0; i < 13; i++){

            float x = startX1 + i * spacing;

            Rectangle rect = {
                (float)x,
                (float)y1,
                (float)(cardW * scale),
                (float)(100 * scale)
            };

            bool hover = IsMouseOver(rect);

            if(hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                selectedCard = i;
                selectedPlayer = 1;
            }

            float offsetY = hover ? -10 : 0;

            DrawTextureEx(
                cards_p1[i],
                {(float)x, (float)y1 + offsetY},
                0,
                scale,
                WHITE
            );
        }

        // ================= PLAYER 2 (TOP) =================
        DrawText("Player 2", 20, 10, 20, WHITE);

        int startX2 = 250;
        int y2 = 30;

        for(int i = 0; i < 13; i++){
            float x = startX2 + i * spacing;

            DrawTextureEx(
                cards_p2[i],
                {(float)x, (float)y2},
                0,
                scale,
                WHITE
            );
        }

        // ================= PLAYER 3 (LEFT - FIXED ALIGNMENT) =================
        DrawText("Player 3", 20, 350, 20, WHITE);

        int x3 = 80;
        int startY3 = 180;   // FIXED (was misaligned)

        for(int i = 0; i < 13; i++){
            float y = startY3 + i * spacing;

            DrawTextureEx(
                cards_p3[i],
                {(float)x3, (float)y},
                90,
                scale,
                WHITE
            );
        }

        // ================= PLAYER 4 (RIGHT - FIXED ALIGNMENT) =================
        DrawText("Player 4", 1050, 350, 20, WHITE);

        int x4 = 1100;
        int startY4 = 180;   // FIXED alignment

        for(int i = 0; i < 13; i++){
            float y = startY4 + i * spacing;

            DrawTextureEx(
                cards_p4[i],
                {(float)x4, (float)y},
                90,
                scale,
                WHITE
            );
        }

        // ================= POPUP WINDOW =================
        if(selectedCard != -1){
            DrawRectangle(400, 250, 400, 200, Fade(BLACK, 0.8f));
            DrawRectangleLines(400, 250, 400, 200, WHITE);

            DrawText("CARD CLICKED!", 520, 270, 20, YELLOW);

            string msg = "Player " + to_string(selectedPlayer) +
                         " Card Index: " + to_string(selectedCard);

            DrawText(msg.c_str(), 450, 320, 18, WHITE);

            DrawText("Click anywhere to continue", 450, 370, 15, GRAY);
        }

        EndDrawing();
    }

    // ---------------- CLEANUP ----------------
    for(int i = 0; i < 13; i++){
        UnloadTexture(cards_p1[i]);
        UnloadTexture(cards_p2[i]);
        UnloadTexture(cards_p3[i]);
        UnloadTexture(cards_p4[i]);
    }

    CloseWindow();
    return 0;
}