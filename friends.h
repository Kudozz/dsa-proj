#ifndef FRIENDSYSTEM_H
#define FRIENDSYSTEM_H
#include "Player.h"
#include <string>
#include <iostream>
#include<cstdio>
#include <fstream>
using namespace std;

// Wajiha Abbasi 24i-2059
// Hanaa Sajid  24i-2029
// PROJECT: XONIX GAME

struct hNode{
    string user;
    int playerId;
    hNode* next;

    hNode(const string &name, int id){
        user =name;
        playerId =id;
    }

};


class Hashtable{
    hNode** table;
    int size;

    int hashfunc(const string& key){
        int h=0;

        int l= key.length();

        for(size_t i=0; i<l; i++){
            h= (h*3 + key[i])%size;
        }

        return h;

    }
    
    public:
        Hashtable(int capacity=100);
        int search(const string& user);
        bool exists(const string& user);
        void insert(const string& name, int playerID);
        void remove(const string& user);
        ~Hashtable();

};

struct FriendRequest
{
    string from;
    string to;
};

class FriendSystem
{
private:
    FriendRequest *pendingReqs;// dynamic array of requests
    int r_count;               // request count
    int r_capacity;              // req capacity

    Hashtable players;

    void encryptAndLog(const string &filename, const string &msg);
    string decrypt(const string &msg);
    void expandRequests();

public:
    FriendSystem();
    int getPlayerID(const string& name);
    bool isPlayer(const string& name);
    void loadPlayers();
    void addPlayer(const string& name, int playerID);
    void sendFriendRequest(const string &from, const string &to);
    void acceptFriendRequest(const string &from, const string &to);
    void rejectFriendRequest(const string &from, const string &to);
    void displayNotifications(const string &username);
    void displayPendingRequests(const string& username);
    void findMutualFriends(const string& user1, const string& user2);    
    int NotificationCount(const string& username);



    ~FriendSystem();
};

#endif