#include "friends.h"
#include"Player.h"
#include"SystemLogs.h"

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME


int Hashtable::search(const string& user){
        int index = hashfunc(user);

        hNode* temp= table[index];

        while(temp){
            if(temp->user == user){
            return (temp->playerId);

            }

        temp= temp->next;
        }

        return -1;
}

void Hashtable::insert(const string& name, int playerID){
    int x =hashfunc(name);

    hNode* newNode= new hNode(name,playerID);
    newNode->next = table[x];

    table[x]= newNode;
}

bool Hashtable::exists(const string& user){
     return (search(user)!=-1);
}

void Hashtable::remove(const string& user){
        int idx= hashfunc(user);

        hNode* temp= table[idx];
        hNode* ptr= nullptr;


        while(temp){
            if(temp->user == user){
                if(ptr){
                    ptr->next = temp->next;
                }

                else{
                    table[idx]= temp->next;
                }
                temp = temp->next;
                delete temp;
                return;
            }
        }
}


Hashtable::~Hashtable(){
            for(int i=0;i<size; i++){
                hNode* curent= table[i];

                while(curent){
                    hNode* temp= curent;
                    curent= curent->next;
                    delete temp;
                }
            }

            delete[]  table;
}

Hashtable::Hashtable(int capacity){
    size = capacity;

    table = new hNode*[size];

    for(int i=0; i<size;i++){
        table[i]= nullptr;
    }

}



//constructor
FriendSystem::FriendSystem(){
    players= 100;
    r_capacity = 10;
    pendingReqs = new FriendRequest[r_capacity];
    r_count = 0;

}

FriendSystem::~FriendSystem() {
    delete[] pendingReqs;
}


void FriendSystem::expandRequests() {
    FriendRequest* newArr = new FriendRequest[r_capacity*2];
    
    for(int i = 0; i<r_count; i++){
        newArr[i] = pendingReqs[i];
    }

    delete[] pendingReqs;
    
    pendingReqs = newArr;
    r_capacity *= 2;

}

//encryption and logging
void FriendSystem::encryptAndLog(const string& filename, const string& msg) {
    string out = msg;
    string key = "POTATOES";

    int count = out.size();

    for(size_t i = 0; i < count; i++){
        out[i]^= key[i% key.size()];

    }
    
    ofstream file;
    file.open("notifications/" + filename, ios::app);
    
    if(file.is_open()){
        file<< out << endl;
        file.close();
    }

    else{
        cout<<"Eror opening file";

    }
}

//dec a message
string FriendSystem::decrypt(const string& msg){
    string out = msg;
    string key = "POTATOES";

    int count = out.size();

    for(size_t i = 0; i < count ; i++){
        out[i] ^= key[i % key.size()];
    }


    return out;

}

bool FriendSystem::isPlayer(const string& name){
    return (players.exists(name));
}

int FriendSystem::getPlayerID(const string& name){
    return (players.search(name));
}

void FriendSystem::addPlayer(const string& name, int playerID){
    players.insert(name, playerID);
}

void FriendSystem::loadPlayers(){
    ifstream f;

    f.open("audit.txt");

    if(!f.is_open()){
        cout<<"error opening file"<<endl;
        return;
    }

    string line;
    int count =0;

while(getline(f,line)){    
    string x="",username="";
    int cCount =0;
    int playerID=0;

    int l= line.length();

    for(size_t i=0; i < l; i++){
        if(line[i] ==':'){
           cCount++;
            
           if(cCount == 1){
                username = x;//save username
            }
    
            x = ""; //resetting for next field
    
        } 
    
        else if(cCount == 3){
            x += line[i];
        }
        
        else{
                x += line[i];
        }

    }
        

    string idStr = "";  //extracting player id from x
    int xl = x.length();

    for(size_t i = 0; i < xl; i++){

        if((x[i] >='0') && (x[i] <= '9')){
            idStr += x[i];  //digits only
        }

    }
        
    if(!idStr.empty() && !username.empty()){
        playerID = stoi(idStr);
        addPlayer(username, playerID);
        count++;
    }
}
    
    f.close();
    cout << "Loaded "<< count <<" players from audit.txt\n";
}


//send a request
void FriendSystem::sendFriendRequest(const string& from, const string& to){
    
    if(!isPlayer(from)){
        cout<<"error "<<from<<" does not exist"<<endl;
        return;
    }


    if(!isPlayer(to)){
        cout<<"error "<<to<<" does not exist"<<endl;
        return;
    }

    if(from==to){
        cout<<"You cannot send a  friend request to yourself"<<endl;
        return;
    }

    Player* curr = getCurrentPlayer();

    // if(curr && curr->username ==from){ 
    //     if(curr->isFriend(to)){
    //         cout<<"You and "<<to<<" are already friends!\n";
    //         return;

    //     }
    // }

    for(int i= 0; i< r_count; i++){
        if((pendingReqs[i].from == from) && (pendingReqs[i].to == to)){
            cout<<"Friend request has already been sent to "<<to<<endl;
            return;
        }
    }

    encryptAndLog(to+"_notifications.txt", "Friend request from " + from);

    if(r_count == r_capacity){
        expandRequests();
    }

    pendingReqs[r_count++] = {from, to};

    cout<<"Friend request sent to "<< to << endl;
    logSysActivity(from,"Send friend request to"+to, "SUCCESS");

}

