
#include "globalvariables.h"


////////////////////////////////
////// CONSTANT VARIABLES //////
////////////////////////////////
const bool debugmode = false;
std::string honeyversion = "0.4.0";




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









///////////////////
//// VARIABLES ////
///////////////////
// CONSTANT STRINGS
std::string heartbeat = "heartbeatSSH";
std::string attacksend = "attacked";

// CONSTANT DEBUGGING SCRIPTS
bool debug = true;
const bool runtomain = true;

// SYSTEM VARIABLES
std::atomic<int> encounterederrors(0);
std::atomic<int> mainhost(0);
std::atomic<int> attacked(0);
std::atomic<int> logvariableset(0);
std::atomic<int> sshloginattempts(0);
std::atomic<int> sshmaxloginattempts(98);

std::atomic<std::string*> syntheticuser;
std::atomic<std::string*> syntheticpass;
int startupchecks = 0;


// SIGNAL VARIABLES
std::atomic<int> stopSIGNAL(0);
std::atomic<int> updateSIGNAL(0);
std::atomic<int> serverErrors(0);
std::atomic<int> serverStarted(0);


// COMMUNICATION VARIABLES
std::string sshterminals[255] = {};
//std::vector<std::string> sshterminals(255, "");


// SSH FIFO LOCATIONS
std::string sshfifo = "/tmp/sshfifo";
std::string cmdfifo = "/tmp/cmdfifo";


int sock, valread;
struct sockaddr_in serv_addr;
std::string hello = "Hello from client";
char buffer[1024] = {0};




// TIME VARIABLES
long long int startuptime = 0;
long long int currenttime = 0;
long long int timesincestartup = 0;
int secondsperyear = 31536000;
int daysperyear = 365.25;
int secondsperday = 86400;
int secondsperhour = 3600;
int secondsperminute = 60;
int minutesperhour = 60;
int hoursperday = 24;

bool calculatingtime = false;





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
            //loginfo("VARIABLE SET", true);
            attacked.store(1);
            return 1;
        } else {
            //loginfo("VARIABLE NOT SET", true);
            attacked.store(0);
            return 0;
        }
    }
    
}



