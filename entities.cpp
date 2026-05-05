
#include "entities.h"
#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
using namespace std;
Card::Card() : suit("spades"), value(0), index(0) {
    texture = {0};
}


void Card::load(int cardNum) {
    int n = (cardNum - 1) / 13;
    value = (cardNum - 1) % 13 + 2;
    string suits_array[4]={"spades", "clubs", "hearts", "diamonds"};
    suit=suits_array[n];

    string name = to_string(value) + "_of_" + suit + ".png";

    texture = LoadTexture(("cards/" + name).c_str());
    index = cardNum;
}

void Card::unload() {
    if (texture.id > 0)
        UnloadTexture(texture);
}



// ───────── DECK ─────────
Deck::Deck() {
    srand((unsigned)time(0));
    for (int i = 0; i < 52; i++)
        raw[i] = i + 1;
}

void Deck::shuffle() {
    for (int i = 51; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(raw[i], raw[j]);
    }
}

int Deck::cardAt(int i) {
    return raw[i];
}

// ───────── PLAYER ─────────
Player::Player() {
    handSize = 0;
    bid = tricksWon = score = 0;
    isHuman = false;
}
int Player::getCardIndex(string leadSuit, int currentBest, bool isAIFirstMove) {
    if (isHuman) {
        Vector2 mouse = GetMousePosition();
        for (int i = 0; i < handSize; i++) {
            if (CheckCollisionPointRec(mouse, rects[i]) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                return i;
            }
        }
        return -1;
    } else {
        if(isAIFirstMove){
            return 0;
            isAIFirstMove=false;

        }
        else{

        // ───────── check if must beat ─────────
        bool mustBeat = false;

        for (int i = 0; i < handSize; i++) {
            if (hand[i].suit == leadSuit && hand[i].value > currentBest) {
                mustBeat = true;
                break;
            }
        }

        // ───────── if must beat, choose smallest winning card ─────────
        if (mustBeat) {
            int best = -1;

            for (int i = 0; i < handSize; i++) {
                if (hand[i].suit == leadSuit && hand[i].value > currentBest) {
                    if (best == -1 || hand[i].value < hand[best].value) {
                        best = i;
                    }
                }
            }

            return best;
        }

        // ───────── follow suit normally ─────────
        int lowestLead = -1;
        int lowestLeadVal = 15;

        for (int i = 0; i < handSize; i++) {
            if (hand[i].suit == leadSuit) {
                if (hand[i].value < lowestLeadVal) {
                    lowestLeadVal = hand[i].value;
                    lowestLead = i;
                }
            }
        }

        if (lowestLead != -1) {
            return lowestLead;
        }

        // ───────── no lead suit → play smallest spade ─────────
        int smallestTrump = -1;
        int smallestTrumpVal = 15;

        for (int i = 0; i < handSize; i++) {
            if (hand[i].suit == "spades") {
                if (hand[i].value < smallestTrumpVal) {
                    smallestTrumpVal = hand[i].value;
                    smallestTrump = i;
                }
            }
        }

        if (smallestTrump != -1) return smallestTrump;

        // ───────── fallback ─────────
        int lowestAny = 0;
        int lowestAnyVal = 15;

        for (int i = 0; i < handSize; i++) {
            if (hand[i].value < lowestAnyVal) {
                lowestAnyVal = hand[i].value;
                lowestAny = i;
            }
        }

        return lowestAny;
     }
    }

}
Player::Player(string n,int id,bool human) {
    name = n;
    player_id = id;
    isHuman = human;
    handSize = 0;
    bid = tricksWon = score = 0;
    isRevealed = false;
}

void Player::receiveCard(int cardNum) {
    if (handSize < 13) {
        hand[handSize].load(cardNum);
        handSize++;
    }
}

void Player::unloadCards() {
    for (int i = 0; i < handSize; i++)
        hand[i].unload();

    handSize = 0;
}

Card Player::playCard(int index) {
    Card played = hand[index];

    swap(hand[index], hand[handSize - 1]);
    handSize--;

    return played;
}

// ───────── AI PLAYER ─────────
AIPlayer::AIPlayer() {
    handSize = 0;
    bid = tricksWon = score = 0;
    isHuman = false;
    isRevealed = false;
}

AIPlayer::AIPlayer(string n,int id) {
    name = n;
    player_id = id;
    handSize = 0;
    bid = tricksWon = score = 0;
    isHuman = false;
    isRevealed = false;
}

int AIPlayer::chooseBid() {
    int strong = 0;

    for (int i = 0; i < handSize; i++) {
        if (hand[i].suit == "spades" || hand[i].value >= 11)
            strong++;
    }
    
    return max(1, (int)(1.2 * strong / 2));
   // return max(1,(int)(1.5*strong / 2));
}
