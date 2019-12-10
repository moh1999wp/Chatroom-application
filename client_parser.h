#pragma once

#include <string>
#include <vector>
using namespace std;
#define LOGIN 0
#define LO_ACK 1
#define LO_NAK 2
#define EXIT 4
#define JOIN 5
#define JN_ACK 6
#define JN_NAK 7
#define LEAVE_SESS 8
#define NEW_SESS 9
#define NS_ACK 10
#define MESSAGE 11
#define QUERY 12
#define QU_ACK 13
#define TERMINATE 14
#define REGISTER 15

struct message {
    unsigned int type;
    unsigned int size;
    string source;
    string data;
};

struct client {
    int client_id;
    string username;
    string password;
    bool online=false;
    int client_socket;
};

struct session {
    int session_id;
    string session_name;
    vector<client*> clients_in_session;
};

vector<client> clients;
vector<session> sessions;






