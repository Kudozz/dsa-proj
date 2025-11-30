#include"profile.h"
#include"Player.h"


void viewProfile(string username) {
    Player* player = loadFromFile(username);

    cout << "\n. ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ Player Profile ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ . ݁"<<endl;
    cout<<"\n Username: "<<player->username
        <<"\nTotal points: "<<player->points
        <<"\nTotal matches: "<<player->matchesPlayed
        <<"\nWins: "<<player->wins
        <<"\nLosses: "<<player->losses;

    cout<<endl<<endl;
}