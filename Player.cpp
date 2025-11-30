#include"player.h"

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