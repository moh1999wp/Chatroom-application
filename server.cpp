#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string> 
#include <iostream>
#include <string.h>
#include "client_parser.cpp"
#include "get_clients.cpp"


#define PORT 5001 
using namespace std;



int main(int argc, char const *argv[]) 
{ 
    fd_set master;
    fd_set read_fds;
    int fdmax; // maximum file descriptor number 32 33 int listener; // listening socket descriptor 34 int newfd; 
    int server_fd, new_socket, valread; 
    int nbytes;

    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    string hello = "Hello from server";
    create_clients ("clients.csv",clients);
    //return 0;

       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 10) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    FD_SET(server_fd, &master); 
     fdmax = server_fd; // so far, it's this one 

    // if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
    //                    (socklen_t*)&addrlen))<0) 
    // { 
    //     perror("accept"); 
    //     exit(EXIT_FAILURE); 
    // }
    int session_id=0;


    while (1){

    read_fds = master; // copy it
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
        perror("select");
        exit(4);
    }
            for(int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == server_fd) {
                    // handle new connections
                    addrlen = sizeof address;
					new_socket = accept(server_fd,
						(struct sockaddr *)&address,
						(socklen_t*)&addrlen);

					if (new_socket == -1) {
                        perror("accept");
                    } else {
                        FD_SET(new_socket, &master); // add to master set
                        if (new_socket > fdmax) {    // keep track of the max
                            fdmax = new_socket;
                        }
                        printf("selectserver: new connection from  on "
                            "socket \n");
                    }
                } else {
                    // handle data from a client
                    if ((nbytes = recv(i, buffer, 1024, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        // we got some data from a client
                        string message= buffer;
                        int type,size;
                        string data,source;
                        extractData(type,size,source,data,message);
                    // cout << "The type is:"<< type <<" and the size is:"<< size <<" and the source is:"<< source << " and the data is:"<< data <<endl;
                        string send_info = "The type is:" + to_string(type) + " and the size is:"+ to_string(size) +" and the source is:" + source + " and the data is:" + data;
                        cout <<type <<endl <<flush;
                        
                        string return_value;
                        if(type==LOGIN){
                            int pos= data.find(',');
                            
                            string temp_username= data.substr(0,pos);
                            string temp_password= data.substr(pos+1,data.length());
                            cout << temp_username<<":"<<temp_password<<endl<<flush;

                            if (valid_login(temp_username,temp_password,new_socket)){
                                return_value=" 1";
                                cout << "client "<<temp_username<< " logged in successfully"<<endl<<flush;
                            } else {
                                return_value="2";
                            } 

                            send(new_socket , return_value.c_str() , return_value.size(), 0 ); 
                        }
                        if(type == REGISTER){
                            int pos= data.find(',');
                            
                            string temp_username= data.substr(0,pos);
                            string temp_password= data.substr(pos+1,data.length());
                            cout << temp_username<<":"<<temp_password<<endl<<flush;
                            client new_client;
                            new_client.username=temp_username;
                            new_client.password=temp_password;
                            new_client.online=true;
                            new_client.client_socket=new_socket;
                            clients.push_back(new_client);
                            std::ofstream outfile;
                            outfile.open("clients.csv", std::ios_base::app); // append instead of overwrite
                            outfile << temp_username + "," + temp_password + "\n";
                            return_value = "created client " + temp_username + " successfully";
                            send(new_socket , return_value.c_str() , return_value.size(), 0 );

                        }
                        if(type==NEW_SESS) {
                            int client_asking_for_session;
                            client_asking_for_session=online_client(source);
                            if (client_asking_for_session!=-1){
                                session curr_seassion;
                                curr_seassion.session_id=session_id;
                                session_id++;
                                curr_seassion.session_name=data;
                                curr_seassion.clients_in_session.push_back(&clients[client_asking_for_session]);
                                sessions.push_back(curr_seassion);
                                return_value= "10 " + data;
                                cout << "client " << source << " created session " << data << endl<<flush;
                                send(clients[client_asking_for_session].client_socket , return_value.c_str() , return_value.size(), 0 ); 
                            } else {
                                
                                
                            }
                        }
                        if(type==QUERY){
                            for (int i=0; i<sessions.size(); i++){
                                return_value+= "Session " + sessions[i].session_name + " has clients\n";
                                for (int j=0;j<sessions[i].clients_in_session.size();j++){
                                    return_value+=sessions[i].clients_in_session[j]->username;
                                    return_value+= ", ";
                                }
                                return_value.pop_back();
                                return_value.pop_back();

                                return_value+="\n";

                            }
                            int client_asking_for_session;
                            client_asking_for_session=online_client(source);
                            if (return_value.size()==0)
                                return_value="There are no sessions online at the moment";

                            send(clients[client_asking_for_session].client_socket , return_value.c_str() , return_value.size(), 0 );

                        }
                        if(type==JOIN){
                            int client_asking_to_join;
                            client_asking_to_join=online_client(source);
                            int session_to_join=online_session(data);
                            if(session_to_join==-1){
                                return_value="Joining session " + data + " failed, session does not exist\n";
                                send(clients[client_asking_to_join].client_socket , return_value.c_str() , return_value.size(), 0 );
                            } else {
                                sessions[session_to_join].clients_in_session.push_back(&clients[client_asking_to_join]);
                                return_value="Joined session " + data + "\n";
                                send(clients[client_asking_to_join].client_socket , return_value.c_str() , return_value.size(), 0 );
                            }
                        }
                        if (type==LEAVE_SESS){
                            int client_asking_to_leave;
                            client_asking_to_leave=online_client(source);
                            int session_to_leave=online_session(data);
                            int client_in_server=-1;

                            for(int i=0;i<sessions[session_to_leave].clients_in_session.size();i++){
                                if(sessions[session_to_leave].clients_in_session[i]->username==source)
                                    client_in_server=i;
                            }

                            sessions[session_to_leave].clients_in_session.erase(sessions[session_to_leave].clients_in_session.begin()+client_in_server);
                            if(sessions[session_to_leave].clients_in_session.size()==0)
                                sessions.erase(sessions.begin()+session_to_leave);

                            if(session_to_leave==-1){
                                return_value="leaving session " + data + " failed, session does not exist\n";
                                send(clients[client_asking_to_leave].client_socket , return_value.c_str() , return_value.size(), 0 );
                            } else {
                              //  sessions[session_to_leave].clients_in_session.push_back(&clients[client_asking_to_leave]);
                                return_value="left session " + data + "\n";
                                send(clients[client_asking_to_leave].client_socket , return_value.c_str() , return_value.size(), 0 );
                            }
                        }
                        if (type==MESSAGE){
                            for (int i=0; i<sessions.size(); i++){
                                if (is_client_in_session(source,i)){
                                    for (int j=0; j<sessions[i].clients_in_session.size();j++){
                                        
                                            return_value= "session (" + sessions[i].session_name + ")" ;
                                            return_value+= " " + data + "\n";
                                            send(sessions[i].clients_in_session[j]->client_socket,return_value.c_str() , return_value.size(), 0 );
                                        
                                    }
                                }
                            }


                        }




                        

                        memset(buffer, 0, sizeof buffer);
                        
                    }

                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors



    //valread = recv( new_socket , buffer, 1024,0); 

    }
    return 0; 
} 