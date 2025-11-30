#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>
using namespace std;

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME


bool signup();
bool login();
bool isValidPassword(const string& password);
bool usernameExists(const string& username);
string hashPassword(const string& password);
bool verifyPassword(const string& username, const string& password);
void saveCredentials(const string& username, const string& hashedPassword);
void setCurrentUser(const string& username);
string getCurrentUser();
void clearCurrentUser();
bool isUserLoggedIn();




#endif