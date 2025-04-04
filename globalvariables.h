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
extern std::string endline;


// DEPENDENCY FILES
#include "adminconsole.h"
#include "standardloops.h"
#include "authtoken.h"
#include "virtualterminal.h"
#include "mainssh.h"


// SERVER VERSION
extern std::string honeyversion;


// SIGNALS
extern std::atomic<int> stopSIGNAL;
extern std::atomic<int> updateSIGNAL;
extern std::atomic<int> serverStarted;
extern std::atomic<int> mainhost;
extern bool debug;


// FIFOS
extern std::string sshfifo;
extern std::string cmdfifo;
extern std::string infifo;
extern std::string usefifo;
extern std::string pwdfifo;
extern std::string bshfifo;
extern std::string sudopassword;
extern std::string homedirfifo;


// SSH VARIABLES
extern char authorizedkeys[DEF_STR_SIZE];
extern int numberofpasswordstofake;
extern std::atomic<int> numberofpasswordstried;
extern std::atomic<int> numberofpassbackup;


// ERRORS LOGGED
extern std::atomic<int> encounterederrors;


// CHARACTER MAP
extern std::string charactermap;


// STRINGS TO MAIN SERVER
extern std::string heartbeat;