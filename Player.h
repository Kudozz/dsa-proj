#ifndef PLAYER_H
#define PLAYER_H

#include<iostream>
#include<fstream>
using namespace std;

struct Player {
    string username;
    int playerID;
    int totalScore;
    int matchCount; 
    int wins;
    int losses;
    int draws;
    int currentThemeID;

    Player* friends;

    Player(string username, int id, int totalScore, int matchCount, int wins, int losses, int draws, int currentThemeID);
};

#endif