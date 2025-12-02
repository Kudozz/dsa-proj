#ifndef PLAYERPOOL_H
#define PLAYERPOOL_H
#include"Player.h"
#include<string>
#include<fstream>

class PlayerPool {
    bool* inQueue; 
    int capacity;
public:
    Player **players;
    int size;
    PlayerPool();
    void loadFromFile(); 
    Player* getRandomPlayer();
   // void markInQueue(string username, bool status);
};

#endif