#ifndef PLAYERPOOL_H
#define PLAYERPOOL_H
#include"Player.h"
#include<string>
#include<fstream>

class PlayerPool {
    Player** players;
    int size; 
    bool* inQueue; 
    int capacity;
public: 
    PlayerPool();
    void loadFromFile(); 
    Player* getRandomPlayer();
   // void markInQueue(string username, bool status);
};

#endif