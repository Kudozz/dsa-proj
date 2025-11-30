#include "friends.h"

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

//constructor
FriendSystem::FriendSystem() {
    p_capacity = 10;
    players = new Player*[p_capacity];
    p_count = 0;

    r_capacity = 10;
    pendingReqs = new FriendRequest[r_capacity];
    r_count = 0;
}

FriendSystem::~FriendSystem() {
    delete[] players;
    delete[] pendingReqs;
}

void FriendSystem::expandPlayers() {
    Player** newArr = new Player*[p_capacity * 2];
    for (int i = 0; i < p_count; i++)
        newArr[i] = players[i];
    delete[] players;
    players = newArr;
    p_capacity *= 2;
}

void FriendSystem::expandRequests() {
    FriendRequest* newArr = new FriendRequest[r_capacity * 2];
    for (int i = 0; i < r_count; i++)
        newArr[i] = pendingReqs[i];
    delete[] pendingReqs;
    pendingReqs = newArr;
    r_capacity *= 2;
}

//encryption and logging
void FriendSystem::encryptAndLog(const string& filename, const string& msg) {
    string out = msg;
    string key = "XONIX_KEY";
    for (size_t i = 0; i < out.size(); i++)
        out[i] ^= key[i % key.size()];

    ofstream file("notifications/" + filename, ios::app);
    if (file.is_open()) {
        file << out << endl;
        file.close();
    }
}

//dec a message
string FriendSystem::decrypt(const string& msg) {
    string out = msg;
    string key = "XONIX_KEY";
    for (size_t i = 0; i < out.size(); i++)
        out[i] ^= key[i % key.size()];
    return out;
}

// get player pointer by username
Player* FriendSystem::getPlayer(const string& username) {
    for (int i = 0; i < p_count; i++) {
        if (players[i]->username == username)
            return players[i];
    }
    return nullptr;
}

//asd a new player to the system
void FriendSystem::addPlayer(Player* p) {
    if (p_count == p_capacity)
        expandPlayers();
    players[p_count++] = p;
}

//send a request
void FriendSystem::sendFriendRequest(const string& from, const string& to) {
    Player* fromP = getPlayer(from);
    Player* toP = getPlayer(to);

    if (!fromP || !toP) {
        cout << "User not found!\n";
        return;
    }

    if (fromP->isFriend(to)) {
        cout << "Already friends!\n";
        return;
    }

    encryptAndLog(to + "_notifications.txt", "Friend request from " + from);

    if (r_count == r_capacity)
        expandRequests();

    pendingReqs[r_count++] = {from, to};

    cout << "Friend request sent to " << to << endl;
}

//azccept request
void FriendSystem::acceptFriendRequest(const string& from, const string& to) {
    Player* fromP = getPlayer(from);
    Player* toP = getPlayer(to);

    if (!fromP || !toP) return;

    fromP->addFriend(to, toP->playerID);
    toP->addFriend(from, fromP->playerID);

    cout << to << " accepted friend request from " << from << endl;

    //remove request from pendingReqs
    for (int i = 0; i < r_count; i++) {
        if (pendingReqs[i].from == from && pendingReqs[i].to == to) {
            for (int j = i; j < r_count - 1; j++)
                pendingReqs[j] = pendingReqs[j + 1];
            r_count--;
            break;
        }
    }
}

//reject a friend request
void FriendSystem::rejectFriendRequest(const string& from, const string& to) {
    cout << to << " rejected friend request from " << from << endl;

    for (int i = 0; i < r_count; i++) {
        if (pendingReqs[i].from == from && pendingReqs[i].to == to) {
            for (int j = i; j < r_count - 1; j++)
                pendingReqs[j] = pendingReqs[j + 1];
            r_count--;
            break;
        }
    }
}

//display notifications for a user
void FriendSystem::displayNotifications(const string& username) {
    string filename = "notifications/" + username + "_notifications.txt";
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "No notifications.\n";
        return;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        cout << decrypt(line) << endl;
        count++;
    }
    file.close();
    remove(filename.c_str());
    cout << "Total notifications: " << count << endl;
}

//display all registered players
void FriendSystem::displayAllPlayers() {
    cout << "All players:\n";
    for (int i = 0; i < p_count; i++)
        cout << "- " << players[i]->username << " (ID: " << players[i]->playerID << ")\n";
}


// #include "friends.h"
// #include <cstring>


// //Wajiha Abbasi 24i-2059
// //Hanaa Sajid  24i-2029
// //PROJECT: XONIX GAME


// //constructorr
// FriendSystem::FriendSystem(){
//     p_capacity = 10;

//     players = new Player*[p_capacity];
//     p_count = 0;

//     r_capacity = 10;
//     pendingReqs = new FriendRequest[r_capacity];
//     r_count = 0;
// }

