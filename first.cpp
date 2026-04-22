#include "raylib.h"
#include <string>

int main(void)
{
    InitWindow(1200, 700, "Spades");
    SetTargetFPS(60);

    // Load 2 to 10 of spades (9 cards)
    Texture2D cards[9];
    for (int i = 0; i < 9; i++)
    {
        std::string path = "/Users/prakrit/Desktop/c++/cards/" + std::to_string(i + 2) + "_of_spades.png";
        Image img = LoadImage(path.c_str());
        cards[i] = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    float smallW = 80, smallH = 110;
    float popW = 220, popH = 300;
    int totalCards = 9;
    int poppedIndex = -1;

    float totalWidth = totalCards * smallW + (totalCards - 1) * 10;
    float startX = (1200 - totalWidth) / 2;
    float cardY = 700 - smallH - 20;

    Rectangle cardRects[9];
    for (int i = 0; i < totalCards; i++)
    {
        cardRects[i] = { startX + i * (smallW + 10), cardY, smallW, smallH };
    }

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();
            bool clickedCard = false;

            for (int i = 0; i < totalCards; i++)
            {
                if (CheckCollisionPointRec(mouse, cardRects[i]))
                {
                    poppedIndex = (poppedIndex == i) ? -1 : i;
                    clickedCard = true;
                    break;
                }
            }

            if (!clickedCard)
                poppedIndex = -1;
        }

        BeginDrawing();
            ClearBackground(MAROON);

            // Draw all 9 spade cards at bottom
            for (int i = 0; i < totalCards; i++)
            {
                Color tint = (poppedIndex == i) ? GRAY : WHITE;
                DrawTexturePro(
                    cards[i],
                    { 0, 0, (float)cards[i].width, (float)cards[i].height },
                    cardRects[i],
                    { 0, 0 },
                    0.0f,
                    tint
                );
            }

            // Popped card in center
            if (poppedIndex != -1)
            {
                DrawRectangle(0, 0, 1200, 700, { 0, 0, 0, 140 });

                float bigX = (1200 - popW) / 2;
                float bigY = (700 - popH) / 2 - 30;

                DrawTexturePro(
                    cards[poppedIndex],
                    { 0, 0, (float)cards[poppedIndex].width, (float)cards[poppedIndex].height },
                    { bigX, bigY, popW, popH },
                    { 0, 0 },
                    0.0f,
                    WHITE
                );

                DrawText("Click anywhere to close", 490, 630, 18, LIGHTGRAY);
            }
            else
            {
                DrawText("Click a card to view", 490, 300, 20, LIGHTGRAY);
            }

        EndDrawing();
    }

    // Unload all textures
    for (int i = 0; i < totalCards; i++)
        UnloadTexture(cards[i]);

    CloseWindow();
    return 0;
}