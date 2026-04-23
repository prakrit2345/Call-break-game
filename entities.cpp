
#include "entities.h"
#include <cstdlib>
#include <ctime>
using namespace std;
Card::Card() : suit(0), value(0) {
    texture = {0};
}

void Card::load(int cardNum) {
    suit = (cardNum - 1) / 13;
    value = (cardNum - 1) % 13 + 1;

    string suits[] = {"spades", "clubs", "hearts", "diamonds"};
    string name = to_string(value) + "_of_" + suits[suit] + ".png";

    texture = LoadTexture(("cards/" + name).c_str());
}

void Card::unload() {
    if (texture.id > 0)
        UnloadTexture(texture);
}

bool Card::operator>(const Card& o) const {
    if (suit == o.suit) return value > o.value;
    return suit == 0;
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

Player::Player(string n, bool human) {
    name = n;
    isHuman = human;
    handSize = 0;
    bid = tricksWon = score = 0;
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
AIPlayer::AIPlayer() {}

AIPlayer::AIPlayer(string n) {
    name = n;
    isHuman = false;
}

int AIPlayer::chooseBid() {
    int strong = 0;

    for (int i = 0; i < handSize; i++) {
        if (hand[i].suit == 0 || hand[i].value >= 11)
            strong++;
    }

    return max(1, strong / 2);
}

int AIPlayer::chooseCard(int leadSuit) {
    for (int i = 0; i < handSize; i++) {
        if (hand[i].suit == leadSuit)
            return i;
    }

    return 0;
}