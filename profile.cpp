#include "profile.h"
#include "Player.h"
#include "SystemLogs.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include"authentication.h"
using namespace std;

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

static FriendSystem* friends = nullptr;
void profile(){
   

int choice;

            cout<<"\n ⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨ Profile Menu ୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ \n"
            <<"1. Add friend\n"
            <<"2. Remove friend\n"
            <<"3. View friends\n"
            <<"4. Send friend request\n"
            <<"5. View pending friend requests\n"
            <<"6. Accept friend request\n"
            <<"7. Reject friend request\n"
            <<"8. Find mutual friends\n"
            <<"9. View notifications\n"
            <<"10. Display match history\n"
            <<"11. Total points\n"
            <<"0. Exit\n"
            <<"Enter choice: ";
            cin>>choice;

            switch(choice){
                case 0:
                return;

                case 1:
                addFriend();
                break;

                case 2:{
                cout<<endl;
                string name;
                cout<<"Friend's username: ";
                cin>>name;

                 Player* player= getCurrentPlayer();

                if(player){
                    player->rmFriend(name);

                }

                else{
                    cout<<"no player loaded!!!"<<endl;
                }

                break;
                }

                case 3:{
                    displayFriends();
                    break;
                }

                

                case 4:{
                    Player* player= getCurrentPlayer();

                    if(!player){
                        cout<<"no player loaded!!"<<endl;
                        return;
                    }

                    string fUser;//friend's username

                    cout<<"Enter friend's username: ";
                    cin>>fUser;

                    FriendSystem* fs = getFriendSys();
                    fs->sendFriendRequest(player->username, fUser);

                }
                

                case 5:{
                    Player* player= getCurrentPlayer();

                    if(!player){
                        cout<<"no player loaded!!"<<endl;
                        return;
                    }

                    FriendSystem* fs = getFriendSys();
                    fs->displayPendingRequests(player->username);
                }

                case 6:{
                    Player* player= getCurrentPlayer();

                    if(!player){
                        cout<<"no player loaded!!"<<endl;
                        return;
                    }
                    FriendSystem* fs= getFriendSys();
                    fs->displayPendingRequests(player->username);

                    string from;
                    cout<<"Accept request from: ";
                    cin>>from;

                    fs->acceptFriendRequest(from,player->username);
                }

                case 7:{
                    Player* player= getCurrentPlayer();

                    if(!player){
                        cout<<"no player loaded!!"<<endl;
                        return;
                    }

                    FriendSystem* fs= getFriendSys();
                    fs->displayPendingRequests(player->username);

                    string from;
                    cout<<"Reject request from: ";
                    cin>>from;

                    fs->rejectFriendRequest(from,player->username);
                }

                case 8:{
                    Player* player= getCurrentPlayer();

                    if(!player){
                        cout<<"no player loaded!!"<<endl;
                        return;
                    }

                    
                    string f;
                    cout<<"Enter friend's name: ";
                    cin>>f;

                    FriendSystem* fs= getFriendSys();
                    fs->findMutualFriends(player->username,f);

                }

                case 9:{
                    Player* player= getCurrentPlayer();

                    if(!player){
                        cout<<"no player loaded!!"<<endl;
                        return;
                    }

                    FriendSystem* fs= getFriendSys();
                    fs->displayNotifications(player->username);
                }


                
                case 10:{
                    displayMatchHistory();
                    break;
                }

                case 11:
                    getTotalPoints();
                break;


            }

            cout<<"\t\t\t ⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ "<<endl;

}

//DOES THE USER EXIST IIN AUDIT.TXT HUHHH??? DOES IT ??? DOES IT NOWWW???HUHHHHHHHHH??
bool userExistsInAudit(string username,int playerID){
    ifstream file;
    file.open("audit.txt");

    if(!file.is_open()){
        return false;
    }
    
    string line;
    
    while(getline(file,line)){
        int pos1 = line.find(':');
        
        if(pos1 == -1){
            continue;
        }
        
        string user = line.substr(0,pos1);
        
        if (user == username){
            int pos2 = line.find(':',(pos1+1));
            int pos3 = line.find(':',(pos2+1));
            
            if (pos3!= -1) {
                string idField = line.substr(pos3+1);//EVERYTHING AFTER THE 3RD COLON
                
                int idPos = idField.find("PlayerID- ");
                if (idPos != -1) {
                    string afterID = idField.substr(idPos+10);//Skip "PlayerID- "
                    int commaPos = afterID.find(',');
                    if (commaPos != -1) {
                        playerID = stoi(afterID.substr(0, commaPos));
                    } else {
                        playerID = stoi(afterID);
                    }
                } else {
                    playerID = stoi(idField);
                }
                file.close();
                return true;
            }
        }
    }
    
    file.close();
    return false;
}

// Add friend
void addFriend(){
    Player* player= getCurrentPlayer();

    if(!player){
        cout<<"no player loasded!"<<endl;
        return;

    }


    string friendUser;
    cout<<"\nEnter friend's username: ";
    cin>>friendUser;
    
    if(friendUser== player->username){
        cout<<"\nYou cannot add yourself as a friend!\n";
        return;
    }
    

    if(player->isFriend(friendUser)){
        cout<<"\nYou are already friends with "<<friendUser<<" !\n";
        return;
    }

    int playerID;//friend's id
    
    if(!userExistsInAudit(friendUser,playerID)){
        cout<<"\nUser '"<<friendUser<<"' does not exist!!1!\n";
        
        return;

    }

    player->addFriend(friendUser, playerID);
    savePlayer(*player);
    cout<<"Friend added successfully!!";

    logSysActivity(player->username,"Add friend","SUCCESS");


}

//display friends(me n my lil twin tower)
void displayFriends(){

    Player* player = getCurrentPlayer();
    
    
     if(!player){
        cout<<"no player loasded!"<<endl;
        return;

    }

    player->displayFriends();


}

//logging math resiult to user's mathes file
void logMatch(string username, string opponent,int myPoints, int oppPoints){
    Player* player= getCurrentPlayer();

     if(!player){
        cout<<"no player loasded!"<<endl;
        return;

    }

    player->addMatchResult(opponent,myPoints,oppPoints);
    savePlayer(*player);

    logSysActivity(player->username,"Logging match result","SUCCESS" );

    cout<<"Match logged successfully!!!1"<<endl;


    
  
}

//displays match history by reading user's matches file
void displayMatchHistory(){
    Player* player=getCurrentPlayer();

     if(!player){
        cout<<"no player loasded!"<<endl;
        return;

    }

    
    cout<<"\n⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ \n"
    <<"           MATCH HISTORY\n"
    <<"⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ \n"
    <<"Username: "<<player->username
    <<"\nPlayer ID: "<<player->playerID
    <<"\nTotal matches: "<<player->matchCount
    <<"\nWins: "<<player->wins
    <<"\nTotal score: "<<player->totalScore
    <<"\n⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";


}

//total points calculated by reading user's points from their matches file
int getTotalPoints(){

    Player* player=getCurrentPlayer();

    if(!player){
        return 0;
    }

    return player->totalScore;

}


void initiateFriendSys(){
    if(!friends){
        friends = new FriendSystem();
        friends->loadPlayers();

    }
}

FriendSystem* getFriendSys(){
    if(!friends){
        initiateFriendSys();
    }

    return friends;
}

void cleanup(){
    if(friends){
        delete friends;

        friends = nullptr;
    }
}