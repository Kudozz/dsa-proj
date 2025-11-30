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

void profile(){

int choice;

            cout<<"\n ⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨ Profile Menu ୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ \n"
            <<"1. Add friend\n"
            <<"2. Remove friend\n"
            <<"3. Display match history\n "
            <<"4. View friends\n"
            <<"5. Total points\n"
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

               // rmFriend(name);

                break;
                }

                case 3:
                displayMatchHistory(getCurrentUser());

                break;

                case 4:
                displayFriends();
                break;

                case 5:
                getTotalPoints(getCurrentUser());
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

    // string friendsFile = "profile/" + currentUser + "_friends.txt";
    // ifstream file(friendsFile);
    
    // if (!file.is_open()) {
    //     cout << "\nNo friends added yet!\n";
    //     return;
    // }
    
    // string line;
    // while (getline(file, line)){
    //     size_t pos = line.find(':');
        
    //     if(pos != string::npos){
    //         string username = line.substr(0,pos);
    //         string playerID = line.substr(pos+1);
            
    //         friends.addFriend(username,playerID);
    //     }
    // }
    
    // file.close();
    // friends.displayFriends();
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

    // string filename = "profile/" + username +"_matches.txt";
    // ofstream file(filename, ios::app);
    
    // if(file.is_open()){
    //     string result;
        
    //     if(myPoints>oppPoints){
    //         result ="WIN";
    //     }

    //     else if(myPoints<oppPoints){
    //         result ="LOSS";
    //     }

    //     else{
    //         result ="DRAW";
    //     }
        
    //     file << opponent <<":"<<to_string(myPoints)<<":"<<to_string(oppPoints)<<":"<<result<<endl;

    //     file.close();
    // }
}

//displays match history by reading user's matches file
void displayMatchHistory(string username){
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


    // string line;
    // int matchNum = 1;
    
    // while (getline(file, line)){
    //     size_t pos1 = line.find(':');
    //     size_t pos2 = line.find(':',(pos1+1));
    //     size_t pos3 = line.find(':',(pos2+1));
        
    //     if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos){
    //         string opponent = line.substr(0, pos1);
    //         string myScore = line.substr((pos1+1),(pos2-pos1-1));
    //         string oppScore = line.substr(pos2 + 1, pos3 - pos2 - 1);
    //         string result = line.substr(pos3 + 1);
            
    //         cout<<"Match "<<matchNum++<<": vs "<<opponent 
    //             <<"Score: "<<myScore <<" - "<<oppScore 
    //             <<"Result: "<<result <<"\n";
    //     }
    // }
    
    //cout << "⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ \n";
    //file.close();
}

//total points calculated by reading user's points from their matches file
int getTotalPoints(string user){

    Player* player=getCurrentPlayer();

    if(!player){
        return 0;
    }

    return player->totalScore;
    // string filename ="profile/"+ user +"_matches.txt";
    
    // ifstream file(filename);
    
    // if(!file.is_open()){
    //     return 0;
    // }
    
    // int total=0;
    // string line;
    
    // while(getline(file, line)){

    //     int pos1=line.find(':');
    //     int pos2=line.find(':',(pos1+1));
        
    //     if((pos1!= -1)&&(pos2!= -1)){
    //         string myScore = line.substr((pos1+1),(pos2-pos1-1));

    //         total += stoi(myScore);
    //     }

    // }
    
    // file.close();
    // return total;
}