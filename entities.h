#pragma once

#include <string>
#include "raylib.h"

class Card {
public:
    int suit, value;
    Texture2D texture;

    Card();

    void load(int cardNum);
    void unload();

    bool operator>(const Card& o) const;
};

class Deck {
public:
    int raw[52];

    Deck();

    void shuffle();
    int cardAt(int i);
};

class Player {
public:
    std::string name;
    Card hand[13];
    int handSize;

    int bid, tricksWon, score;
    bool isHuman;

    Player();
    Player(std::string n, bool human);

    void receiveCard(int cardNum);
    void unloadCards();
    Card playCard(int index);
};

class AIPlayer : public Player {
public:
    AIPlayer();
    AIPlayer(std::string n);

    int chooseBid();
    int chooseCard(int leadSuit);
};