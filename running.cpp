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

std::string heartbeat = "heartbeatSSH";
std::string attacksend = "attacked";

const bool debug = true;
const bool runtomain = true;

int encounterederrors = 0;

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

    ssh_bind sshbind = ssh_bind_new();
    ssh_session session = ssh_new();
    int rc;

    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, &PORT);
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, "/etc/ssh/ssh_host_rsa_key");

    rc = ssh_bind_listen(sshbind);
    if (rc < 0) {
        std::cerr << "Error listening to socket: " << ssh_get_error(sshbind) << std::endl;
        return 1;
    }

    rc = ssh_bind_accept(sshbind, session);
    if (rc == SSH_ERROR) {
        std::cerr << "Error accepting connection: " << ssh_get_error(sshbind) << std::endl;
        return 1;
    }

    if (ssh_handle_key_exchange(session)) {
        std::cerr << "Error handling key exchange: " << ssh_get_error(session) << std::endl;
        return 1;
    }

    if (!authenticate(session)) {
        std::cerr << "Authentication failed" << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    ssh_channel chan = ssh_channel_new(session);
    if (ssh_channel_open_session(chan)) {
        std::cerr << "Error opening channel" << std::endl;
        return 1;
    }

    ssh_channel_request_shell(chan);

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
            } else {
                logcritical("Killing docker container");
                encounterederrors = encounterederrors + 1;
                return;
            }
        }

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

int main() {

    setup();

    startupchecks = startupchecks + system("rm honeypi");
    startupchecks = startupchecks + system("rm randomize");


    loginfo("SSH Guest has started successfully...");

    

    while(true) {
        if (runtomain == true) {
            send(sock, heartbeat.c_str(), heartbeat.size(), 0);
            std::cerr << heartbeat << std::endl;
        }
        sleep(8);
        loginfo("heartbeatSSH");
    }    
}
