#include "authentication.h"
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>

using namespace std;

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

void saveCredentials(const string& username, const string& hashedPassword){
    ofstream file("audit.txt", ios::app);
    
    time_t t = time(0);
    char *tt = ctime(&t);
    string timeStamp= string (tt);

    if(!timeStamp.empty() && (timeStamp[timeStamp.length() -1]=='\n')){
        timeStamp.erase(timeStamp.length()-1);
    }

    if(file.is_open()){
        file<< username <<":"<< hashedPassword <<","<<timeStamp<< endl;
        file.close();
    } 
    
    else{
        cout<<"\nError!!!1!"<<endl;
    }

}

//verify pass during login
bool verifyPassword(const string& username, const string& password){
    ifstream file("audit.txt");
    
    if(!file.is_open()){
        return false;
    }
    
    string line, user, hash;
    string inputHash = hashPassword(password);
    
   while(getline(file, line)){
        user ="";
        hash ="";
         int i = 0;
         int l=line.length();
    
        //read username
        while((i <l) && (line[i] !=':')){
            user += line[i];
            
            i++;
        }
    
        if(i >=l){
            continue; //no colon found...skip line
        }
    
        i++; //skip the colon
    
        while((i<l)&&(line[i]!=' ')&&(line[i]!=',')){
            hash += line[i];
            i++;
        }
    
        if((user== username) && (hash == inputHash)){
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
    
    string username, password, confirmPassword;
    
    cout<<"Enter username: ";
    cin>>username;
    
    if(usernameExists(username)){
        cout<<"\nUsername already exists! Please choose a different username"<<endl;
        
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
        return false;
    }
    
    cout<<"Confirm password: ";
    cin>>confirmPassword;
    
    if(password != confirmPassword){
        cout<<"\nPasswords do not match!" << endl;
        
        return false;
    }
    
    string hashedPassword = hashPassword(password);
    
    saveCredentials(username, hashedPassword);
    
    setCurrentUser(username);
    
    cout<<"\nRegistration successful! Welcome, "<<username<<"!"<<endl;
    return true;
}

bool login(){
    cout<<"\n\n\t✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦"<<endl;
    cout<<"\t              USER LOGIN                "<<endl;
    cout<<"\t✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦\n"<<endl;
    
    string username, password;
    int attempts = 0;
    const int max_attempts= 3;
    
    cout<<"Enter username: ";
    cin>>username;
    
    if(!usernameExists(username)){
        cout<<"\nUsername not found! Please sign up first"<<endl;
        
        return false;
    }
    
    while(attempts< max_attempts){
        
        cout<<"Enter password(Attempt "<<(attempts+1)<<"/"<<max_attempts <<"): ";
        cin >> password;
        
        if(verifyPassword(username, password)){
          
            setCurrentUser(username);
          
            cout<<"\nLogin successful! Welcome back, "<<username <<"!"<<endl;

            return true;
        } 
        
        else{
            attempts++;
            
            if(attempts< max_attempts){
                cout<<"\nIncorrect password! "<<(max_attempts - attempts)<<" attempt(s) remaining"<<endl;
            }

        }
    }


    
    cout<<"\nMaximum login attempts exceeded....Access denied!"<<endl;
    return false;
}