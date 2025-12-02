#include<iostream>
#include"playerPool.h"
#include"authentication.h"

PlayerPool::PlayerPool() {
    capacity = 30; 
    size = 0;
    players = new Player *[capacity];
    inQueue =  new bool[capacity];

    for(int i=0; i<capacity; i++)
        inQueue[i] = false;
}


void PlayerPool::loadFromFile() {
    fstream playerNamesFile; 
    playerNamesFile.open("audit.txt", ios::in);


    string username, line;
    while(getline(playerNamesFile, username, ':')) {
        players[size++] = loadPlayer(username);
        //cout<<"\nLoaded "<<username;
       // cout<<"\nDEBUG loaded from profile "<<players[size-1]->username;
        getline(playerNamesFile, line, '\n');
    }

    playerNamesFile.close();
}

Player* PlayerPool::getRandomPlayer() {
    int index = rand() % (size); 
    while(inQueue[index])
        index = (index+1)%size;

    if(!inQueue[index] && players[index]->username != getCurrentUser()) {
        inQueue[index] = true;
      //  cout<<"DEBUG player at "<<index<<" IS "<<players[index]->username;
        return players[index]; 
    }
    return nullptr;
}