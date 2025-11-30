#ifndef PROFILE_H
#define PROFILE_H

#include<string>
#include<fstream>
using namespace std;

// struct FriendNode {
//     string username;
//     string playerID;
//     FriendNode* next;
    
//     FriendNode(string user, string id) : username(user), playerID(id), next(nullptr) {}
// };



// class FriendsList {
// private:
//     FriendNode* head;
    
// public:
//     FriendsList();
//     ~FriendsList();
//     void addFriend(string username, string playerID);
//     void displayFriends();
//     bool isEmpty();
// };


bool userExistsInAudit(string username, string& playerID);
void addFriend(string currentUser);
void displayFriends(string currentUser);
void logMatch(string username, string opponent, int myPoints, int oppPoints);
void displayMatchHistory(string username);
int getTotalPoints(string username);

#endif