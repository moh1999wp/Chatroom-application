#include "client_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>



using namespace std;
void create_clients (string file, vector<client>& clients){

    ifstream infile(file);
    string word;
    string row;
    int curr_client=0;

    while (getline(infile,row)){

       // stringstream s(row);
        client current;
        current.client_id=curr_client;
        curr_client++;
        int pos= row.find(',');
        
        current.username= row.substr(0,pos);
        current.password= row.substr(pos+1,row.length());
        cout<<current.username<<"End"<<endl;
        cout<<current.password<<"End"<<endl;

       //getline(s,word, ',');
        clients.push_back(current);
    }
    return;
}

