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
#define KEYS_FOLDER "/etc/ssh/"
#define USER "myuser"
#define PASS "l"
#define BUF_SIZE 1048576
#define SESSION_END (SSH_CLOSED | SSH_CLOSED_ERROR)
#define SFTP_SERVER_PATH "/usr/lib/sftp-server"
#define DEF_STR_SIZE 1024
#undef ssh_channel_callbacks_struct




////////////////////////////////
////// CONSTANT VARIABLES //////
////////////////////////////////
const bool debugmode = false;
const std::string honeyversion = "0.1.1";




////////////////////////////////////
////////////////////////////////////
//// DEPENDENCIES / DEFINITIONS ////
////////////////////////////////////
////////////////////////////////////

/*
#define ssh_channel_callbacks_struct {
    .userdata = 0,
    .channel_pty_request_function = 0,
    .channel_pty_window_change_function = 0,
    .channel_shell_request_function = 0,
    .channel_exec_request_function = 0,
    .channel_data_function = 0,
    .channel_subsystem_request_function = 0
}
*/


char authorizedkeys[DEF_STR_SIZE] = {0};


























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
std::atomic<int> encounterederrors(0);
std::atomic<int> mainhost(0);
std::atomic<int> attacked(0);
std::atomic<int> logvariableset(0);
int startupchecks = 0;


// DICTIONARY
std::map<int, char*> pingrandom = {
    {0, " google.com"},
    {1, " yahoo.com" },
    {2, " youtube.com" },
    {3, " gmail.com" },
    {4, " stackoverflow.com" },
    {5, " w3schools.com" },
    {6, " facebook.com" },
    {7, " instagram.com" },
    {8, " whatsapp.com" },
    {9, " x.com" },
    {10, " wikipedia.org"},
    {11, " yahoo.com" },
    {12, " reddit.com" },
    {13, " amazon.com" },
    {14, " baidu.com" },
    {15, " chatgpt.com" },
    {16, " chatgpt.com" },
    {17, " netflix.com" },
    {18, " netflix.com" },
    {19, " linkedin.com" },
    {20, " linkedin.com"},
    {21, " live.com" },
    {22, " live.com" },
    {23, " office.com" },
    {24, " office.com" },
    {25, " pinterest.com" },
    {26, " pinterest.com" },
    {27, " bing.com" },
    {28, " bing.com" },
    {29, " microsoftonline.com" },
    {30, " discord.com"},
    {31, " microsoft.com" },
    {32, " twitch.tv" },
    {33, " twitch.tv" },
    {34, " microsoft.com" },
    {35, " weather.com" },
    {36, " weather.com" },
    {37, " t.me" },
    {38, " roblox.com " },
    {39, " roblox.com" },
    {40, " duckduckgo.com"},
    {41, " quora.com" },
    {42, " sharepoint.com" },
    {43, " ebay.com" },
    {44, " w3schools.com" },
    {45, " facebook.com" },
    {46, " instagram.com" },
    {47, " whatsapp.com" },
    {48, " pinterest.com" },
    {49, " pinterest.com" },
    {50, " stackoverflow.com"},
    {51, " wikipedia.org" },
    {52, " live.com" },
    {53, " microsoft.com" },
    {54, " youtube.com" },
    {55, " youtube.com" },
    {56, " google.com" },
    {57, " google.com" },
    {58, " google.com" },
    {59, " google.com" }
};


// COMMUNICATION VARIABLES
std::string sshterminals[255] = {};
//std::vector<std::string> sshterminals(255, "");


struct addrinfo hints, *res;

int sock, valread;
struct sockaddr_in serv_addr;
std::string hello = "Hello from client";
char buffer[1024] = {0};


// SSH VARIABLES
std::string syntheticuser = "";
std::string syntheticpass = "";
int authenticationtries = 98;



// TIME VARIABLES
long long int startuptime = 0;
long long int currenttime = 0;
long long int timesincestartup = 0;
int currenthour = 0;
int currentminute = 0;
int currentsecond = 0;
int currentdayofyear = 0;
int currentdays = 0;
int currentyear = 0;
int currentmonth = 0;
int secondsperyear = 31536000;
int daysperyear = 365.25;
int secondsperday = 86400;
int secondsperhour = 3600;
int secondsperminute = 60;
int minutesperhour = 60;
int hoursperday = 24;

bool calculatingtime = false;




int timedetector() {
    if (calculatingtime == true) {
        std::cout << "[WARNING] - Call to Time Calculation Called While Already Processing!" << std::endl;
        return 1;

    }  else {
        // TIME
        currenttime = time(NULL);

        // CURRENT SECONDS
        timesincestartup = currenttime - startuptime;
        currentsecond = currenttime % secondsperminute;

        // CURRENT MINUTES
        currentminute = currenttime - currentsecond;
        currentminute = currentminute % 3600;
        currentminute = currentminute / 60;

        // CURRENT HOURS
        currenthour = currenttime - ((currentminute * 60) + currentsecond);
        currenthour = currenthour % hoursperday;
        
        // CURRENT DAYS
        currentdays = currenttime - ((currenthour * 3600) + (currentminute * 60) + currentsecond);
        currentdays = currentdays / 86400;

        // CURRENT YEARS
        currentyear = 1970 + (currentdays / 365.25);

        // DEBUG PRINT VALUES TO CONSOLE
        if (debug == true) {
            std::cout << currentsecond << std::endl;
            std::cout << currentminute << std::endl;
            std::cout << currenthour << std::endl;
            std::cout << currentdays << std::endl;
            std::cout << currentyear << std::endl;
        }

        return 0;
    }

    return 1;
}