// // Destructor
// FriendSystem::~FriendSystem() {
//     delete[] players;
//     delete[] pendingReqs;
// }

// // Expand player array if full
// void FriendSystem::expandPlayers() {
//     Player** newArr = new Player*[playerCapacity * 2];
//     for (int i = 0; i < p_count; i++)
//         newArr[i] = players[i];
//     delete[] players;
//     players = newArr;
//     playerCapacity *= 2;
// }

// // Expand requests array if full
// void FriendSystem::expandRequests() {
//     FriendRequest* newArr = new FriendRequest[r_capacity * 2];
//     for (int i = 0; i < requestCount; i++)
//         newArr[i] = pendingReqs[i];
//     delete[] pendingReqs;
//     pendingReqs = newArr;
//     r_capacity *= 2;
// }

// // --- Encryption / Decryption ---
// void FriendSystem::encryptAndLog(const string& filename, const string& msg) {
//     string out = msg;
//     string key = "XONIX_KEY";
//     for (size_t i = 0; i < out.size(); i++)
//         out[i] ^= key[i % key.size()];

//     ofstream file("notifications/" + filename, ios::app);
//     if (file.is_open()) {
//         file << out << endl;
//         file.close();
//     }
// }

// string FriendSystem::decrypt(const string& msg) {
//     string out = msg;
//     string key = "XONIX_KEY";
//     for (size_t i = 0; i < out.size(); i++)
//         out[i] ^= key[i % key.size()];
//     return out;
// }

// // --- Helper ---
// Player* FriendSystem::getPlayer(const string& username) {
//     for (int i = 0; i < p_count; i++) {
//         if (players[i]->username == username)
//             return players[i];
//     }
//     return nullptr;
// }

// // --- Main functions ---
// void FriendSystem::addPlayer(Player* p) {
//     if (p_count == playerCapacity)
//         expandPlayers();
//     players[p_count++] = p;
// }

// void FriendSystem::sendFriendRequest(const string& fromUser, const string& toUser) {
//     Player* fromP = getPlayer(fromUser);
//     Player* toP = getPlayer(toUser);

//     if (!fromP || !toP) {
//         cout << "User not found!\n";
//         return;
//     }

//     if (fromP->isFriend(toUser)) {
//         cout << "Already friends!\n";
//         return;
//     }

//     // Log request
//     encryptAndLog(toUser + "_notifications.txt", "Friend request from " + fromUser);

//     if (requestCount == r_capacity)
//         expandRequests();
//     pendingReqs[requestCount++] = {fromUser, toUser};

//     cout << "Friend request sent to " << toUser << endl;
// }

// void FriendSystem::acceptFriendRequest(const string& fromUser, const string& toUser) {
//     Player* fromP = getPlayer(fromUser);
//     Player* toP = getPlayer(toUser);

//     if (!fromP || !toP) return;

//     fromP->addFriend(toUser, toP->playerID);
//     toP->addFriend(fromUser, fromP->playerID);

//     cout << toUser << " accepted friend request from " << fromUser << endl;

//     // Remove from pendingRequests
//     for (int i = 0; i < requestCount; i++) {
//         if (pendingReqs[i].fromUser == fromUser && pendingReqs[i].toUser == toUser) {
//             for (int j = i; j < requestCount - 1; j++)
//                 pendingReqs[j] = pendingReqs[j + 1];
//             requestCount--;
//             break;
//         }
//     }
// }

// void FriendSystem::rejectFriendRequest(const string& fromUser, const string& toUser) {
//     cout << toUser << " rejected friend request from " << fromUser << endl;

//     for (int i = 0; i < requestCount; i++) {
//         if (pendingReqs[i].fromUser == fromUser && pendingReqs[i].toUser == toUser) {
//             for (int j = i; j < requestCount - 1; j++)
//                 pendingReqs[j] = pendingReqs[j + 1];
//             requestCount--;
//             break;
//         }
//     }
// }

// // --- Notifications ---
// void FriendSystem::displayNotifications(const string& username) {
//     string filename = "notifications/" + username + "_notifications.txt";
//     ifstream file(filename);
//     if (!file.is_open()) {
//         cout << "No notifications.\n";
//         return;
//     }

//     string line;
//     int count = 0;
//     while (getline(file, line)) {
//         cout << decrypt(line) << endl;
//         count++;
//     }
//     file.close();
//     remove(filename.c_str());
//     cout << "Total notifications: " << count << endl;
// }

// void FriendSystem::displayAllPlayers() {
//     cout << "All players:\n";
//     for (int i = 0; i < p_count; i++)
//         cout << "- " << players[i]->username << " (ID: " << players[i]->playerID << ")\n";
// }

