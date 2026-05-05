#pragma once
#include "renderer.h"
#include "entities.h"
#include <vector>
#include <string>

using namespace std;

class TimeManager {
public:
    float totalTime = 60.0f;
    float currentTime = 60.0f;

    void update();
    void reset();
};

class RoundManager {
public:
    vector<Move> moves;
    string leadsuit = "";
    string currentBestSuit = "";
    int currentBest = 0;
    int currentPlayer = 0;
    bool isaifirstmove=false;
    int winner = -1;
    bool finished = false;

    void startRound(int player_id, Player* players[4]);
    bool isValidMove(Player* p, int cardIndex);
    bool updateRound(Player* players[4]);
    int getWinner();
};

class GameManager {
public:
    RoundManager roundManager;
    TimeManager timeManager;
    int roundsPlayed = 0;
    bool gameOver = false;
    float botDelay = 0.0f;
    const float BOT_DELAY = 1.0f;
    float roundEndDelay = 0.0f;
    const float ROUND_END_DELAY = 2.0f;
    bool waitingForNextRound = false;

    void updateGame(Player* players[4]);
    void calculateScores(Player* players[4]);
};