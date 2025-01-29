#include "standardloops.h"
#include "globalvariables.h"
#include <random>



std::string charactermap = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
int serverStatus;
int updating;
int stopping;

// LOGFILE
const char* logfile = "/var/rund/log.txt";



// DICTIONARY
std::map<int, std::string> pingrandom = {
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







//////////////////////
//// TIMER SCRIPT ////
//////////////////////
std::string timedetector() {
    time_t time10 = time(0); 
    std::string dateandtime = ctime(&time10);
    std::string datestd = dateandtime.substr(0,10);
    std::string clockstd = dateandtime.substr(10, 9);
    std::string yearstd = dateandtime.substr(20,4);
    std::string newformat = datestd + ", " + yearstd + " " + clockstd;
    return newformat;
}



////////////////////////////
// Send to Logger Scripts //
////////////////////////////
void sendtolog(std::string data2) {
    serverStatus = serverStarted.load();
    updating = updateSIGNAL.load();
    stopping = stopSIGNAL.load();

    if (serverStatus == false || updating == true || stopping == true) {
        std::cout << data2 << std::endl;
    } 
    
    std::ofstream logfilestream;
    logfilestream.open(logfile, std::ofstream::out | std::ofstream::app);
    if (logfilestream.is_open() == true) {
        logfilestream << data2 << std::endl;
    } else {
        std::cout << "[ERRO] - COULD NOT SAVE TO LOGFILE!" << std::endl;
        std::cout << "[ERRO] - MESSAGE: " << data2 << std::endl;
    }
    logfilestream.close();
}

void sendtologopen(std::string data2) {
    serverStatus = serverStarted.load();
    updating = updateSIGNAL.load();
    stopping = stopSIGNAL.load(); 

    if (serverStatus == false || updating == true || stopping == true) {
        std::cout << data2;
    } 
    
    std::ofstream logfilestream;
    logfilestream.open(logfile, std::ofstream::out | std::ofstream::app);
    if (logfilestream.is_open() == true) {
        logfilestream << data2;
    } else {
        std::cout << "[ERRO] - COULD NOT SAVE TO LOGFILE!" << std::endl;
        std::cout << "[ERRO] - MESSAGE: " << data2 << std::endl;
    }
    logfilestream.close();
}

void logdebug(std::string data2, bool complete) {
    std::string timedet = timedetector();
    data2 = "[DEBUG] - " + timedet + " - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void loginfo(std::string data2, bool complete) {
    std::string timedet = timedetector();
    data2 = "[INFO ] - " + timedet + " - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logwarning(std::string data2, bool complete) {
    std::string timedet = timedetector();
    data2 = "[WARN*] - " + timedet + " - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logcritical(std::string data2, bool complete) {
    std::string timedet = timedetector();
    data2 = "[ERRNO] - " + timedet + " - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logerror(std::string headerdata2, std::string errormessage) {
    std::string timedet = timedetector();
    std::string data2 = "[ERROR] - " + timedet + " - " + headerdata2 + " - " + errormessage;
    sendtolog(data2);
}




/////////////////////////////////
//// READ FROM LOGGER SRIPTS ////
/////////////////////////////////
void readfromlogger() {
    std::ifstream logfilestream;
    logfilestream.open(logfile);
    if (logfilestream.is_open() == true) {
        char linebits[2048];
        while (logfilestream.eof() == false) {
            logfilestream.getline(linebits, 2048);
            std::cout << linebits << std::endl;
        }
        std::cout << std::endl << "Reached End of Log File" << std::endl;
    } else {
        std::cout << "[ERRO] - COULD NOT OPEN LOGFILE!" << std::endl;
    }
    logfilestream.close();
}





//////////////////////////////////
//// STRING TO INT CONVERSION //// 
//////////////////////////////////
int stringtoint(std::string values) {
    long int valuesdetermine = 0;
    bool completedwhile = false;
    int timing9760 = 0;
    int timing9760max = values.length();
    bool validcase = false;
    bool periodcheck = false;
    
    // CHECK FOR NULL
    if (values.length() == 0) {
        loginfo("RECEIVED NULL STRING TO ANALYZE! (STD->INT)", true);
        return 0;
    }

    // CHECK FOR LONG INT
    if (values.length() > 9) {
        loginfo("RECEIVED A SUPER LONG STRING TO ANALYZE! (STD->INT)", true);
        return 0;
    }

    // MAIN CHECK LOOP
    std::string substringvalue = "";
    while (completedwhile == false && timing9760 <= timing9760max) {
        if (timing9760 + 1 <= timing9760max) {
            validcase = false;
            if (periodcheck == false) {
                valuesdetermine = valuesdetermine * 10;
                periodcheck = false;
            } else {
                periodcheck = false;;
            }
            substringvalue = values.substr(timing9760, 1);
            if (substringvalue == "0") {
                validcase = true;
            } else if (substringvalue == "1") {
                validcase = true;
                valuesdetermine = valuesdetermine + 1;
            } else if (substringvalue == "2") {
                validcase = true;
                valuesdetermine = valuesdetermine + 2;
            } else if (substringvalue == "3") {
                validcase = true;
                valuesdetermine = valuesdetermine + 3;
            } else if (substringvalue == "4") {
                validcase = true;
                valuesdetermine = valuesdetermine + 4;
            } else if (substringvalue == "5") {
                validcase = true;
                valuesdetermine = valuesdetermine + 5;
            } else if (substringvalue == "6") {
                validcase = true;
                valuesdetermine = valuesdetermine + 6;
            } else if (substringvalue == "7") {
                validcase = true;
                valuesdetermine = valuesdetermine + 7;
            } else if (substringvalue == "8") {
                validcase = true;
                valuesdetermine = valuesdetermine + 8;
            } else if (substringvalue == "9") {
                validcase = true;
                valuesdetermine = valuesdetermine + 9;
            }


            // DECIMALS (IGNORE THIS CASE)
            // FIX THIS
            else if (substringvalue == ".") {
                validcase = true;
                periodcheck = true;
            }


            // INVALID STRING
            if (validcase != true) {
                loginfo("AN INVALID CHARACTER WAS RECEIVED (STD->INT); THE CHARACTER WAS: ", false);
                sendtolog(substringvalue);
                completedwhile = true;
                return 0;
            }   
        } else {
            completedwhile == true;
        }
        timing9760 = timing9760 + 1;
    }

    return valuesdetermine;
}



//////////////////////////////////
//// INT TO STRING CONVERSION //// 
//////////////////////////////////
std::string inttostring(int value) {
    std::string returnvalue = "";
    bool single = false;
    bool doublenum = false;
    bool triple = false;
    bool quad = false;
    bool penta = false;
    bool hexa = false;

    // DETERMINE CASE
    if (value < 9) {
        single = true;
        doublenum = false;
        triple = false;
        quad = false;
        penta = false;
        hexa = false;
    } else if (value < 99) {
        single = true;
        doublenum = true;
        triple = false;
        quad = false;
        penta = false;
        hexa = false;
    } else if (value < 999) {
        single = true;
        doublenum = true;
        triple = true;
        quad = false;
        penta = false;
        hexa = false;
    } else if (value < 9999) {
        single = true;
        doublenum = true;
        triple = true;
        quad = true;
        penta = false;
        hexa = false;
    } else if (value < 99999) {
        single = true;
        doublenum = true;
        triple = true;
        quad = true;
        penta = true;
        hexa = false;
    } else if (value < 999999) {
        single = true;
        doublenum = true;
        triple = true;
        quad = true;
        penta = true;
        hexa = true;
    }

    // START WORK ON EACH CASE
    if (single == true) {
        int numbertech = value % 10;
        switch (numbertech) {
            case 0:
                returnvalue = "0";
                break;
            case 1:
                returnvalue = "1";
                break;
            case 2:
                returnvalue = "2";
                break;
            case 3:
                returnvalue = "3";
                break;
            case 4:
                returnvalue = "4";
                break;
            case 5:
                returnvalue = "5";
                break;
            case 6:
                returnvalue = "6";
                break;
            case 7:
                returnvalue = "7";
                break;
            case 8:
                returnvalue = "8";
                break;
            case 9:
                returnvalue = "9";
                break;
        }
    }
    if (doublenum == true) {
        value = value / 10;
        int numbertech = value % 10;
        switch (numbertech) {
            case 0:
                returnvalue = "0" + returnvalue;
                break;
            case 1:
                returnvalue = "1" + returnvalue;
                break;
            case 2:
                returnvalue = "2" + returnvalue;
                break;
            case 3:
                returnvalue = "3" + returnvalue;
                break;
            case 4:
                returnvalue = "4" + returnvalue;
                break;
            case 5:
                returnvalue = "5" + returnvalue;
                break;
            case 6:
                returnvalue = "6" + returnvalue;
                break;
            case 7:
                returnvalue = "7" + returnvalue;
                break;
            case 8:
                returnvalue = "8" + returnvalue;
                break;
            case 9:
                returnvalue = "9" + returnvalue;
                break;
        }
    }
    if (triple == true) {
        value = value / 10;
        int numbertech = value % 10;
        switch (numbertech) {
            case 0:
                returnvalue = "0" + returnvalue;
                break;
            case 1:
                returnvalue = "1" + returnvalue;
                break;
            case 2:
                returnvalue = "2" + returnvalue;
                break;
            case 3:
                returnvalue = "3" + returnvalue;
                break;
            case 4:
                returnvalue = "4" + returnvalue;
                break;
            case 5:
                returnvalue = "5" + returnvalue;
                break;
            case 6:
                returnvalue = "6" + returnvalue;
                break;
            case 7:
                returnvalue = "7" + returnvalue;
                break;
            case 8:
                returnvalue = "8" + returnvalue;
                break;
            case 9:
                returnvalue = "9" + returnvalue;
                break;
        }
    }
    if (quad == true) {
        value = value / 10;
        int numbertech = value % 10;
        switch (numbertech) {
            case 0:
                returnvalue = "0" + returnvalue;
                break;
            case 1:
                returnvalue = "1" + returnvalue;
                break;
            case 2:
                returnvalue = "2" + returnvalue;
                break;
            case 3:
                returnvalue = "3" + returnvalue;
                break;
            case 4:
                returnvalue = "4" + returnvalue;
                break;
            case 5:
                returnvalue = "5" + returnvalue;
                break;
            case 6:
                returnvalue = "6" + returnvalue;
                break;
            case 7:
                returnvalue = "7" + returnvalue;
                break;
            case 8:
                returnvalue = "8" + returnvalue;
                break;
            case 9:
                returnvalue = "9" + returnvalue;
                break;
        }
    }
    if (penta == true) {
        value = value / 10;
        int numbertech = value % 10;
        switch (numbertech) {
            case 0:
                returnvalue = "0" + returnvalue;
                break;
            case 1:
                returnvalue = "1" + returnvalue;
                break;
            case 2:
                returnvalue = "2" + returnvalue;
                break;
            case 3:
                returnvalue = "3" + returnvalue;
                break;
            case 4:
                returnvalue = "4" + returnvalue;
                break;
            case 5:
                returnvalue = "5" + returnvalue;
                break;
            case 6:
                returnvalue = "6" + returnvalue;
                break;
            case 7:
                returnvalue = "7" + returnvalue;
                break;
            case 8:
                returnvalue = "8" + returnvalue;
                break;
            case 9:
                returnvalue = "9" + returnvalue;
                break;
        }
    }
    if (hexa == true) {
        value = value / 10;
        int numbertech = value % 10;
        switch (numbertech) {
            case 0:
                returnvalue = "0" + returnvalue;
                break;
            case 1:
                returnvalue = "1" + returnvalue;
                break;
            case 2:
                returnvalue = "2" + returnvalue;
                break;
            case 3:
                returnvalue = "3" + returnvalue;
                break;
            case 4:
                returnvalue = "4" + returnvalue;
                break;
            case 5:
                returnvalue = "5" + returnvalue;
                break;
            case 6:
                returnvalue = "6" + returnvalue;
                break;
            case 7:
                returnvalue = "7" + returnvalue;
                break;
            case 8:
                returnvalue = "8" + returnvalue;
                break;
            case 9:
                returnvalue = "9" + returnvalue;
                break;
        }
    }
    return returnvalue;
}




//////////////////
// PING NETWORK //
//////////////////
int pingnetwork() {
    int testnumber = 0;
    testnumber = int((rand() % 60));
    int result;
    std::string starter3 = "ping -c 5 ";
    std::string ender3 = " > nul: ";
    std::string pingnetworkcom = "";
    pingnetworkcom = pingnetworkcom + starter3;
    pingnetworkcom = pingnetworkcom + pingrandom[testnumber];
    pingnetworkcom = pingnetworkcom + ender3;
    result = system(pingnetworkcom.c_str());
    if (result != 0) {
        logwarning("UNABLE TO PING WEBSITE!", true);
    }
    return result;
}




///////////////
// PING HOST //
///////////////
int pinghost() {
    struct addrinfo hints, *res;
    int sock;
    struct sockaddr_in serv_addr;   

    if (getaddrinfo("HoneyPiMain", nullptr, &hints, &res) != 0) {
        sendtolog("ERROR");
        logcritical("Unable to resolve hostname!", true);
        if (debug == true) {
            loginfo("Not killing in debug mode", true);
            mainhost = false;
            return 1;
        } else {
            logcritical("Killing docker container", true);
            encounterederrors = encounterederrors + 1;
            mainhost = false;
            return 50;
        }
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(63599);
    serv_addr.sin_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;

    freeaddrinfo(res);


    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        logcritical("Socket creation error!", true);
        if (debug == true) {
            loginfo("Not killing in debug mode", true);
            return 1;
        } else {
            logcritical("Killing docker container", true);
            encounterederrors = encounterederrors + 1;
            return 50;
        }
    }

    // Connect to the server 63599
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        logcritical("Connection failed!", true);
        if (debug == true) {
            loginfo("Not killing in debug mode", true);
            return 1;
        } else {
            logcritical("Killing docker container", true);
            encounterederrors = encounterederrors + 1;
            return 50;
        }
    }

    sleep(2);
    send(sock, heartbeat.c_str(), heartbeat.size(), 0);

    close(sock);
    return 0;
}