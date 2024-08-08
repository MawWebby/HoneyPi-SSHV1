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


const bool skiprandom = true;
const bool debug = false;


/////////////////
/// VARIABLES ///
/////////////////

// CONSTANT VARIABLES
const int heartbeattime = 10;

// SYSTEM VARIABLES
bool checkforupdates = false;
int startupchecks = 0;
int encounterederrors = 0;
bool attacked = false;
bool systemup = false;
int heartbeat = 0;
string erroroccurred = "";
bool calculatingtime = false;

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


////////////////////////////
// THE MAIN SETUP SCRIPTS //
//////////////////////////// 
int setup() {
    sendtolog("STARTING");
    
    // DELAY FOR SYSTEM TO START FURTHER
    sleep(5);
    if (debug == true) {
        int testing = system("./debug");
    } else {
        int testing = system("rm debug");
    }
    


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



    // RANDOMIZING SYSTEM
    if (skiprandom == false) {
        int randomizevalue = system("./randomize");
    } else {
        int randomizevalue = system("./run");
    }
    
}

int main() {

    // SETUP LOOP
    setup();

}