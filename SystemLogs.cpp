#include "authentication.h"
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

string hashString(const string& str){
    long long hash=5;

    int l =str.length();

    for(int i=0; i<l;i++){
        hash=((hash<<3)+hash)+str[i];
    }

    return to_string(hash);
    
}

void logSysActivity(const string& username,const string& action, const string&status){
    ofstream  o("SystemLogs.txt",ios::app);

    if(!o.is_open()){
        cout<<"ERROR!!!"<<endl;
        return;
    }

    time_t t=time(0);
    char* tt= ctime(&t);
    string timeStamp= string(tt);

    if(!timeStamp.empty() && (timeStamp[timeStamp.length()-1]=='\n')){
        timeStamp.erase(timeStamp.length()-1);
    }


    string h_user= hashString(username);//hashed username

    o<<"Time: "<<timeStamp<<endl;
    o<<"User: "<<h_user<<endl;
    o<<"Action: "<<status<<endl<<endl;

    o.close();
}


