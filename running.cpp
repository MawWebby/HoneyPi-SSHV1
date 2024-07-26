//////////////////
// Dependencies //
//////////////////
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>
#include <netdb.h>


int startupchecks = 0;


struct addrinfo hints, *res;

int sock, valread;
struct sockaddr_in serv_addr;
std::string hello = "Hello from client";
char buffer[1024] = {0};



////////////////////////////
// Send to Logger Scripts //
////////////////////////////
void sendtolog(std::string data2) {
    std::cout << data2 << std::endl;
}
void sendtologopen(std::string data2) {
    std::cout << data2;
}
void sendtologclosed(std::string data2) {
    std::cout << data2 << std::endl;
}
void loginfo(std::string data2) {
    data2 = "[INFO] - " + data2;
    sendtolog(data2);
}
void logwarning(std::string data2) {
    data2 = "[WARNING] - " + data2;
    sendtolog(data2);
}
void logcritical(std::string data2) {
    data2 = "[CRITICAL] - " + data2;
    sendtolog(data2);
}



std::string heartbeat = "heartbeatSSH";
std::string attacksend = "attacked";




///////////////////////////////////////
// HANDLE NETWORKED CONNECTIONS (22) //
///////////////////////////////////////
/*
void handleSSHConnections(int server_fd) { 
    char buffer[1024] = {0};
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int new_socket;
    ssize_t valread;
    std::string hello = "Hello from server";

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(true) {
        read(new_socket, buffer, 1024);

        if (buffer != NULL) {
            loginfo(buffer);
        } else {
            
        }

        // Send a hello message to the client
//        send(new_socket, hello.c_str(), hello.size(), 0);
//        std::cout << "Hello message sent" << std::endl;
    }
}
*/





/////////////////////////////////
//// SETUP MAIN RUNNING LOOP //// 
/////////////////////////////////
int setup() {

    int PORT = 63599;
    loginfo("finishing SSH Guest V1 startup...");


    //////////////////////////////////////////////////////
    // START CLIENT CONNECTION TO MAIN HONEYPI ON 63599 //
    //////////////////////////////////////////////////////

    

    

    
    //////////////////////////////////
    // START HOSTING ON SSH PORT 22 //
    //////////////////////////////////

/*
    PORT = 22;

    int server_fd2, new_socket2;
    ssize_t valread2;
    struct sockaddr_in address2;
    socklen_t addrlen2 = sizeof(address2);
    int opt2 = 1;
    
    sleep(1);

    if((server_fd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 22
    if (setsockopt(server_fd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt2, sizeof(opt2))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // REACHED HERE
    address2.sin_family = AF_INET;
    address2.sin_addr.s_addr = INADDR_ANY;
    address2.sin_port = htons(PORT);

    // Binding the socket to the network address and port
    if (bind(server_fd2, (struct sockaddr*)&address2, sizeof(address2)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd2, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // SERVER PORT LISTEN THREAD(22)
    sendtologopen("[INFO] - Creating server thread on port 22 listen...");

    sleep(2);
    std::thread acceptingClientsThread2(handleSSHConnections, server_fd2);
    acceptingClientsThread2.detach();
    sleep(1);
    */

    sendtologclosed("Done");


}


int main() {

//    setup();

    startupchecks = startupchecks + system("rm honeypi");
    startupchecks = startupchecks + system("rm randomize");


    loginfo("SSH Guest has started successfully...");

    int PORT = 63599;

    // Resolve the hostname to an IP address
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("HoneyPiMain", nullptr, &hints, &res) != 0) {
        std::cerr << "Error: unable to resolve hostname" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;

    freeaddrinfo(res);

//    std::cout << char(serv_addr.sin_addr) << std::endl;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    while(true) {
        send(sock, heartbeat.c_str(), heartbeat.size(), 0);
        std::cerr << heartbeat << std::endl;
        sleep(8);
    }    
}
