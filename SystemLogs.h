#ifndef SYSTEMLOGS_H
#define SYSTEMLOGS_H


//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME


#include <string>
using namespace std;

string hashString(const string& str);
void logSysActivity(const string& username,const string& action, const string&status);

#endif