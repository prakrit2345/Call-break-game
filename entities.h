#pragma once

#include <string>

#include "raylib.h"

class Card {
public:
    std::string suit;
    int value;
    Texture2D texture;
    int index;

    Card();

    void load(int cardNum);
    void unload();

};

class Deck {
public:
    int raw[52];

    Deck();

    void shuffle();
    int cardAt(int i);
};

struct Move {
    int player_id;
    Card cardPlayed;
};

class Player {
public:
   
    std::string name;
    int player_id;
    Card hand[13];
    int handSize;
    Rectangle rects[13];

    int bid, tricksWon, score;
    bool isHuman;
    bool isRevealed;
    Player();
    Player(std::string n,int id,bool human);

    void receiveCard(int cardNum);
    void unloadCards();
    int getCardIndex(std::string leadSuit, int currentBest,bool isAIFirstMove);
    void getPlayedCard();
    Card playCard(int index);
    
};

class AIPlayer : public Player {
public:
    AIPlayer();
    AIPlayer(std::string n,int id);

    int chooseBid();
    int chooseCard(std::string leadSuit);
};