///////////////////////////////
//// HANDLE DOCKER SIGNALS ////
///////////////////////////////
void handleSignal(int signal) {
    if (signal == SIGTERM || signal == SIGINT) {
        std::cout << "Received termination signal, shutting down gracefully..." << std::endl;
        stopSIGNAL.store(1);     
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








///////////////////////////////////////
//// RANDOM PACKAGE INSTALL THREAD ////
///////////////////////////////////////
void randompackagethread() {
    loginfo("Randomizing Packages!", true);
    int returntype = system("/usr/src/honeypi/randomize");
    if (returntype == 0) {
        loginfo("Randomized System Successfully", true);
    } else {
        logwarning("RANDOMIZE RETURN == ", false);
        sendtolog(inttostring(returntype));
    }
}










///////////////////////////
//// MAIN SETUP SCRIPT ////
///////////////////////////
int setup(int argc, char **argv) {
    // LOGFILE
    system("rm /var/rund/log.txt");
    system("touch /var/rund/log.txt");

    // START
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


    // SET DOCKER CONTAINER OPTIONS
    loginfo("DOCKER - Setting Container Options...", false);
    signal(SIGTERM, handleSignal);
    signal(SIGINT, handleSignal);
    sendtolog("OK");
    sleep(0.5);
    


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

    // RANDOMIZING SYSTEM
    loginfo("Starting Random Installation...", false);
    std::thread randompack(randompackagethread);
    randompack.detach();
    sendtolog("DONE");


    loginfo("Finishing SSH Guest V1 startup...", true);

    /*
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
*/
    loginfo("Removing Unneeded Dependencies!", true);
    int removepackage = system("apt-get remove openssh-server openssh-client -y > nul:");





    //////////////////////////////////////////////////////
    // START CLIENT CONNECTION TO MAIN HONEYPI ON 63599 //
    //////////////////////////////////////////////////////

    int PORT = 63599;





    //////////////////////
    // START FIFO PIPES //
    //////////////////////

    // CREATE NEW PIPES
    loginfo("Creating New FIFO Pipes...", false);
    int ramble = mkfifo(sshfifo.c_str(), 0666);
    int rugby = mkfifo(cmdfifo.c_str(), 0666);
    if (ramble > 0 && rugby > 0) {
        sendtolog("ERROR");
        logcritical("AN ERROR OCCURRED STARTING FIFO PIPES!", true);
        startupchecks = startupchecks + 1;
    } else {
        sendtolog("DONE");
    }


    // CREATE THREAD FOR NEW FIFO READER
    loginfo("Creating New FIFO Reader...", false);
    std::thread readbackthread(readback);
    readbackthread.detach();
    sendtolog("DONE");


    // CREATE SSH THREAD WRITER
    loginfo("Creating New FIFO Writer...", false);
    std::thread sshterm(sshwriter);
    sshterm.detach();
    sendtolog("DONE");




    
    /////////////////////////////////////
    //// CREATE ADMIN CONSOLE THREAD ////
    /////////////////////////////////////
    if (startupchecks == 0) {
        loginfo("Starting SSH Admin Console...", false);
        std::thread adminConsole(interactiveTerminal);
        adminConsole.detach();
        sendtolog("Done");
    } else {
        logcritical("NOT STARTING ADMIN CONSOLE!", true);
    }
    

    return 0;
}







/////////////////////////////
//// MAIN LOOPING SCRIPT ////
/////////////////////////////
int main(int argc, char **argv) {

    int startupchecks = setup(argc, argv);



    // SSH STARTUP THINGS
    ssh_bind sshbind;
    ssh_session session;
    ssh_event event;
    struct sigaction sa;
    int rc;
    int fd;

    //FIFO
    //mkfifo(sshfifo.c_str(), 0666);
    //std::cout << "MADE PIPE1" << std::endl;
    //sleep(10);

    rc = ssh_init();
    if (rc < 0) {
        fprintf(stderr, "ssh_init failed\n");
        return 1;
    }
    //sleep(10);

    std::cout << "ZAE" << std::endl;

    sshbind = ssh_bind_new();
    if (sshbind == NULL) {
        fprintf(stderr, "ssh_bind_new failed\n");
        return 1;
    }

    set_default_keys(sshbind, 0, 0, 0);

    if(ssh_bind_listen(sshbind) < 0) {
        fprintf(stderr, "%s\n", ssh_get_error(sshbind));
        return 1;
    }

    // PING NETWORK TEST  
    int testing234 = pingnetwork();


    loginfo("SSH Guest has started successfully...", true);
    
    // RUNNING FLAGS
    int serverstop = 0;
    int serverupdate = 0;
    int serverrunning = 1;
    stopSIGNAL.store(0);
    updateSIGNAL.store(0);
    serverStarted.store(1);

    

    while(serverstop == 0 && serverupdate == 0 && serverrunning == 1) {

        // READ RUNNNING FLAGS
        int serverstop = 0;
        int serverupdate = 0;
        int serverrunning = 1;

        // SEND HEARTBEAT TO HOST
        int pingheart = pinghost();
        if (pingheart == 50) {
            logcritical("AN ECEPTION OCCURRED THAT CAN NOT BE RECOVERED HAPPENED TRYING TO PING HOST!", true);
            logcritical("KILLING!", true);
            return 25;
            return 25;
            return 25;
        } else if (pingheart == 1) {
            logcritical("AN ERROR OCCURRED SEARCHING FOR HOST!", true);
        }

        // SETUP MAIN SSH FUNCTIONS
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

                    // CONVERT INTO NONBLOCKING EVENT
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

    // FREE EVERYTHING AND CLOSE
    ssh_bind_free(sshbind);
    ssh_finalize();

    // FIX THIS - ADD PROPER SHUTDOWN HERE IF NEEDED
    return 255;
}
