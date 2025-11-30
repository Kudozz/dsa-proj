#ifndef PROFILE_H
#define PROFILE_H

#include"player.h"
#include<iostream>
#include<string>
using namespace std;


//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME


    void profile();
    void addFriend();
    void rmFriend( string friendUser);
    void displayFriends();
    void logMatch(string username, string opponent,int myPoints, int oppPoints);
    void displayMatchHistory();
    int getTotalPoints();
    bool userExistsInAudit(string username,int playerID);
//     void clear();//should clear all friends..after submission



 #endif