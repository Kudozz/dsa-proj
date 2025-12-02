#include "authentication.h"
#include"SystemLogs.h"
#include"Player.h"
#include"SaveGame.h"
#include"profile.h"
#include"friends.h"
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>

using namespace std;

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

extern  SaveGameManager save;//it alr exists in singleplayer file
static string currentUser = "";

void setCurrentUser(const string& username){

    currentUser = username;

}

string getCurrentUser(){
    return currentUser;
}

void clearCurrentUser(){
    currentUser ="";
}

bool isUserLoggedIn(){
    return !currentUser.empty();

}

int getPlayerID(){
    int IDcount =1;

    ifstream f("playerIDcount.txt");

    if(f.is_open()){
        f>>IDcount;
        f.close();

    }
    
    int nextID=IDcount+1;

    ofstream o("playerIDcount.txt");
    if(o.is_open()){
        o<<nextID;
        o.close();

    }

    return IDcount;
    
}


string hashPassword(const string& password){
    long long hash = 12;
    int l = password.length();

    for(int i = 0; i <l; i++){
        hash =((hash<<2)+ hash)+ password[i]; 
    }

    return to_string(hash);
}



bool isValidPassword(const string& password){
    if(password.length()< 8){
        cout<<"\npassword must be at least 8 characters long!"<<endl;

        return false;
    }
    
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    int l = password.length();
    
    for(int i = 0; i<l; i++){
        char c = password[i];
    
        if (isupper(c)){ 
            hasUpper = true;
        }

        else if (islower(c)){
             hasLower = true;
        }
    
        else if (isdigit(c)){
             hasDigit = true;
        }
    
        else if (ispunct(c)){
            hasSpecial = true;
        }

    }
    
    if(!hasUpper){
        cout<<"\nPassword must contain at least one uppercase letter!"<<endl;
        return false;
    }
    if (!hasLower){
        cout<<"\nPassword must contain at least one lowercase letter!"<<endl;
        return false;
    }

    if(!hasDigit){
        cout<<"\nPassword must contain at least one digit!"<<endl;
        return false;
    }

    if(!hasSpecial){
        cout<<"\nPassword must contain at least one special character!"<<endl;

        return false;
    }
    
    return true;
}

//does user alr exist??
bool usernameExists(const string& username){
    ifstream file("audit.txt");
    
    if(!file.is_open()){
        return false; 
    }
    
    string line, storedUser;
  
    
    while (getline(file, line)){
        int l = line.length();
        storedUser = "";
        int i = 0;
        
        //username(before colon)
        while((i<l) && (line[i] !=':')){
            storedUser += line[i];
            i++;
        }
        
        if(storedUser == username){
            file.close();
            return true;
        }
    }
    
    file.close();
    return false;
}

void saveCredentials(const string& username, const string& hashedPassword,const string& hashedSecQ){
    ofstream file("audit.txt", ios::app);
    
    time_t t = time(0);
    char *tt = ctime(&t);
    string timeStamp= string (tt);

    if(!timeStamp.empty() && (timeStamp[timeStamp.length() -1]=='\n')){
        timeStamp.erase(timeStamp.length()-1);
    }

    int playerID= getPlayerID();

    if(file.is_open()){
        file<< username <<":"<< hashedPassword <<":"<< hashedSecQ <<":PlayerID- "<< playerID<<","<<timeStamp<< endl;
        file.close();

        cout<<"Your Player ID: "<<playerID<<endl;
    } 
    
    else{
        cout<<"\nError!!!1!"<<endl;
    }

}

bool verifySecurityAnswer(const string& username, const string& answer){
    ifstream file("audit.txt");
    
    if(!file.is_open()){
        return false;
    }
    
    string line, user, passHash, secHash;
    string inputHash = hashString(answer);
    
    while(getline(file, line)){
        user = "";
        passHash ="";
        secHash ="";
        int i=0;
        int l=line.length();
    
        while((i<l)&&(line[i]!=':')){
            user += line[i];
            i++;
        }

        if(i >= l){
            continue;
        }

        i++;//skip colon
        
        //akip pass hash
        while((i<l)&&(line[i]!=':')){
            i++;
        }


        if(i>=l){
            continue;
        }

        i++;//skip colon
        
        while((i<l)&& (line[i]!= ':')){
            secHash += line[i];
            i++;
        }
    
        if((user==username)&&(secHash==inputHash)){
            file.close();
            return true;
        }

    }

    
    file.close();
    return false;

}



bool verifyPassword(const string& username, const string& password){
    ifstream file("audit.txt");
    
    if(!file.is_open()){
        return false;
    }
    
    string line;
    string inputHash = hashPassword(password);
    
    while(getline(file, line)){
        int firstCol = line.find(':');
        if(firstCol == string::npos) continue;
        
        string user = line.substr(0,firstCol);
        
        int secondCol = line.find(':',(firstCol+1));
        if(secondCol == string::npos) continue;
        
        string hash = line.substr((firstCol+1),(secondCol-firstCol-1));
        
        if((user==username)&&(hash == inputHash)){
            file.close();
            return true;
        }
    }
    
    file.close();
    return false;
}

