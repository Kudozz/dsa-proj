#ifndef MATCHMAKING_H
#define MATCHMAKING_H
#include<ctime>
#include"Player.h"

struct QueueEntry {
    Player* player; 
    time_t entryTime; 
    bool isBot;

    QueueEntry(Player*&, time_t, bool);
};

class MatchmakingQueue {
    private:
        QueueEntry** heap;
        int capacity;
        int size;
    public: 
        MatchmakingQueue(int);
        void enqueue(Player* player, bool isBot);
        Player* dequeue();
        Player* peek(); 
        int getSize();
        void remove(string username);
        int getPosition(string username);
        void display();
        bool contains(string username);
        void heapify(int);
        void bottomUpHeapify(int);
};



class PlayerPool {
    Player** players;
    int count; 
    bool inQueue; 
    int size;
public: 
    PlayerPool();
    void loadFromFile(); 
    Player* getRandomPlayer();
    void markInQueue(string username, bool status);
};

void matchmakingMenu();

#endif 