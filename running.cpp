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

int sock = 0, valread;
struct sockaddr_in serv_addr;
struct addrinfo hints, *res;
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




int setup() {
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
}


int main() {

    setup();

    loginfo("SSH Guest has started successfully...");

    

    while(true) {
        send(sock, heartbeat.c_str(), hello.size(), 0);
//        std::cerr << heartbeat << std::endl;
        sleep(8);
    }    
}