bool signup(){

    cout<<"\n\n\n\t    ⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ "<<endl;
    cout<<"\t      USER REGISTRATION"<<endl;
    cout<<"\t    ⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ \n"<<endl;
    
    string username, password, confirmPassword,securityAns;
    
    cout<<"Enter username: ";
    cin>>username;
    
    if(usernameExists(username)){
        cout<<"\nUsername already exists! Please choose a different username"<<endl;
        logSysActivity(username,"Registration_Attemp","FAILED-Username exits");
       
        return false;
    }

    
    cout<<"\nPassword Requirements:"<<endl;
    cout<<" -At least 8 characters"<<endl;
    cout<<" -At least 1 uppercase letter"<<endl;
    cout<<" -At least 1 lowercase letter"<<endl;
    cout<<" -At least 1 digit"<<endl;
    cout<<" -At least 1 special character"<<endl;
    
    cout<<"\nEnter password: ";
    cin>>password;
    

    if(!isValidPassword(password)){
        logSysActivity(username,"Registration_Attemp","FAILED-invalid password");
        return false;
    }
    
    cout<<"Confirm password: ";
    cin>>confirmPassword;
    
    if(password != confirmPassword){
        cout<<"\nPasswords do not match!" << endl;
        logSysActivity(username,"Registration_Attemp","FAILED-password mismatch");

        return false;
    }
    
    cout<<"\nWhat was th name of your first grade science teacher: ";
    cin.ignore();
    getline(cin,securityAns);

    if(securityAns.empty()){
        cout<<"\n security ans can not be empty!!"<<endl;
        logSysActivity(username,"Registration_Attemp","FAILED-invalid security answer");

        return false;
    }

    string hashedPassword = hashPassword(password);
    string hashedAns =hashString(securityAns);
    
    saveCredentials(username,hashedPassword,hashedAns);
    int newID= getPlayerID();
    
    createNewPlayer(username,newID);
    setCurrentUser(username);
    
    Player* player =loadPlayer(username); 
    
    if(player){
        setCurrentPlayer(player);
        
        FriendSystem*  fs =getFriendSys();
        fs->addPlayer(username,newID);

    }

    cout<<"\nRegistration successful! Welcome, "<<username<<"!"<<endl;
    logSysActivity(username,"Registration","SUCCESS");
    
    return true;
}

void SaveGameMenu(const string& user){
    cout<<"\n\n\t    ⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ "<<endl;
    cout<<"\t         SAVED GAME DETECTED"<<endl;
    cout<<"\t    ⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ \n"<<endl;
    
    string saveID = save.getMostRecentSave(user);//last save 

    cout<<"\n\n\t    ⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ "<<endl;
    cout<<"What would you like to do?"<<endl;
    cout<<"1. Continue saved game"<<endl;
    cout<<"2. Start new game"<<endl;
    cout<<"Enter choice: ";

}

bool SaveGameChoice(const string& username){
    int choice;
    cin>>choice;

    string saveID= save.getMostRecentSave(username);

    switch(choice){
        case 1:{
            cout<<"Loading saved game...."<<endl;
            logSysActivity(username,"Continued Saved game","SUCCESS");
            return true;
        }

        case 2:{
            char c;
            cout<<"Starting new game will delete your saved game. Ar you sure(y/n)?";
            cin>>c;

            
            if(c == 'y' || c == 'Y') {
                if(!saveID.empty()) {
                    if(save.deleteSave(saveID)) {
                        cout<<"\n  Previous save deleted."<<endl;
                        cout<<" You can start a new game from the main menu.\n"<<endl;
                        logSysActivity(username,"Deleted save to start new game","SUCCESS");
                    } 
                    
                    else{
                        cout<<"\n Failed to delete save file."<<endl;
                    }

                } 
                
                else {
                    cout<<"\n  No save file found to delete.\n"<<endl;
                }
            } 
            
            else{
                cout<<"\n  Save deletion cancelled."<<endl;
                SaveGameChoice(username);
                
                return false;
            }

            break;
        }
        
        case 3: {
            cout<<"\n Continuing to main menu."<<endl;
            cout<<"  You can manage your save from the Single Player menu.\n"<<endl;
            logSysActivity(username, "Deferred save decision to main menu", "INFO");
            break;
        }
        
        default: {
            cout<<"\n  Invalid choice! Please enter 1-3."<<endl;
            SaveGameChoice(username);
            break;
        }

    }
    return false;
}
bool login(){
    cout<<"\n\n\t✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦"<<endl;
    cout<<"\t           USER LOGIN"<<endl;
    cout<<"\t✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦\n"<<endl;
    
    string username,teacher, password;
    int attempts = 0;
    const int max_attempts= 3;
    
    cout<<"Enter username: ";
    cin>>username;
    
    if(!usernameExists(username)){
        cout<<"\nUsername not found! Please sign up first"<<endl;
        logSysActivity(username,"Login_Attempt","FAILED-user does not exist");
        
        return false;
    }
    
    while(attempts< max_attempts){
        
        cout<<"Enter password(Attempt "<<(attempts+1)<<"/"<<max_attempts <<"): ";
        cin >> password;
        
        cout<<"Enter name of first grade science teacher: ";
        cin>>teacher;


        if(verifyPassword(username, password)&&(verifySecurityAnswer(username,teacher))){
          
            setCurrentUser(username);

            Player* player= loadPlayer(username);

            if(player){
            setCurrentPlayer(player);
        
          
            FriendSystem* fs= getFriendSys();
            if(!fs->isPlayer(username)){//addd in sys if only alr present
                fs->addPlayer(username,player->playerID);
            } 

            int notifs= fs->NotificationCount(username);
            if(notifs>0){
                cout<<"\n⋆. 𐙚 ̊  You have "<< notifs <<" notification(s)! ⋆. 𐙚 ̊ \n";
            }

            cout<<"\nLogin successful! Welcome back, "<<username <<"!"<<endl;
            logSysActivity(username,"Login","SUCCESS");
             
            return true;
            }
        } 
        
        else{
            attempts++;
            
            if(attempts< max_attempts){
                cout<<"\nIncorrect password! "<<(max_attempts - attempts)<<" attempt(s) remaining"<<endl;
            }

        }
    }


    
    cout<<"\nMaximum login attempts exceeded....Access denied!"<<endl;
    logSysActivity(username,"Login_Attempt","FAILED-max attepts exceeded");

    return false;
}