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

    file<<"Username: "<<player.username;
    file<<"\nPlayer ID: "<<player.playerID;
    file<<"\nTotal matches: "<<player.matchCount;
    file<<"\nWins: "<<player.wins;
    file<<"\nLosses: "<<player.losses;
    file<<"\nDraws: "<<player.draws;
    file<<"\nTotal score: "<<player.totalScore;
    file<<"\nTheme id: "<<player.currentThemeID;

    file<<"Friends: \n";
    
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
    cout<<"Player profile loaded!\n";

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