#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>
using namespace std;

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