////////////////////////////
// Send to Logger Scripts //
////////////////////////////
void sendtolog(std::string data2) {
    std::cout << data2 << std::endl;
}

void sendtologopen(std::string data2) {
    std::cout << data2;
}

void logdebug(std::string data2, bool complete) {
    data2 = "[DEBUG] - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void loginfo(std::string data2, bool complete) {
    data2 = "[INFO] - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logwarning(std::string data2, bool complete) {
    data2 = "[WARNING] - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logcritical(std::string data2, bool complete) {
    data2 = "[CRITICAL] - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logerror(std::string headerdata2, std::string errormessage) {
    std::string data2 = "[ERROR] - " + headerdata2 + " - " + errormessage;
    sendtolog(data2);
}



////////////////////////////////////
////////////////////////////////////
//// PROCESS IF NEW DATA SCRIPT //// 
////////////////////////////////////
////////////////////////////////////
int datawaiting() {
    logcritical(sshterminals[0], true);
    if (logvariableset == true) {
        loginfo("VARIABLE SET", true);
        attacked.store(1);
        return 1;
    } else {
        if (sshterminals[0] != "") {
            loginfo("VARIABLE SET", true);
            attacked.store(1);
            return 1;
        } else {
            loginfo("VARIABLE NOT SET", true);
            attacked.store(0);
            return 0;
        }
    }
    
}



////////////////////////////////////
////////////////////////////////////
//// READ TO MAIN THREAD SCRIPT //// 
////////////////////////////////////
////////////////////////////////////
std::string readtomainstring(int numbertoread) {
    return sshterminals[numbertoread];
}



//////////////////////////////////////
//////////////////////////////////////
//// WRITE FROM SSH THREAD SCRIPT //// 
//////////////////////////////////////
//////////////////////////////////////
int writefromsshstring(std::string stringtoinsert, char* chartoinsert) {
    bool completion22 = false;
    int testers = 0;

    if (stringtoinsert == "" && chartoinsert == "") {
        logcritical("RECEIVED EMPTY INPUT STRING, NOT CONTINUING!", true);
        return 1;
    } else {

        // HELPER TO CONVERT CHAR TO STRING
        if (chartoinsert != "") {
            stringtoinsert = chartoinsert;
            loginfo(stringtoinsert, true);
        }

        loginfo(stringtoinsert, true);

        while(completion22 == false) {
            if (testers == 254) {
                logcritical("LOG OVERFLOW ERROR!", true);
                logcritical("MARKING AS ERROR AND ENDING SESSION!", true);
                completion22 = true;
                logvariableset = true;
                return 1;
            }

            if (sshterminals[testers] == "") {
                sshterminals[testers] = stringtoinsert;
                completion22 = true;
                logvariableset = true;
                loginfo("Logged message", true);
                logwarning(sshterminals[testers], true);
                return 0;
            } else {
                testers = testers + 1;
            }
        }
    }
    return 1;
}



//////////////////////////////////////
//////////////////////////////////////
//// CLEAR SSHTERMINALS SCRIPTS   //// 
//////////////////////////////////////
//////////////////////////////////////
int clearsshterminals() {
    logvariableset = false;

    int goats = 0;
    while (goats <= 255) {
        sshterminals[goats] = "";
        goats = goats + 1;
    }

    if (sshterminals[2] != "") {
        return 1;
    } else {
        return 0;
    }
}

















//////////////////////////////////////////
// DETERMINE PUBKEY/PRIVATE KEY OPTIONS //
////////////////////////////////////////// 
static void set_default_keys(ssh_bind sshbind, int rsa_already_set, int dsa_already_set, int ecdsa_already_set) {
    if (!rsa_already_set) {
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, KEYS_FOLDER "ssh_host_rsa_key");
    }
    if (!dsa_already_set) {
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_DSAKEY, KEYS_FOLDER "ssh_host_dsa_key");
    }
    if (!ecdsa_already_set) {
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_ECDSAKEY, KEYS_FOLDER "ssh_host_ecdsa_key");
    }
}




/* A userdata struct for channel. */
struct channel_data_struct {
    /* pid of the child process the channel will spawn. */
    pid_t pid;
    /* For PTY allocation */
    socket_t pty_master;
    socket_t pty_slave;
    /* For communication with the child process. */
    socket_t child_stdin;
    socket_t child_stdout;
    /* Only used for subsystem and exec requests. */
    socket_t child_stderr;
    /* Event which is used to poll the above descriptors. */
    ssh_event event;
    /* Terminal size struct. */
    struct winsize *winsize;
};

/* A userdata struct for session. */
struct session_data_struct {
    /* Pointer to the channel the session will allocate. */
    ssh_channel channel;
    int auth_attempts;
    int authenticated;
};









// COMES THROUGH HERE?
static int data_function(ssh_session session, ssh_channel channel, void *data, uint32_t len, int is_stderr, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *) userdata;

    (void) session;
    (void) channel;
    (void) is_stderr;

    if (len == 0 || cdata->pid < 1 || kill(cdata->pid, 0) < 0) {
        return 0;
    }

    return write(cdata->child_stdin, (char *) data, len);
}




