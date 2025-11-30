// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

// Friend node for linked list
struct FriendNode {
    string username;
    int playerID;
    FriendNode* next;
    
    FriendNode(string user, int id) : username(user), playerID(id), next(nullptr) {}
};

// Match record structure
struct MatchRecord {
    string opponent;
    int myPoints;
    int oppPoints;
    string result; // "WIN", "LOSS", "DRAW"
    
    MatchRecord() : opponent(""), myPoints(0), oppPoints(0), result("") {}
    MatchRecord(string opp, int my, int opp_pts, string res) 
        : opponent(opp), myPoints(my), oppPoints(opp_pts), result(res) {}
};

// Main Player structure
struct Player {
    string username;
    int playerID;
    int totalScore;
    int matchCount;
    int wins;
    int losses;
    int draws;
    int currentThemeID;
    FriendNode* friendsHead;  // Linked list of friends
    
    // Constructor
    Player() : username(""), playerID(0), totalScore(0), matchCount(0), 
               wins(0), losses(0), draws(0), currentThemeID(1), friendsHead(nullptr) {}
    
    Player(string user, int id) : username(user), playerID(id), totalScore(0), 
                                   matchCount(0), wins(0), losses(0), draws(0), 
                                   currentThemeID(1), friendsHead(nullptr) {}
    
    // Destructor to free friends list
    ~Player() {
        FriendNode* current = friendsHead;
        while (current) {
            FriendNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    
    // Add friend to player's friend list
    void addFriend(string friendUsername, int friendID) {
        FriendNode* newNode = new FriendNode(friendUsername, friendID);
        newNode->next = friendsHead;
        friendsHead = newNode;
    }
    
    // Check if already friends
    bool isFriend(string friendUsername) {
        FriendNode* current = friendsHead;
        while (current) {
            if (current->username == friendUsername) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Get friend count
    int getFriendCount() {
        int count = 0;
        FriendNode* current = friendsHead;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }
    
    // Add match result and update stats
    void addMatchResult(string opponent, int myPoints, int oppPoints) {
        matchCount++;
        totalScore += myPoints;
        
        if (myPoints > oppPoints) {
            wins++;
        } else if (myPoints < oppPoints) {
            losses++;
        } else {
            draws++;
        }
    }
    
    // Calculate win rate
    float getWinRate() {
        if (matchCount == 0) return 0.0f;
        return (float)wins / matchCount * 100.0f;
    }
    
    // Display player stats
    void displayStats() {
        cout << "\n⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
        cout << "           PLAYER STATS\n";
        cout << "⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
        cout << "Username: " << username << "\n";
        cout << "Player ID: " << playerID << "\n";
        cout << "Total Score: " << totalScore << "\n";
        cout << "Matches Played: " << matchCount << "\n";
        cout << "Wins: " << wins << " | Losses: " << losses << " | Draws: " << draws << "\n";
        cout << "Win Rate: " << getWinRate() << "%\n";
        cout << "Friends: " << getFriendCount() << "\n";
        cout << "Current Theme ID: " << currentThemeID << "\n";
        cout << "⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
    }
    
    // Display friends list
    void displayFriends() {
        if (friendsHead == nullptr) {
            cout << "\nNo friends added yet!\n";
            return;
        }
        
        cout << "\n⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
        cout << "           YOUR FRIENDS\n";
        cout << "⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
        
        FriendNode* current = friendsHead;
        int count = 1;
        while (current) {
            cout << count++ << ". " << current->username << " (ID: " << current->playerID << ")\n";
            current = current->next;
        }
        
        cout << "⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
    }
};

// Global functions for player management
Player* loadPlayer(const string& username);
void savePlayer(const Player& player);
void createNewPlayer(const string& username, int playerID);
Player* getCurrentPlayer();
void setCurrentPlayer(Player* player);
void clearCurrentPlayer();

#endif // PLAYER_H