//azccept request
void FriendSystem::acceptFriendRequest(const string& from, const string& to){
    bool found = false;
    int idx = -1;
    
    for(int i = 0; i < r_count; i++){
        if((pendingReqs[i].from == from) && (pendingReqs[i].to == to)){
            found = true;
            idx = i;

            break;
        }
    }

    if(!found){
        cout<<"No pending friend request from "<<from<<endl;

        return;
    }

    int from_ID = getPlayerID(from);
    int to_ID = getPlayerID(to);

    Player* p = getCurrentPlayer();
    
    if(p && (p->username == to)){
        p->addFriend(from,from_ID);
        savePlayer(*p);
    }

    Player* sender = loadPlayer(from);//load sender's profile and add fren:))
    
    if(sender){
        sender->addFriend(to, to_ID);
        savePlayer(*sender);

        delete sender;
    }

    cout<< to <<" accepted friend request from "<< from <<endl;

    encryptAndLog(from +"_notifications.txt", to +" accepted your friend request!!!1");

    //removes request from pending
    for(int i = idx; i < r_count - 1; i++){
        pendingReqs[i] = pendingReqs[i+1];
    }

    r_count--;

    logSysActivity(to,"Accept friend request from "+ from,"SUCCESS");
}

//rejecting friend request
void FriendSystem::rejectFriendRequest(const string& from, const string& to){
    bool found = false;
    
    for(int i = 0; i < r_count; i++){
        
        if((pendingReqs[i].from == from) && (pendingReqs[i].to == to)){
            found = true;
            cout<< to <<" rejected friend request from "<< from<<endl;


            for(int j = i; j < r_count - 1; j++){//removing request from pending
                pendingReqs[j] = pendingReqs[j + 1];
            }
                r_count--;
            
            logSysActivity(to, "Reject friend request from " + from, "SUCCESS");
            
            break;
        }
    }

    if(!found){
        cout<<"No pending friend request from "<< from <<endl;
    }

}

//notification coun to display it when user logins in 
int FriendSystem::NotificationCount(const string& username){
    string filename = "notifications/" + username + "_notifications.txt";
    
    ifstream file(filename);
    
    if(!file.is_open()){
        return 0;
    }

    string line;
    int count = 0;
    
    while(getline(file, line)){
        count++;
    }

    file.close();

    return count;
}

//dislay notifs
void FriendSystem::displayNotifications(const string& username){
    string filename = "notifications/" + username + "_notifications.txt";
    
    ifstream f(filename);
    
    if(!f.is_open()){
        cout <<"No notifications.\n";
        
        return;
    }

    string line;
    int count = 0;
    
    cout <<"\n⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨ Notifications ୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
    
    while (getline(f, line)) {
        cout <<(count+1)<<". "<< decrypt(line) << endl;
        count++;
    }

    cout<<"⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
    f.close();

    // delete the file after displaying
    if(remove(filename.c_str()) == 0){
        cout<<"All notifications cleared.\n";
    }
}

//all pending requests displayed
void FriendSystem::displayPendingRequests(const string& username){
    bool found = false;
    
    cout <<"\n⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨ Pending Requests ୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
    
    for(int i = 0; i < r_count; i++){
        if(pendingReqs[i].to == username){
            cout <<"- Request from: "<< pendingReqs[i].from << endl;
            
            found = true;
        }
    }
    
    if(!found){
        cout<<"No pending friend requests\n";
    }
    cout <<"⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
}

//mutual friends
void FriendSystem::findMutualFriends(const string& user1, const string& user2) {
    
    //do both user exist??
    if(!isPlayer(user1)){
        cout << "User: "<< user1 <<" does not exist!\n";
        return;
    }
    
    if(!isPlayer(user2)){
        cout <<"User: "<< user2 <<" does not exist!\n";
        return;
    }

    if(user1 == user2){
        cout <<"Cannot find mutual friends with yourself!!!1\n";
        return;
    }

    //loads both players
    Player* player1 = loadPlayer(user1);
    Player* player2 = loadPlayer(user2);

    if(!player1 || !player2){
        cout<<"Error loading player profiles!\n";

        if(player1){
            delete player1;
        }

        if(player2){
            delete player2;
        }

        return;
    }

    //traversing both linked lists to find mutual friends
    cout <<"\n⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨ Mutual Friends ୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
    cout <<"Between "<< user1 <<" and "<< user2 <<":"<<endl;
    
    int count = 0;
    FriendNode* temp = player1->friendsHead;

    while(temp){
        FriendNode* ptr = player2->friendsHead;//does friend exist in user's list?
        
        while(ptr){
            if(temp->username == ptr->username){
                cout <<"- "<< temp->username <<" (ID: "<< temp->playerID <<")"<<endl;
                count++;
                
                break;
            }

            ptr =ptr->next;
        }

        temp = temp->next;
    }

    if(count == 0){
        cout<<"No mutual friends found.\n";
    } 
    
    else{
        cout <<"\nTotal mutual friends: "<< count<< endl;
    }
    
    cout <<"⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊ \n";

    delete player1;//cleanup memory
    delete player2;
}