///////////////////////////////////////
///////////////////////////////////////
//// LOOPS FOR PTY TERMINAL SIZING ////
///////////////////////////////////////
///////////////////////////////////////
static int pty_request(ssh_session session, ssh_channel channel, const char *term, int cols, int rows, int py, int px, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *)userdata;

    (void) session;
    (void) channel;
    (void) term;

    cdata->winsize->ws_row = rows;
    cdata->winsize->ws_col = cols;
    cdata->winsize->ws_xpixel = px;
    cdata->winsize->ws_ypixel = py;

    loginfo("Set Size of SSH Terminal Window", true);


    if (openpty(&cdata->pty_master, &cdata->pty_slave, NULL, NULL, cdata->winsize) != 0) {
        logcritical("Failed to Open PTY!", true);
        return SSH_ERROR;
    }

    return SSH_OK;
}

static int pty_resize(ssh_session session, ssh_channel channel, int cols, int rows, int py, int px, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *)userdata;

    (void) session;
    (void) channel;

    cdata->winsize->ws_row = rows;
    cdata->winsize->ws_col = cols;
    cdata->winsize->ws_xpixel = px;
    cdata->winsize->ws_ypixel = py;

    loginfo("Resizing SSH Terminal Window", true);

    if (cdata->pty_master != -1) {
        return ioctl(cdata->pty_master, TIOCSWINSZ, cdata->winsize);
    }

    logcritical("Failed to Resize PTY!", true);
    return SSH_ERROR;
}





///////////////////////////////////////////
///////////////////////////////////////////
//// PTY TERMINAL EXECUTION SHELL LOOP //// - IMPORTANT EXECUTION LOOP
///////////////////////////////////////////
///////////////////////////////////////////
static int exec_pty(const char *mode, const char *command, struct channel_data_struct *cdata) {

    loginfo("Starting PTY SSH Terminal", true);
    switch(cdata->pid = fork()) {
        case -1:
            close(cdata->pty_master);
            close(cdata->pty_slave);
            logcritical("Failed to Fork PTY!", true);
            return SSH_ERROR;
        case 0:
            
            close(cdata->pty_master);
            if (login_tty(cdata->pty_slave) != 0) {
                exit(1);
            }







            // NEW LOOP OR NEW FILE EVEN!!!




            // HANDOFF!!!

            fprintf(stderr, "Start of new command line\n");
            fprintf(stderr, "Hello, World!\n");
            loginfo("TEST123", true);
            

            //execl("/bin/sh", "sh", mode, command, NULL);
            exit(0);
        default:
            close(cdata->pty_slave);
            /* pty fd is bi-directional */
            cdata->child_stdout = cdata->child_stdin = cdata->pty_master;
    }
    return SSH_OK;
}








/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//// PTY TERMINAL SINGLE COMMAND EXECUTION SHELL ////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
static int exec_nopty(const char *command, struct channel_data_struct *cdata) {
    int in[2], out[2], err[2];

    /* Do the plumbing to be able to talk with the child process. */
    if (pipe(in) != 0) {
        goto stdin_failed;
    }
    if (pipe(out) != 0) {
        goto stdout_failed;
    }
    if (pipe(err) != 0) {
        goto stderr_failed;
    }

    fprintf(stderr, "29\n");

    switch(cdata->pid = fork()) {
        case -1:
            goto fork_failed;
        case 0:
            /* Finish the plumbing in the child process. */
            close(in[1]);
            close(out[0]);
            close(err[0]);
            dup2(in[0], STDIN_FILENO);
            dup2(out[1], STDOUT_FILENO);
            dup2(err[1], STDERR_FILENO);
            close(in[0]);
            close(out[1]);
            close(err[1]);
            /* exec the requested command. */
            fprintf(stderr, "test\n");
            execl("/bin/sh", "sh", "-c", command, NULL);
            exit(0);
    }

    close(in[0]);
    close(out[1]);
    close(err[1]);

    cdata->child_stdin = in[1];
    cdata->child_stdout = out[0];
    cdata->child_stderr = err[0];

    return SSH_OK;

fork_failed:
    close(err[0]);
    close(err[1]);
stderr_failed:
    close(out[0]);
    close(out[1]);
stdout_failed:
    close(in[0]);
    close(in[1]);
stdin_failed:
    return SSH_ERROR;
}






/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//// PTY TERMINAL SINGLE COMMAND EXECUTION SHELL ////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
static int exec_request(ssh_session session, ssh_channel channel, const char *command, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *) userdata;

    fprintf(stderr, "32\n");

    (void) session;
    (void) channel;

    if(cdata->pid > 0) {
        return SSH_ERROR;
    }

    if (cdata->pty_master != -1 && cdata->pty_slave != -1) {
        return exec_pty("-c", command, cdata);
    }
    return exec_nopty(command, cdata);
}






/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//// SHELL REQUEST START SHELL EXECUTION LOOP ///////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
static int shell_request(ssh_session session, ssh_channel channel, void *userdata) {
    struct channel_data_struct *cdata = (struct channel_data_struct *) userdata;

    (void) session;
    (void) channel;

    fprintf(stderr, "72\n");

    if(cdata->pid > 0) {
        return SSH_ERROR;
    }

    if (cdata->pty_master != -1 && cdata->pty_slave != -1) {
        return exec_pty("-l", NULL, cdata);
    }
    /* Client requested a shell without a pty, let's pretend we allow that */
    return SSH_OK;
}







///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//// SUBSYSTEM REQUESTED LOOP (NOT CURRENTLY SUPPORTED) ///////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
static int subsystem_request(ssh_session session, ssh_channel channel, const char *subsystem, void *userdata) {
    /* subsystem requests behave simillarly to exec requests. */
    if (strcmp(subsystem, "sftp") == 0) {
        return exec_request(session, channel, SFTP_SERVER_PATH, userdata);
    }
    return SSH_ERROR;
}






