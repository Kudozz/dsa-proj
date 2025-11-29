#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H
#include"gameCommon.h"
#include<SFML/Graphics.hpp>


struct Player {
    int x, y, dx, dy; 
    int points, threshold, multiplier, rewardCounter, powerups;
    bool alive;
    Player(int x, int y);
    void reset();
    void clamp();
    void die();
    bool isMoving();
};

// Flood fill function for capturing area
void drop(int y, int x);

// Game mode
void multiPlayer();

void handleCapture(Player &player, Enemy a[], int enemyCount, int ownCaptured);
void updatePlayerMovement(Player &Player, int ownTrail, int oppTrail);

void checkEnemyCollision(Player& player, int trail, Enemy a[], int enemyCount);
void checkPlayerCollision(Player &player1, Player &player2);

bool isPlayerConstructing(Player& player);


#endif