#ifndef PROFILE_H
#define PROFILE_H

#include"player.h"
#include<iostream>
#include<string>
#include"SystemLogs.h"
using namespace std;


//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME



    void addFriend();
    void rmFriend( string friendUser);
    void displayFriends();
    void logMatch(string username, string opponent,int myPoints, int oppPoints);
    void displayMatchHistory(string username);
    int getTotalPoints(string user);
    bool userExistsInAudit(string username,int playerID);
//     void clear();//should clear all friends..after submission



 #endif