//////////////////////////////////////////////////
//////////////////////////////////////////////////
//// AUTHORIZE SSH THROUGH PASSWORD METHOD ///////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
static int auth_password(ssh_session session, const char *user, const char *pass, void *userdata) {
    struct session_data_struct *sdata = (struct session_data_struct *) userdata;

    (void) session;

    int binary = 0;

    // TRUE PASSWORD CONFIGURATION
    if (debugmode == true) {
        if (strcmp(user, USER) == 0 && strcmp(pass, PASS) == 0) {
            sdata->authenticated = 1;
            return SSH_AUTH_SUCCESS;
        }
    } else {


        // WASTE Protocol

        // FIX THIS - ADD RANDOM NUMBER TO AUTHENTICATIONTRIES
        loginfo("Waste 1.0.0", true);
        if (sdata->auth_attempts >= authenticationtries) {
            char* usernamet = "US: ";
            char* passwordt = "PW: ";
            strcat(usernamet, user);
            strcat(passwordt, pass);
            syntheticuser = user;
            syntheticpass = pass;
            loginfo("check", true);
            binary = writefromsshstring("", usernamet);
            binary = binary + writefromsshstring("", passwordt);

            if (binary != 0) {
                logcritical("Success error occurred!", true);
            } else {
                sdata->authenticated = 1;
                return SSH_AUTH_SUCCESS;
            }
        } else {
            char* usernamet = "US: ";
            char* passwordt = "PW: ";
            strcat(usernamet, user);
            strcat(passwordt, pass);
            binary = writefromsshstring("", usernamet);
            binary = binary + writefromsshstring("", passwordt);
        }

    }

    if (binary != 0) {
        logcritical("an error occurred saving information!", true);
        sdata->auth_attempts++;
        return SSH_AUTH_DENIED;
    }

    sdata->auth_attempts++;
    return SSH_AUTH_DENIED;
}







////////////////////////////////////////////////////
////////////////////////////////////////////////////
//// AUTHORIZE SSH THROUGH PUBLIC KEY METHOD ///////
////////////////////////////////////////////////////
////////////////////////////////////////////////////
static int auth_publickey(ssh_session session, const char *user, struct ssh_key_struct *pubkey, char signature_state, void *userdata) {
    struct session_data_struct *sdata = (struct session_data_struct *) userdata;

    (void) user;
    (void) session;

    if (signature_state == SSH_PUBLICKEY_STATE_NONE) {
        return SSH_AUTH_SUCCESS;
    }

    if (signature_state != SSH_PUBLICKEY_STATE_VALID) {
        return SSH_AUTH_DENIED;
    }

    // valid so far.  Now look through authorized keys for a match
    if (authorizedkeys[0]) {
        ssh_key key = NULL;
        int result;
        struct stat buf;

        if (stat(authorizedkeys, &buf) == 0) {
            result = ssh_pki_import_pubkey_file( authorizedkeys, &key );
            if ((result != SSH_OK) || (key==NULL)) {
                fprintf(stderr,
                        "Unable to import public key file %s\n",
                        authorizedkeys);
            } else {
                result = ssh_key_cmp( key, pubkey, SSH_KEY_CMP_PUBLIC );
                ssh_key_free(key);
                if (result == 0) {
                    sdata->authenticated = 1;
                    return SSH_AUTH_SUCCESS;
                }
            }
        }
    }

    // no matches
    sdata->authenticated = 0;
    return SSH_AUTH_DENIED;
}








/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//// OPEN SSH CHANNELS UPON RECEIVING CONNECTION ////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
static ssh_channel channel_open(ssh_session session, void *userdata) {
    struct session_data_struct *sdata = (struct session_data_struct *) userdata;

    sdata->channel = ssh_channel_new(session);
    return sdata->channel;
}








//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//// SEND UPDATED INFORMATION BACK ON SSH TUNNEL (STDOUT) ////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
static int process_stdout(socket_t fd, int revents, void *userdata) {
    char buf[BUF_SIZE];
    int n = -1;
    
    ssh_channel channel = (ssh_channel) userdata;

    if (channel != NULL && (revents & POLLIN) != 0) {
        n = read(fd, buf, BUF_SIZE);
        if (n > 0) {
            fprintf(stderr, buf);
            ssh_channel_write(channel, buf, n);
            memset(buf, 0, BUF_SIZE);
        }
    }

    return n;
}








//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//// SEND UPDATED INFORMATION BACK ON SSH TUNNEL (STDERR) ////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
static int process_stderr(socket_t fd, int revents, void *userdata) {
    char buf[BUF_SIZE];
    int n = -1;
    ssh_channel channel = (ssh_channel) userdata;

    if (channel != NULL && (revents & POLLIN) != 0) {
        n = read(fd, buf, BUF_SIZE);
        if (n > 0) {
            ssh_channel_write_stderr(channel, buf, n);
        }
    }

    return n;
}








