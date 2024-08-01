//////////////////
// Dependencies //
//////////////////
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <fstream>
#include <thread>
#include <ctime>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <atomic>
#include <cstring>
#include <chrono>



///////////////////
//// VARIABLES ////
///////////////////
// CONSTANT STRINGS
std::string heartbeat = "heartbeatSSH";
std::string attacksend = "attacked";

// CONSTANT DEBUGGING SCRIPTS
const bool debug = true;
const bool runtomain = true;

// SYSTEM VARIABLES
int encounterederrors = 0;
int startupchecks = 0;
bool mainhost = true;


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



int handleSSHConnections () {
    int status = system("./ssh");
}




///////////////////////////
//// MAIN SETUP SCRIPT ////
///////////////////////////
void setup() {
    loginfo("finishing SSH Guest V1 startup...");

    //////////////////////////////////////////////////////
    // START CLIENT CONNECTION TO MAIN HONEYPI ON 63599 //
    //////////////////////////////////////////////////////

    int PORT = 63599;

    if (runtomain == true) {
        // Resolve the hostname to an IP address
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo("HoneyPiMain", nullptr, &hints, &res) != 0) {
            logcritical("Unable to resolve hostname!");
            if (debug == true) {
                loginfo("Not killing in debug mode");
                mainhost = false;
            } else {
                logcritical("Killing docker container");
                encounterederrors = encounterederrors + 1;
                mainhost = false;
                return;
            }
        }

        if (mainhost == true) {

            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(PORT);
            serv_addr.sin_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;

            freeaddrinfo(res);

            //    std::cout << char(serv_addr.sin_addr) << std::endl;

            // Create socket
            if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                logcritical("Socket creation error!");
                if (debug == true) {
                    loginfo("Not killing in debug mode");
                } else {
                    logcritical("Killing docker container");
                    encounterederrors = encounterederrors + 1;
                    return;
                }
            }

            // Connect to the server
            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                logcritical("Connection failed!");
                if (debug == true) {
                    loginfo("Not killing in debug mode");
                } else {
                    logcritical("Killing docker container");
                    encounterederrors = encounterederrors + 1;
                    return;
                }
            }
        }
    } else {
        logwarning("Ignoring Connection to Main Docker Container");
    }

    //////////////////////////////////
    // START HOSTING ON SSH PORT 22 //
    //////////////////////////////////

    // SERVER PORT LISTEN THREAD(22)
    sendtologopen("[INFO] - Creating server thread on port 22 listen...");

    sleep(2);
    std::thread acceptingClientsThread2(handleSSHConnections);
    acceptingClientsThread2.detach();
    sleep(1);




}





/////////////////////////////
//// MAIN LOOPING SCRIPT ////
/////////////////////////////
int main() {

    setup();

    startupchecks = startupchecks + system("rm honeypi");
    startupchecks = startupchecks + system("rm randomize");


    loginfo("SSH Guest has started successfully...");

    

    while(true) {
        if (runtomain == true && mainhost == true) {
            send(sock, heartbeat.c_str(), heartbeat.size(), 0);
        }
        sleep(8);
        loginfo("heartbeatSSH");
    }    
}
