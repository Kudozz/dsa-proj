#ifndef PLAYER_H
#define PLAYER_H

#include<iostream>
#include<fstream>
using namespace std;

struct Player {
    string username;
    int points;
    int matchesPlayed; 
    int wins;
    int losses;
    Player* friends;

    Player(string username);
};

Player* loadFromFile(string username);

void saveNewPlayer(string username);

#endif