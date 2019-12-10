#include "client_parser.h"
#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <sstream>


using namespace std;

void extractData ( int& type , int& size,string& source,string& data, string input) {
    stringstream ss(input);

    ss>>type;
    ss>>size;
    ss>>source;

    string temp;
    while (ss){
        ss>>temp;
        if(ss){
        data+=temp;
        data+=' ';
        }
        
    }
    data.pop_back();
    return;

}
bool valid_login (string username, string password,int socket_client){

    for (int i=0;i<clients.size();i++){
        if (clients[i].username==username && clients[i].password==password){
            clients[i].online=true;
            clients[i].client_socket=socket_client;
            cout<<"Client Logged In"<<endl<<flush;
            return true;
        }
    }
    cout<<"Client doesn't exist"<<endl<<flush;
    return false;

}
int online_client (string username){
    for (int i=0; i<clients.size(); i++){
        if (clients[i].username==username && clients[i].online==true)
            return i;
    }
    return -1;
}
int online_session (string session_name){
    for (int i=0; i<sessions.size(); i++){
        if (sessions[i].session_name==session_name)
            return i;
    }
    return -1;
}
bool is_client_in_session (string client_name,int session_index){
    for (int i=0; i<sessions[session_index].clients_in_session.size(); i++){
        if(sessions[session_index].clients_in_session[i]->username==client_name)
            return true;
    }
    return false;
}

int which_command(string input){
    stringstream ss(input);
    string word;

    ss>>word;
    
    if(word=="/login") return LOGIN;
    if(word=="/logout") return EXIT;
    if(word=="/joinsession") return JOIN;
    if(word=="/leavesession") return LEAVE_SESS;
    if(word=="/createsession") return NEW_SESS;
    if(word=="/list") return QUERY;
    if(word=="/quit") return TERMINATE;

    return -1;
}

// int main (void){
//     struct message mo;

//     string input;
//     cout << "What is your input?: " ;
//     getline (cin, input);
//     int command=-2;
//     if(input[0]=='/') command=which_command(input);

//     if(command==-1) cout << "You inputted not a command\n";
//     else if (command!=-2) cout << "You gave a valid command\n";
//     else cout << "You wrote a message" << endl;

//     if(command==NEW_SESS) cout << "You want to make a session" << endl;

//     stringstream ss(input);

//     int type,size;
//     string data,source;
//     extractData(type,size,source,data,input);
//     cout << "The type is:"<< type <<" and the size is:"<< size <<" and the source is:"<< source << " and the data is:"<< data <<endl;






    

//     return 0;

// }