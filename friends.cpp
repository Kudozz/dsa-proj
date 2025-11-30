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