static void handle_session(ssh_event event, ssh_session session) {
    int n;
    int rc = 0;

    /* Structure for storing the pty size. */
    struct winsize wsize = {
        .ws_row = 0,
        .ws_col = 0,
        .ws_xpixel = 0,
        .ws_ypixel = 0
    };

    /* Our struct holding information about the channel. */
    struct channel_data_struct cdata = {
        .pid = 0,
        .pty_master = -1,
        .pty_slave = -1,
        .child_stdin = -1,
        .child_stdout = -1,
        .child_stderr = -1,
        .event = NULL,
        .winsize = &wsize
    };

    /* Our struct holding information about the session. */
    struct session_data_struct sdata = {
        .channel = NULL,
        .auth_attempts = 0,
        .authenticated = 0
    };

     
    struct ssh_channel_callbacks_struct channel_cb = {
        // REORDERED STRUCT
        .userdata = &cdata,
        .channel_data_function = data_function,
        .channel_pty_request_function = pty_request,
        .channel_shell_request_function = shell_request,
        .channel_pty_window_change_function = pty_resize,
        .channel_exec_request_function = exec_request,
        .channel_subsystem_request_function = subsystem_request

    };

    struct ssh_server_callbacks_struct server_cb = {
        .userdata = &sdata,
        .auth_password_function = auth_password,
        .channel_open_request_session_function = channel_open,
    };

    if (authorizedkeys[0]) {
        server_cb.auth_pubkey_function = auth_publickey;
        ssh_set_auth_methods(session, SSH_AUTH_METHOD_PASSWORD | SSH_AUTH_METHOD_PUBLICKEY);
    } else
        ssh_set_auth_methods(session, SSH_AUTH_METHOD_PASSWORD);

    ssh_callbacks_init(&server_cb);
    ssh_callbacks_init(&channel_cb);

    ssh_set_server_callbacks(session, &server_cb);


    if (ssh_handle_key_exchange(session) != SSH_OK) {
        fprintf(stderr, "%s\n", ssh_get_error(session));
        return;
    }

    ssh_event_add_session(event, session);

    n = 0;


    // START SSH CONNECTION
    logwarning("SSH Connection Received...", true);

    if (writefromsshstring("newconnectionreceived", "") != 0) {
        logcritical("UNABLE TO UPDATE MAIN VARIABLES!", true);
        logcritical("Killing...", true);
        return;
        return;
    }

    if (debugmode == true) {
        logwarning("SSH Starting in DEBUGGING MODE!!!", true);
        logwarning("THIS SHOULD NOT BE FOR PRODUCTION!", true);

        // MAIN AUTHENTICATION LOOP
        while (sdata.authenticated == 0 || sdata.channel == NULL) {
        /* If the user has used up all attempts, or if he hasn't been able to
         * authenticate in 100 seconds (n * 100ms), disconnect. */

            if (sdata.auth_attempts >= 3 || n >= 100) {
                return;
            }

            if (ssh_event_dopoll(event, 100) == SSH_ERROR) {
                fprintf(stderr, "%s\n", ssh_get_error(session));
                return;
            }
            
            n++;
        }
    } else {
        loginfo("Using WASTE authentication", true);

        // MAIN AUTHENTICATION LOOP
        while (sdata.authenticated == 0 || sdata.channel == NULL) {
            /* If the user has used up all attempts, or if he hasn't been able to
            * authenticate in 100 seconds (n * 100ms), disconnect. */

            if (debugmode == true) {
                if (sdata.auth_attempts >= 98 || n >= 1000) {
                    return;
                }

                if (ssh_event_dopoll(event, 100) == SSH_ERROR) {
                    fprintf(stderr, "%s\n", ssh_get_error(session));
                    return;
                }

            } else {
                if (sdata.auth_attempts >= 98 || n >= 1000) {
                    return;
                }

                if (ssh_event_dopoll(event, 100) == SSH_ERROR) {
                    fprintf(stderr, "%s\n", ssh_get_error(session));
                    return;
                }
            }
            n++;
        }
    }

    

    // AUTHENTICATE UP
    // RUN SESSION BELOW?
    
    logwarning("SSH Session Starting...", true);

    ssh_set_channel_callbacks(sdata.channel, &channel_cb);

    do {
        /* Poll the main event which takes care of the session, the channel and
         * even our child process's stdout/stderr (once it's started). */
        if (ssh_event_dopoll(event, -1) == SSH_ERROR) {
            fprintf(stderr, "reached here23\n");
          ssh_channel_close(sdata.channel);
        }

        /* If child process's stdout/stderr has been registered with the event,
         * or the child process hasn't started yet, continue. */
        if (cdata.event != NULL || cdata.pid == 0) {
            continue;
            
            fprintf(stderr, "reached here2\n");
        }
        /* Executed only once, once the child process starts. */







        cdata.event = event; // DECLARATION?







        // POLLING FOR COMMAND/RUNNING COMMAND?
        /* If stdout valid, add stdout to be monitored by the poll event. */
        if (cdata.child_stdout != -1) {
            if (ssh_event_add_fd(event, cdata.child_stdout, POLLIN, process_stdout,
                                 sdata.channel) != SSH_OK) {
                fprintf(stderr, "Failed to register stdout to poll context\n");
                ssh_channel_close(sdata.channel);
            }
        }



        
        // NOT NEEDED IN CONTEXT?
        /* If stderr valid, add stderr to be monitored by the poll event. 
        if (cdata.child_stderr != -1){
            if (ssh_event_add_fd(event, cdata.child_stderr, POLLIN, process_stderr,
                                 sdata.channel) != SSH_OK) {
                fprintf(stderr, "Failed to register stderr to poll context\n");
                ssh_channel_close(sdata.channel);
            }
        }
        */
        fprintf(stderr, "reached here3\n");
    } 
    
    
    // WAIT STATE
    while(ssh_channel_is_open(sdata.channel) && (cdata.pid == 0 || waitpid(cdata.pid, &rc, WNOHANG) == 0));

    close(cdata.pty_master);
    close(cdata.child_stdin);
    close(cdata.child_stdout);
    close(cdata.child_stderr);

    /* Remove the descriptors from the polling context, since they are now
     * closed, they will always trigger during the poll calls. */
    ssh_event_remove_fd(event, cdata.child_stdout);
    ssh_event_remove_fd(event, cdata.child_stderr);

    /* If the child process exited. */
    
    fprintf(stderr, "reached here4\n");
    if (kill(cdata.pid, 0) < 0 && WIFEXITED(rc)) {
        rc = WEXITSTATUS(rc);
        ssh_channel_request_send_exit_status(sdata.channel, rc);
    /* If client terminated the channel or the process did not exit nicely,
     * but only if something has been forked. */
    } else if (cdata.pid > 0) {
        kill(cdata.pid, SIGKILL);
    }

    ssh_channel_send_eof(sdata.channel);
    ssh_channel_close(sdata.channel);

    /* Wait up to 5 seconds for the client to terminate the session. */
    for (n = 0; n < 50 && (ssh_get_status(session) & SESSION_END) == 0; n++) {
        ssh_event_dopoll(event, 100);
    }
}






