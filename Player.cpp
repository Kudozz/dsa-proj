#include"Player.h"

void saveNewPlayer(string username) {
    Player temp(username);
    
    string friendsString = "";

    fstream playersFile;
    playersFile.open("playersFile.txt", ios::app);
    playersFile<<username<<"|"<<temp.points<<" "<<temp.matchesPlayed<<" "<<temp.wins<<" "<<temp.losses<<" "<<friendsString<<"\n";
    playersFile.close();
}

Player::Player(string username) {
    this->username = username;
    points = 0;
    matchesPlayed = 0;
    wins = 0;
    losses = 0;
    friends = nullptr;
}

Player* loadFromFile(string username) {
    fstream playersFile;
    playersFile.open("playersFile.txt", ios::in);
    string friendsString="";
    //searching for user
    Player* temp = new Player(username);
    string line; 
    while(getline(playersFile, line, '|')) {
        if(line == username) {
            temp->username = username; 
            
            playersFile>>temp->points>>temp->matchesPlayed>>temp->wins>>temp->losses>>friendsString;
        } else {
            getline(playersFile, line, '\n');
        }
    }

    playersFile.close();
    return temp;
}