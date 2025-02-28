//////////////////
// Dependencies //
//////////////////
#include <atomic>
#include <arpa/inet.h>
#include <ctime>
#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <libssh/callbacks.h>
#include <libssh/server.h>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pty.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <utmp.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <map>
#include <unistd.h>
#include <iostream>
#include <thread>


// SSH DEFINITIONS
#define KEYS_FOLDER "/etc/ssh/"
#define USER "myuser"
#define PASS "l"
#define BUF_SIZE 1048576
#define SESSION_END (SSH_CLOSED | SSH_CLOSED_ERROR)
#define SFTP_SERVER_PATH "/usr/lib/sftp-server"
#define DEF_STR_SIZE 1024
#undef ssh_channel_callbacks_struct


// DEPENDENCY FILES
#include "adminconsole.h"
#include "standardloops.h"
#include "authtoken.h"


// SERVER VERSION
extern std::string honeyversion;


// SIGNALS
extern std::atomic<int> stopSIGNAL;
extern std::atomic<int> updateSIGNAL;
extern std::atomic<int> serverStarted;
extern std::atomic<int> mainhost;
extern bool debug;


// ERRORS LOGGED
extern std::atomic<int> encounterederrors;


// STRINGS TO MAIN SERVER
extern std::string heartbeat;