#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include "raylib.h"
using namespace std;

// Convert card number to image filename
string getImageName(int card_num){
    string suits[]={"spades","clubs","hearts","diamonds"};
    
    int suit=(card_num-1)/13;
    int value=(card_num-1)%13+1;
    
    return to_string(value)+"_of_"+suits[suit]+".png";
}

int main(){
    srand(time(0));
    
    // --- Your original logic unchanged ---
    int array[52];
    for(int i=0;i<52;i++) array[i]=i+1;
    for(int i=51;i>=0;i--){
        int j=rand()%(i+1);
        swap(array[i],array[j]);
    }

    int p1[13],p2[13],p3[13],p4[13];
    int round=1;
    for(int i=0;i<52;i++){
        int k=i%4;
        switch(k){
            case 0: p1[round-1]=array[i]; break;
            case 1: p2[round-1]=array[i]; break;
            case 2: p3[round-1]=array[i]; break;
            default: p4[round-1]=array[i]; round++; break;
        }
    }
    // --- End of your original logic ---

    // Raylib window
    InitWindow(1200, 800, "Call Break");
    SetTargetFPS(60);

    // Load card images for all 4 players
    Texture2D cards_p1[13], cards_p2[13], cards_p3[13], cards_p4[13];
    for(int i=0;i<13;i++){
        string path1 = "cards/" + getImageName(p1[i]);
        string path2 = "cards/" + getImageName(p2[i]);
        string path3 = "cards/" + getImageName(p3[i]);
        string path4 = "cards/" + getImageName(p4[i]);
        
        cards_p1[i] = LoadTexture(path1.c_str());
        cards_p2[i] = LoadTexture(path2.c_str());
        cards_p3[i] = LoadTexture(path3.c_str());
        cards_p4[i] = LoadTexture(path4.c_str());
    }

    int cardW=70, cardH=100;  // card display size

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(DARKGREEN);

        // Player 1 - Bottom
        DrawText("Player 1", 10, 680, 20, WHITE);
        for(int i=0;i<13;i++){
            DrawTextureEx(cards_p1[i], {(float)(10+i*88), 570}, 0, (float)cardW/cards_p1[i].width, WHITE);
        }

        // Player 2 - Top
        DrawText("Player 2", 10, 10, 20, WHITE);
        for(int i=0;i<13;i++){
            DrawTextureEx(cards_p2[i], {(float)(10+i*88), 30}, 0, (float)cardW/cards_p2[i].width, WHITE);
        }

        // Player 3 - Left (rotated)
        DrawText("Player 3", 10, 350, 20, WHITE);
        for(int i=0;i<13;i++){
            DrawTextureEx(cards_p3[i], {(float)10, (float)(150+i*40)}, 0, (float)cardW/cards_p3[i].width, WHITE);
        }

        // Player 4 - Right
        DrawText("Player 4", 1050, 350, 20, WHITE);
        for(int i=0;i<13;i++){
            DrawTextureEx(cards_p4[i], {(float)1100, (float)(150+i*40)}, 0, (float)cardW/cards_p4[i].width, WHITE);
        }

        EndDrawing();
    }

    // Unload textures
    for(int i=0;i<13;i++){
        UnloadTexture(cards_p1[i]);
        UnloadTexture(cards_p2[i]);
        UnloadTexture(cards_p3[i]);
        UnloadTexture(cards_p4[i]);
    }

    CloseWindow();
    return 0;
}