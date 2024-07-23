//////////////////
// Dependencies //
//////////////////
#include <cstdio>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#define sleep Sleep

#else
#include <unistd.h>
#endif
#include <iostream>
#include <fstream>
using namespace std;




/////////////////
/// VARIABLES ///
/////////////////

// CONSTANT VARIABLES
const string honeyversion = "0.0.1";
const int heartbeattime = 10;

// SYSTEM VARIABLES
bool checkforupdates = false;
int startupchecks = 0;
int encounterederrors = 0;
bool attacked = false;
bool systemup = false;
int heartbeat = 0;
string erroroccurred = "";

// REPORT VARIABLES
bool generatingreport = false;
string pubip = "0.0.0.0";
int port = 0;
string usernames[101] = {};
string passwords[101] = {};
string keybinds[1000] = {};
string fileschanged[101] = {};
string fileactions[101] = {};


const char* randomize = "./randomize";

////////////////////////////
// Send to Logger Scripts //
////////////////////////////
void sendtolog(string data2) {
    std::cout << data2 << std::endl;
}
void sendtologopen(string data2) {
    std::cout << data2;
}
void sendtologclosed(string data2) {
    std::cout << data2 << std::endl;
}
void loginfo(string data2) {
    data2 = "[INFO] - " + data2;
    sendtolog(data2);
}
void logwarning(string data2) {
    data2 = "[WARNING] - " + data2;
    sendtolog(data2);
}
void logcritical(string data2) {
    data2 = "[CRITICAL] - " + data2;
    sendtolog(data2);
}


//////////////////////////////
// REPORT GENERATING SCRIPT //
//////////////////////////////
int createreport() {
    logwarning("CREATING REPORT (THIS COULD TAKE SEVERAL MINUTES!)");

    sleep(5);


    // SAVE TO FILE
 //   myFile << "hello\n";

    return(1);

}



/////////////////////////
// THE MAIN CRASH LOOP //
/////////////////////////



////////////////////////////
// THE MAIN SETUP SCRIPTS //
//////////////////////////// 
int setup() {
    sendtolog("Hello, World");
    sendtolog("HoneyPi - GUEST Docker");
    sendtolog("Program by Matthew Whitworth (MawWebby)");
    sendtolog("Version #" + honeyversion);
    sendtolog("STARTING");
    
    // DELAY FOR SYSTEM TO START FURTHER
    sleep(5);


    // CHECK FOR SYSTEM UPDATES
    sendtologopen("[INFO] - Checking for Updates...");
    int returnedvalue = system("apt-get update > nul:");
    if (returnedvalue == 0) {
        sendtologclosed("Done");
    } else {
        sendtologclosed("ERROR");
        logcritical("UNABLE TO CHECK FOR SYSTEM UPDATES!");
        logcritical("This could be potentially dangerous!");
        logcritical("KILLING PROCESS!");
        startupchecks = startupchecks + 1;
        return 1;
        return 1;
        return 1;
    }



    // CHECK FOR SYSTEM UPDATES
    sendtologopen("[INFO] - Updating System...");
    int returnedvalue2 = system("apt-get update > nul:");
    if (returnedvalue2 == 0) {
        sendtologclosed("Done");
    } else {
        sendtologclosed("ERROR");
        logcritical("UNABLE TO UPGRADE SYSTEM!");
        logcritical("This could be potentially dangerous!");
        logcritical("KILLING PROCESS!");
        startupchecks = startupchecks + 1;
        return 1;
        return 1;
        return 1;
    }



    // PROCESS SYSTEM
    sendtologopen("[INFO] - Processing System...");
    sleep(2);


    // RANDOMIZING SYSTEM
    sendtologopen("[INFO] -  RANDOMIZING System...");
    int randomizevalue = system("./randomize");

    sendtolog("[INFO] - Guest VM with SSH has Started!");

}

int main() {

    // SETUP LOOP
    setup();

}