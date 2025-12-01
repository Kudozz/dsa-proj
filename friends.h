#ifndef FRIENDSYSTEM_H
#define FRIENDSYSTEM_H
#include "player.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

// Wajiha Abbasi 24i-2059
// Hanaa Sajid  24i-2029
// PROJECT: XONIX GAME

struct FriendRequest
{
    string from;
    string to;
};

class FriendSystem
{
private:
    Player **players; // dynamic array of player pointers
    int p_count;      // player count
    int p_capacity;   // player capacity

    FriendRequest *pendingReqs; // dynamic array of requests
    int r_count;                // request count
    int r_capacity;             // req capacity

    Player *getPlayer(const string &username);
    void encryptAndLog(const string &filename, const string &msg);
    string decrypt(const string &msg);
    void expandPlayers();
    void expandRequests();

public:
    FriendSystem();
    void addPlayer(Player *p);
    void sendFriendRequest(const string &from, const string &to);
    void acceptFriendRequest(const string &from, const string &to);
    void rejectFriendRequest(const string &from, const string &to);
    void displayNotifications(const string &username);
    void displayAllPlayers();
    ~FriendSystem();
};

#endif