// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <string>
#include <iostream> //
#include <unistd.h>
using namespace std;

#define PORT 5001 
   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    fd_set master;
    fd_set read_fds;
    int fdmax;
    
    
    
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    FD_SET(sock, &master); 
    fdmax = sock; // so far, it's this one 
    FD_SET(STDIN_FILENO, &master);

    char buffer[1024] = {0}; 

    while (1){
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
        perror("select");
        exit(4);
        }
    if (FD_ISSET(STDIN_FILENO, &read_fds)){
        string hello = "Hello from client";
       // cout << "What would you like to send:";
        getline (cin, hello);
        send(sock , hello.c_str() , hello.length() , 0 ); 
       // printf("Hello message sent\n"); 
        valread = recv( sock , buffer, 1024, 0); 
        string response = buffer;
        if (response=="1")
            cout << "I was able to login successfully" << endl;
        else if (response=="2") cout << "Login failed" << endl;
        printf("%s\n",buffer );
        memset(buffer, 0, sizeof buffer);
    }
    if (FD_ISSET(sock, &read_fds)){
         valread = recv( sock , buffer, 1024, 0); 
        string response = buffer;
        if (response=="1")
            cout << "I was able to login successfully" << endl;
        else if (response=="2") cout << "Login failed" << endl;
        printf("%s\n",buffer );
        memset(buffer, 0, sizeof buffer);

    }

    } 
    return 0; 
} 