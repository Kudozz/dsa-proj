#include"Player.h"

#include "player.h"
#include "SystemLogs.h"
#include <iostream>
#include <fstream>
using namespace std;


static Player* currPlayer = nullptr;


void savePlayer(const Player& player){
    string filename = "profiles/"+player.username+".txt";
    
    ofstream file;
    file.open(filename);

    if(!file.is_open()){
        cout<<"Error!1!!!\n";

        return;
    }

    file<<"USERNAME "<<player.username;
    file<<"\nPLAYER_ID "<<player.playerID;
    file<<"\nMATCH_COUNT "<<player.matchCount;
    file<<"\nWINS "<<player.wins;
    file<<"\nLOSSES "<<player.losses;
    file<<"\nDRAWS "<<player.draws;
    file<<"\nTOTAL_SCORE "<<player.totalScore;
    file<<"\nTHEME_ID "<<player.currentThemeID;

    file<<"\nFRIENDS \n";
    
    FriendNode* curr = player.friendsHead;


    while(curr){
        file<<curr->username<<" "<<curr->playerID<<endl;
        curr = curr->next;
    }

    file<<"END_FRIENDS"<<endl;

    file.close();

    cout<<"Player profile saved!!!!"<<endl;
    logSysActivity(player.username,"Saving player profile","SUCCESS");


}

Player* loadPlayer(const string& user){
    string filename ="profiles/"+user +".txt";
    ifstream file;

    file.open(filename);
    
    if(!file.is_open()){
        cout<<"No profile found for "<<user<<".\n";

        return nullptr;
    }

    Player* player=new Player();
    string tag;

    while(file>>tag){
        if(tag =="USERNAME"){
            file>>player->username;
        }

        else if(tag=="PLAYER_ID"){
            file>>player->playerID;
        }

        else if(tag=="TOTAL_SCORE"){
           file>>player->totalScore;
        }

        else if(tag=="MATCH_COUNT"){ 
            file>>player->matchCount;
        }

        else if(tag=="WINS"){ 
            file>>player->wins;
        }


        else if(tag=="LOSSES"){
            file>>player->losses;
        }

        else if(tag=="DRAWS"){
            file>>player->draws;
        }

        else if(tag== "THEME_ID"){
            file>>player->currentThemeID;
        }

        else if(tag== "FRIENDS"){
            string fuser;
            int fid;

            while(file>> fuser && fuser!="END_FRIENDS"){
                file>> fid;
                player->addFriend(fuser,fid);
            }
        }
    }

    file.close();
//    cout<<"Player profile loaded!\n";

    logSysActivity(player->username,"Loading player","SUCCESS");

    return player;
}


void createNewPlayer(const string& username,int playerID){
    Player newPlayer(username, playerID);
    
    savePlayer(newPlayer);
}


Player* getCurrentPlayer(){
    return currPlayer;
}

void setCurrentPlayer(Player* player){
    currPlayer= player;
}

void clearCurrentPlayer(){
    if(currPlayer){
        delete currPlayer;
      
        currPlayer = nullptr;
    }
}

// Player::Player(string username, int id, int totalScore, int matchCount, int wins, int losses, int draws, int currentThemeID) {
//     this->username = username;
//     this->playerID = id;
//     this->totalScore =totalScore;
//     this->matchCount = matchCount;
//     this->wins = wins;
//     this->losses = losses;
//     this->draws = draws;
//     this->currentThemeID = currentThemeID;
//     friends = nullptr;
// }

// void saveNewPlayer(string username) {
//     Player temp(username);
    
//     string friendsString = "";

//     fstream playersFile;
//     playersFile.open("playersFile.txt", ios::app);
//     playersFile<<username<<"|"<<temp.points<<" "<<temp.matchesPlayed<<" "<<temp.wins<<" "<<temp.losses<<" "<<friendsString<<"\n";
//     playersFile.close();
// }

// Player::Player(string username) {
//     this->username = username;
//     points = 0;
//     matchesPlayed = 0;
//     wins = 0;
//     losses = 0;
//     friends = nullptr;
// }

// Player* loadFromFile(string username) {
//     fstream playersFile;
//     playersFile.open("playersFile.txt", ios::in);
//     string friendsString="";
//     //searching for user
//     Player* temp = new Player(username);
//     string line; 
//     while(getline(playersFile, line, '|')) {
//         if(line == username) {
//             temp->username = username; 
            
//             playersFile>>temp->points>>temp->matchesPlayed>>temp->wins>>temp->losses>>friendsString;
//         } else {
//             getline(playersFile, line, '\n');
//         }
//     }

//     playersFile.close();
//     return temp;
// }