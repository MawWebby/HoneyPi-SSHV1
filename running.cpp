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
#include <libssh/libssh.h>
#include <libssh/server.h>



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




/////////////////////////////
//// SSH CONTROL SCRIPTS ////
/////////////////////////////
int authenticate(ssh_session session) {
    int PORT = 22;
    ssh_message message;
    int auth = 0;

    do {
        message = ssh_message_get(session);
        if (!message) {
            break;
        }

        if (ssh_message_type(message) == SSH_REQUEST_AUTH &&
            ssh_message_subtype(message) == SSH_AUTH_METHOD_PASSWORD) {

            const char* username = ssh_message_auth_user(message);
            const char* password = ssh_message_auth_password(message);

            // For simplicity, use a hardcoded username and password
            if (std::string(username) == "user" && std::string(password) == "password") {
                auth = 1;
                ssh_message_auth_reply_success(message, 0);
            } else {
                ssh_message_auth_set_methods(message, SSH_AUTH_METHOD_PASSWORD);
                ssh_message_reply_default(message);
            }

            ssh_message_free(message);
        } else {
            ssh_message_reply_default(message);
            ssh_message_free(message);
        }

    } while (!auth);

    return auth;
}

int handleSSHConnections() { 
    int PORT = 22;
    int sshchecks = 0;

    ssh_bind sshbind = ssh_bind_new();
    ssh_session session = ssh_new();
    int rc;

    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, &PORT);
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, "/etc/ssh/ssh_host_rsa_key");

    rc = ssh_bind_listen(sshbind);
    if (rc < 0) {
        logcritical("Error listening to socket");
        logcritical(ssh_get_error(sshbind));
        logcritical("Dropping connection");
        sshchecks = sshchecks + 1;
    }

    rc = ssh_bind_accept(sshbind, session);
    if (rc == SSH_ERROR) {
        std::cerr << "Error accepting connection: " << ssh_get_error(sshbind) << std::endl;
        sshchecks = sshchecks + 1;
    }


    if (ssh_handle_key_exchange(session)) {
        std::cerr << "Error handling key exchange: " << ssh_get_error(session) << std::endl;
        sshchecks = sshchecks + 1;
    }


    if (!authenticate(session)) {
        std::cerr << "Authentication failed" << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        sshchecks = sshchecks + 1;
    }
    
    sleep(1);

    
    ssh_channel chan = ssh_channel_new(session);
    
    if(chan == NULL) {
        logcritical("SSH Channel started NULL!");
    } else {
        loginfo("SSH Channel Started Successfully");




        // FUTURE HOME OF ATTACK COMMAND?






    }

    sleep(1);


    int sshstartsession = ssh_channel_open_session(chan);
    std::cerr << sshstartsession << std::endl;

    sleep(1);

    if (sshstartsession != SSH_ERROR) {
        std::cerr << "Error opening channel" << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        sshchecks = sshchecks + 1;
        std::cerr << ssh_get_error(chan) << std::endl;
    } else {
        loginfo("Channel started successfully");
    }
    
    sleep(1);



 //   int status = ssh_channel_request_shell(chan);

//    if (status != SSH_OK) {
 //       logcritical("UNABLE TO START INTERACTIVE TERMINAL");
  //      sshchecks = sshchecks + 1;
   // }


    //

    if (sshchecks != 0) {
        logcritical("Something terrible has happened to SSH Server!");
        logcritical("Dropping SSH Connection and attempting to restart");
        logcritical(ssh_get_error(chan));
        std::cerr << ssh_get_error(chan) << std::endl;
    } else {

        

        char buffer23[1024];


// ssh_channel_is_open(chan) && !ssh_channel_is_eof(chan)
        while (true) {

            int nbytes = ssh_channel_read(chan, buffer23, sizeof(buffer23), 0);
            if (nbytes < 0)
            return SSH_ERROR;
        
            if (nbytes > 0)
            write(1, buffer23, nbytes);
            loginfo(buffer23);
        }
    }


    

    ssh_channel_send_eof(chan);
    ssh_channel_close(chan);
    ssh_channel_free(chan);

    ssh_disconnect(session);
    ssh_free(session);
    ssh_bind_free(sshbind);
    ssh_finalize();

 //   sleep(10);
    sendtolog("worked");
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