/* SIGCHLD handler for cleaning up dead children. */
static void sigchld_handler(int signo) {
    (void) signo;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}






























int handleSSHConnections (int argc, char **argv) {
    ssh_bind sshbind;
    ssh_session session;
    ssh_event event;
    struct sigaction sa;
    int rc;

    /* Set up SIGCHLD handler. */
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) != 0) {
        fprintf(stderr, "Failed to register SIGCHLD handler\n");
        return 1;
    }

    rc = ssh_init();
    if (rc < 0) {
        fprintf(stderr, "ssh_init failed\n");
        return 1;
    }

    sshbind = ssh_bind_new();
    if (sshbind == NULL) {
        fprintf(stderr, "ssh_bind_new failed\n");
        return 1;
    }

#ifdef HAVE_ARGP_H
    argp_parse(&argp, argc, argv, 0, 0, sshbind);
#else
    (void) argc;
    (void) argv;

    set_default_keys(sshbind, 0, 0, 0);
#endif /* HAVE_ARGP_H */

    if(ssh_bind_listen(sshbind) < 0) {
        fprintf(stderr, "%s\n", ssh_get_error(sshbind));
        return 1;
    }

    while (1) {
        session = ssh_new();
        if (session == NULL) {
            fprintf(stderr, "Failed to allocate session\n");
            continue;
        }

        /* Blocks until there is a new incoming connection. */
        if(ssh_bind_accept(sshbind, session) != SSH_ERROR) {
            switch(fork()) {
                case 0:
                    /* Remove the SIGCHLD handler inherited from parent. */
                    sa.sa_handler = SIG_DFL;
                    sigaction(SIGCHLD, &sa, NULL);
                    /* Remove socket binding, which allows us to restart the
                     * parent process, without terminating existing sessions. */
                    ssh_bind_free(sshbind);

                    event = ssh_event_new();
                    if (event != NULL) {
                        /* Blocks until the SSH session ends by either
                         * child process exiting, or client disconnecting. */
                        handle_session(event, session);
                        ssh_event_free(event);
                    } else {
                        fprintf(stderr, "Could not create polling context\n");
                    }
                    ssh_disconnect(session);
                    ssh_free(session);

                    exit(0);
                case -1:
                    fprintf(stderr, "Failed to fork\n");
            }
        } else {
            fprintf(stderr, "%s\n", ssh_get_error(sshbind));
        }
        /* Since the session has been passed to a child fork, do some cleaning
         * up at the parent process. */
        ssh_disconnect(session);
        ssh_free(session);
    }

    ssh_bind_free(sshbind);
    ssh_finalize();
    return 0;
}























/*
void thread1(std::promise<void>&& promise) {
    std::unique_lock<std::mutex> lock(mtx);
    while (!ready) {
        cv.wait(lock);
    }
    std::cout << "Thread 1 received data: " << shared_data << sshterminals[0] << std::endl;

    shared_data++;
    ready = false;
    cv.notify_all();
    // Signal that thread1 has finished its work
    promise.set_value();
}

void thread2(std::promise<void>&& promise) {
    std::unique_lock<std::mutex> lock(mtx);
    shared_data = 42;
    sshterminals[0] = "TESTING123";
    ready = true;
    std::cout << "Thread 2 sent data: " << shared_data << sshterminals[0] << std::endl;
    cv.notify_all();
    while (ready) {
        cv.wait(lock);
    }
    std::cout << "Thread 2 received updated data: " << shared_data << std::endl;
    // Signal that thread2 has finished its work
    promise.set_value();
}
*/





int ping() {
    int testnumber = 0;
    testnumber = int((rand() % 60));
    int result;
    char buffer51[512];
    const char* starter3 = "ping -c 5 ";
    const char* ender3 = " > nul: ";
    strcpy(buffer51, starter3);
    char* pingtest = "";
    pingtest = pingrandom[testnumber];
    strcat(buffer51, pingtest);
    strcat(buffer51, ender3);
    result = system(buffer51);
    if (result != 0) {
        logwarning("UNABLE TO PING WEBSITE!", true);
    } else {
        if (debug == true) {
            loginfo("FINISHED PING", true);
        }
    }
    
    return result;
}














// PLEASE WORK


