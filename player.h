#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>
using namespace std;



struct FriendNode{
    string username;
    int playerID;
    FriendNode* next;

    FriendNode(string user, int id)
        : username(user), playerID(id), next(nullptr) {}
};


struct Player{
     string username;
    int playerID;
    int totalScore;
    int matchCount; 
    int wins;
    int losses;
    int draws;
    int currentThemeID;

    FriendNode* friendsHead;

    // ConstructorS
    Player()
        : username(""),
          playerID(0),
          totalScore(0),
          matchCount(0),
          wins(0),
          losses(0),
          draws(0),
          currentThemeID(1),
          friendsHead(nullptr) {}

    Player(string user, int id)
        : username(user),
          playerID(id),
          totalScore(0),
          matchCount(0),
          wins(0),
          losses(0),
          draws(0),
          currentThemeID(1),
          friendsHead(nullptr) {}


    //for leaderboard
    Player(string user, int id, int total, int matches, int w, int l, int d, int theme)
    : username(user), playerID(id), totalScore(total), matchCount(matches),
      wins(w), losses(l), draws(d), currentThemeID(theme), friendsHead(nullptr) {}


    // Destructor
    ~Player() {
        FriendNode* current = friendsHead;
        while (current) {
            FriendNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

 
    void addFriend(string friendUsername, int friendID) {
        FriendNode* newNode = new FriendNode(friendUsername, friendID);
        newNode->next = friendsHead;
        friendsHead = newNode;
    }


    void rmFriend(string friendUser){
    FriendNode* current = friendsHead;
    FriendNode* prev = nullptr;

    while (current) {
        if (current->username == friendUser) {
            if (prev) {
                prev->next = current->next;
            } else {
                friendsHead = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

    bool isFriend(string friendUsername) {
        FriendNode* current = friendsHead;
        while (current) {
            if (current->username == friendUsername)
                return true;
            current = current->next;
        }
        return false;
    }

    int getFriendCount() {
        int cnt = 0;
        FriendNode* current = friendsHead;
        while (current) {
            cnt++;
            current = current->next;
        }
        return cnt;
    }


    void addMatchResult(string opponent, int myPoints, int oppPoints) {
        matchCount++;
        totalScore += myPoints;

        if (myPoints > oppPoints)
            wins++;
        else if (myPoints < oppPoints)
            losses++;
        else
            draws++;
    }


    void displayStats() {
        cout << "\n⋆ Player Stats ⋆\n";
        cout << "Username: " << username << "\n";
        cout << "Player ID: " << playerID << "\n";
        cout << "Total Score: " << totalScore << "\n";
        cout << "Matches Played: " << matchCount << "\n";
        cout << "Wins: " << wins
             << " | Losses: " << losses
             << " | Draws: " << draws << "\n";
        cout << "Friends: " << getFriendCount() << "\n";
        cout << "Theme ID: " << currentThemeID << "\n";
    }

    void displayFriends() {
        if (!friendsHead) {
            cout << "\nNo friends added yet!\n";
            return;
        }

        cout << "\n⋆ Friend List ⋆\n";
        FriendNode* current = friendsHead;
        int counter = 1;

        while (current) {
            cout << counter++ << ". " << current->username
                 << " (ID: " << current->playerID << ")\n";
            current = current->next;
        }
    }
};


Player* loadPlayer(const string& username);
void savePlayer(const Player& player);
void createNewPlayer(const string& username, int playerID);

Player* getCurrentPlayer();
void setCurrentPlayer(Player* player);
void clearCurrentPlayer();

#endif


