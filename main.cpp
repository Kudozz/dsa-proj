#include<iostream>
#include<SFML/Graphics.hpp>
#include "authentication.h"
#include "friends.h"
#include "inventory.h"
#include "leaderboard.h"
#include "profile.h"
#include "singlePlayer.h"
#include "multiPlayer.h"

using namespace std;


//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME


Inventory i;

void mainMenu();

int main() {

    initializeInventory();

    cout << "\n\n\t\t. ݁₊ ⊹ . ݁˖ . ݁.𖥔 ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ . ݁\t"<<endl
        <<"\n\n\t\t\t\t Welcome Gamer\n\n "<<endl
        <<"\t\t. ݁₊ ⊹ . ݁˖ . ݁.𖥔 ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ . ݁\t"<<endl;

    while(true) {
        int x;
        cout<<"\n (1) Login"
            <<"\n (2) Signup "
            <<"\n (0) Exit"<<endl;
        cin>>x;

       switch(x) { 
        case 0:
            return 0;
        case 1:
            if (login()){
                mainMenu();
                clearCurrentUser();
            }
            break;

        case 2: {
            if(signup()){
                mainMenu();
                clearCurrentUser();
            }
            break;
        }
        default: {
            cout<<"\nInvalid input, please try again";
            continue;
        }
       }
    }
    
    return 0;
}

void mainMenu() {
    while(true) {
    cout<<"\n\n . ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ Main Menu ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ . ݁ "<<endl<<endl
        <<"(1) Single Player"<<endl
        <<"(2) Multiplayer"<<endl
        <<"(3) Leaderboard"<<endl
        //<<"(4) Inventory"<<endl
        <<"(5) Profile"<<endl
        <<"(6) Friends"<<endl
        <<"(0) Logout"<<endl;

    int op;
    cin>>op;

    switch(op) {
        case 0: {
            return;
        }
        case 1: {
            singlePlayer();
            break;
        } 
        case 2: {
            multiPlayer();
            break;
        }
        case 3: {
          //  leaderboard();
            break;
        }
        case 4: {
            inventory();
            break;
        } 
        case 5: {
            //profile();
            break;
        }
        case 6: {
         //   friends();
            break;
        }
        default: {
            cout<<"\nInvalid option, please try again";
            break;
        }
    }
    }
}