void mainrunningloop() {

    int testing234 = ping();

    startupchecks = startupchecks + system("rm honeypi");
    startupchecks = startupchecks + system("rm randomize");


    loginfo("SSH Guest has started successfully...", true);


    
    // MAIN THREAD CONSTANT LOOP
    while(true) {

        sleep(2);
        send(sock, heartbeat.c_str(), heartbeat.size(), 0);


        if (attacked == true) {
            

            // IF DATA WAITING, ANALYZE THE DATA
            if (datawaiting() == 1) {

                loginfo("ANALYZING DATA", true);

                // ANALYZE DATA RECEIVED
                bool completiongh = false;
                int learned = 0;
                while(learned <= 255 && completiongh == false) {
                    std::string datareceived = readtomainstring(learned);

                    if (datareceived == "") {
                        loginfo("Analyzed all available data, clearing array", true);
                        logvariableset = false;
                        clearsshterminals();
                        completiongh = true;
                    } else {

                        // OTHER COMMANDS HERE
                        if (datareceived == "newconnectionreceived") {
                            attacked = true;
                            std::string attacksend = "attacked";
                            send(sock, attacksend.c_str(), attacksend.size(), 0);
                        } else {

                            // TO DECLARE VARIABLES
                            std::string subsystempart = datareceived.substr(0, 2);
                            std::string restofmessage = datareceived.substr(4, datareceived.size() - 4);

                            bool validmessage = false;

                            // USERNAME
                            if (subsystempart == "US") {
                                char* rainbows = "USE: ";
                                strcat(rainbows, restofmessage.c_str());

                                send(sock, rainbows, strlen(rainbows), 0);
                                validmessage = true;
                            }

                            // IP ADDRESS
                            if (subsystempart == "IP") {

                            }

                            // PASSWORDS
                            if (subsystempart == "PW") {

                            }

                            // COMMANDS RAN
                            if (subsystempart == "CM") {

                            }

                            // PUBLIC KEYS
                            if (subsystempart == "PK") {

                            }

                            // PRIVATE KEYS
                            if (subsystempart == "PI") {

                            }

                            // FILES ACCESSED
                            if (subsystempart == "FA") {

                            }
                            
                            // FILE CHANGES
                            if (subsystempart == "FC") {

                            }




                            // IF VALID MESSAGE EQUALS FALSE!
                            if (validmessage == false) {
                                logcritical("INVALID MESSAGE RECEIVED!", true);
                                std::string invalidmessage = "invalid";
                                send(sock, invalidmessage.c_str(), invalidmessage.size(), 0);
                            }

                        }
                    }


                    learned = learned + 1;
                }

            } else {
                sleep(0.25);
            }
        } else {
        
            if (datawaiting() == 1) {


                // ANALYZE DATA RECEIVED
                loginfo("ANALYZING", true);
                bool completiongh = false;
                int learned = 0;
                while(learned <= 255 && completiongh == false) {
                    std::string datareceived = readtomainstring(learned);
                    logwarning(datareceived, true);

                    if (datareceived == "") {
                        loginfo("Analyzed all available data, clearing array", true);
                        logvariableset = false;
                        clearsshterminals();
                        completiongh = true;
                    } else {

                        // OTHER COMMANDS HERE
                        if (datareceived == "newconnectionreceived") {
                            attacked = true;
                            logcritical("RECEIVED ATTACK FROM SSH, LOGGING INFORMATION", true);
                            std::string attacksend = "attacked";
                            send(sock, attacksend.c_str(), attacksend.size(), 0);
                        } else {

                            // TO DECLARE VARIABLES
                            std::string subsystempart = datareceived.substr(0, 2);
                            std::string restofmessage = datareceived.substr(4, datareceived.size() - 4);

                            bool validmessage = false;

                            // USERNAME
                            if (subsystempart == "US") {
                                char* rainbows = "USE: ";
                                strcat(rainbows, restofmessage.c_str());

                                send(sock, rainbows, strlen(rainbows), 0);
                                validmessage = true;
                            }

                            // IP ADDRESS
                            if (subsystempart == "IP") {

                            }

                            // PASSWORDS
                            if (subsystempart == "PW") {

                            }

                            // COMMANDS RAN
                            if (subsystempart == "CM") {

                            }

                            // PUBLIC KEYS
                            if (subsystempart == "PK") {

                            }

                            // PRIVATE KEYS
                            if (subsystempart == "PI") {

                            }

                            // FILES ACCESSED
                            if (subsystempart == "FA") {

                            }
                            
                            // FILE CHANGES
                            if (subsystempart == "FC") {

                            }




                            // IF VALID MESSAGE EQUALS FALSE!
                            if (validmessage == false) {
                                logcritical("INVALID MESSAGE RECEIVED!", true);
                                std::string invalidmessage = "invalid";
                                send(sock, invalidmessage.c_str(), invalidmessage.size(), 0);
                            }

                        }
                    }


                    learned = learned + 1;
                }




                sleep(0.25);
            } else {
                sleep(8);
                loginfo("heartbeatSSH", true);
            }
        }

    }    

}







///////////////////////////
//// MAIN SETUP SCRIPT ////
///////////////////////////
int setup(int argc, char **argv) {
    sendtolog("Hello, World from 2515!");
    sendtolog("  _____     _____     ____________      _____      ____  ________________   ____         ____           ______________     ________________  ");
    sendtolog("  |   |     |   |    /            `     |   `      |  |  |               |  `  `        /   /           |             `   |               |  ");
    sendtolog("  |   |     |   |   /              `    |    `     |  |  |  |¯¯¯¯¯¯¯¯¯¯¯¯    `  `      /   /            |   |¯¯¯¯¯¯`   |  |_____    ______|  ");
    sendtolog("  |   |     |   |  /   /¯¯¯¯¯¯¯¯`   `   |     `    |  |  |  |____________     `  `    /   /             |   |______/   |        |   |        ");
    sendtolog("  |    ¯¯¯¯¯    |  |   |         |   |  |      `   |  |  |               |     `  `  /   /              |   __________/         |   |        ");
    sendtolog("  |    _____    |  |   |         |   |  |   |`  `  |  |  |               |      `  `/   /               |   |                   |   |        ");
    sendtolog("  |   |     |   |  |   |         |   |  |   | `  ` |  |  |  |¯¯¯¯¯¯¯¯¯¯¯¯        |     |                |   |                   |   |        ");
    sendtolog("  |   |     |   |  |   |         |   |  |   |  `  `|  |  |  |____________        |     |                |   |                   |   |        ");
    sendtolog("  |   |     |   |  `   `¯¯¯¯¯¯¯¯¯    /  |   |   `     |  |               |       |     |                |   |             |¯¯¯¯¯     ¯¯¯¯¯|  ");
    sendtolog("  |   |     |   |   `               /   |   |    `    |  |               |       |     |                |   |             |               |  ");
    sendtolog("  ¯¯¯¯¯     ¯¯¯¯¯    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯    ¯¯¯¯      `¯¯¯   ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯        ¯¯¯¯¯¯                 ¯¯¯¯¯             ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯   ");
    sendtolog("GUEST SSH!");
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("Program by Matthew Whitworth (MawWebby)");
    sendtolog("Version " + honeyversion);
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("");

    sendtolog("STARTING");
    
    // DELAY FOR SYSTEM TO START FURTHER
    sleep(1.5);
    if (debug == true) {
        int testing = system("./debug");
    } else {
        int testing = system("rm debug");
    }
    


    // CHECK FOR SYSTEM UPDATES
    loginfo("Checking for Updates...", false);
    int returnedvalue = system("apt-get update > nul:");
    if (returnedvalue == 0) {
        sendtolog("Done");
    } else {
        sendtolog("ERROR");
        logcritical("UNABLE TO CHECK FOR SYSTEM UPDATES!", true);
        logcritical("This could be potentially dangerous!", true);
        logcritical("KILLING PROCESS!", true);
        startupchecks = startupchecks + 1;
        return 1;
        return 1;
        return 1;
    }



    // CHECK FOR SYSTEM UPDATES
    loginfo("Updating System...", false);
    int returnedvalue2 = system("apt-get update > nul:");
    if (returnedvalue2 == 0) {
        sendtolog("Done");
    } else {
        sendtolog("ERROR");
        logcritical("UNABLE TO UPGRADE SYSTEM!", true);
        logcritical("This could be potentially dangerous!", true);
        logcritical("KILLING PROCESS!", true);
        startupchecks = startupchecks + 1;
        return 1;
        return 1;
        return 1;
    }

    // RANDOMIZING SYSTEM (PROBABLY HERE) (FIX THIS) (ADD SEPARATE THREAD FOR IT!)
    //random


    loginfo("finishing SSH Guest V1 startup...", true);

    std::fstream rsakeys;
    rsakeys.open("/etc/ssh/ssh_host_rsa_key");
    if (rsakeys.is_open() == true) {
        rsakeys.close();
        loginfo("SSH RSA Keys Found!", true);        
    } else {
        loginfo("SSH RSA Keys Not Found!", true);
        loginfo("Generating New Keys", true);
        int generatekeys = system("ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N '' > nul: && ssh-keygen -t ecdsa -f /etc/ssh/ssh_host_ecdsa_key -N '' > nul: && ssh-keygen -t ed25519 -f /etc/ssh/ssh_host_ed25519_key -N '' > nul: ");
        loginfo("Done Generating SSH Keys", true);
    }

    loginfo("Removing Unneeded Dependencies!", true);
    int removepackage = system("apt-get remove openssh-server openssh-client -y > nul:");



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
            logcritical("Unable to resolve hostname!", true);
            if (debug == true) {
                loginfo("Not killing in debug mode", true);
                mainhost = false;
            } else {
                logcritical("Killing docker container", true);
                encounterederrors = encounterederrors + 1;
                mainhost = false;
                return 1;
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
                logcritical("Socket creation error!", true);
                if (debug == true) {
                    loginfo("Not killing in debug mode", true);
                } else {
                    logcritical("Killing docker container", true);
                    encounterederrors = encounterederrors + 1;
                    return 1;
                }
            }

            // Connect to the server
            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                logcritical("Connection failed!", true);
                if (debug == true) {
                    loginfo("Not killing in debug mode", true);
                } else {
                    logcritical("Killing docker container", true);
                    encounterederrors = encounterederrors + 1;
                    return 1;
                }
            }
        }
    } else {
        logwarning("Ignoring Connection to Main Docker Container", true);
    }

    //////////////////////////////////
    // START HOSTING ON SSH PORT 22 //
    //////////////////////////////////

    // SERVER PORT LISTEN THREAD(22)
    sendtologopen("[INFO] - Creating server thread on port 22 listen...");

    sleep(1);
    std::thread acceptingClientsThread2(handleSSHConnections, argc, argv);
    acceptingClientsThread2.detach();
    sleep(1);

    std::thread mainRunningLoop(mainrunningloop);
    mainRunningLoop.detach();

    return 0;
}







/////////////////////////////
//// MAIN LOOPING SCRIPT ////
/////////////////////////////
int main(int argc, char **argv) {

    setup(argc, argv);

    while(true) {
        sleep(100);
    }

    return 255;
}
