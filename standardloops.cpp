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
// 0 - US1 - USERNAMES/PASSWORD COMBO
// 33 - US2 - USERNAMES/PASSWORD COMBO.2
// 1 - USE - USERNAME
// 2 - PWD - PASSWORD
// 3 - MUP - MULTIPLEIP
// 4 - SUP - SINGLEIP
// 5 - ADD - IPADDR
// 6 - FIN - COMPLETE
// 7 - CMD - COMMANDRUN
// 8 - FVW - FILEVIEW
// 9 - FCH - FILECHANGES
// 10 - FED - FILEEDITS
// 11 - SMM - SINGLE/MULTI MODE
// 12 - EXA - EXTRAOPT ARRAY
// 13 - EX1 - EXTRAOPT1
// 14 - EX2 - EXTRAOPT2
// 15 - EX3 - EXTRAOPT3
// 16 - EX4 - EXTRAOPT4
// 17 - EX5 - EXTRAOPT5
// 18 - EX6 - EXTRAOPT6
// 19 - EX7 - EXTRAOPT7
// 20 - EX8 - EXTRAOPT8
// 21 - EX9 - EXTRAOPT9
// 22 - E10 - EXTRAOPT10
// 23 - E11 - EXTRAOPT11
// 24 - SOF - GUEST REPORT
// 25 - ROP - reportV
// 26 - CMB - COMMAND BLOCK
// 27 - CMR - COMMAND RUN
// 28 - KIL - TERMINATE SESSION
// 29 - WAT - MAYDAY WATCHDOG KILL
// 30 - MUR - BLOCK ALL CONTAINERS TO KILL (ACTIVE TARGET OF ATTACK)
// 31 - TES - TEST
// 32 - FTE - VERIFY TEST
int compilepacket(std::string data1, int packetinto) {
    std::string messagestring = "";

    // CHECK FOR DATA1 CONDITION
    if (data1 == "") {
        logcritical("NULL DATA RECEIVED!", true);
        return 200;
    }

    bool sendtohost = false;
    switch(packetinto) {
        case 0:
            messagestring = "US1: " + data1;
            sendtohost = true;
            break;
        case 1:
            messagestring = "USE: " + data1;
            sendtohost = true;
            break;
        case 2:
            messagestring = "PWD: " + data1;
            sendtohost = true;
            break;
        case 3:
            messagestring = "MUP: " + data1;
            sendtohost = true;
            break;
        case 4:
            messagestring = "SUP: " + data1;
            sendtohost = true;
            break;
        case 5:
            messagestring = "ADD: " + data1;
            sendtohost = true;
            break;
        case 6:
            messagestring = "FIN: " + data1;
            sendtohost = true;
            break;
        case 7:
            messagestring = "CMD: " + data1;
            sendtohost = true;
            break;
        case 8:
            messagestring = "FVW: " + data1;
            sendtohost = true;
            break;
        case 9:
            messagestring = "FCH: " + data1;
            sendtohost = true;
            break;
        case 10:
            messagestring = "FED: " + data1;
            sendtohost = true;
            break;
        case 11:
            messagestring = "SMM: " + data1;
            sendtohost = true;
            break;
        case 12:
            messagestring = "EXA: " + data1;
            sendtohost = true;
            break;
        case 13:
            messagestring = "EX1: " + data1;
            sendtohost = true;
            break;
        case 14:
            messagestring = "EX2: " + data1;
            sendtohost = true;
            break;
        case 15:
            messagestring = "EX3: " + data1;
            sendtohost = true;
            break;
        case 16:
            messagestring = "EX4: " + data1;
            sendtohost = true;
            break;
        case 17:
            messagestring = "EX5: " + data1;
            sendtohost = true;
            break;
        case 18:
            messagestring = "EX6: " + data1;
            sendtohost = true;
            break;
        case 19:
            messagestring = "EX7: " + data1;
            sendtohost = true;
            break;
        case 20:
            messagestring = "EX8: " + data1;
            sendtohost = true;
            break;
        case 21:
            messagestring = "EX9: " + data1;
            sendtohost = true;
            break;
        case 22:
            messagestring = "E10: " + data1;
            sendtohost = true;
            break;
        case 23:
            messagestring = "E11: " + data1;
            sendtohost = true;
            break;
        case 24:
            messagestring = "SOF: " + data1;
            sendtohost = true;
            break;
        case 25:
            messagestring = "ROP: " + data1;
            sendtohost = true;
            break;
        case 26:
            messagestring = "CMB: " + data1;
            sendtohost = true;
            break;
        case 27:
            messagestring = "CMR: " + data1;
            sendtohost = true;
            break;
        case 28:
            messagestring = "KIL: " + data1;
            sendtohost = true;
            break;
        case 29:
            messagestring = "WAT: " + data1;
            sendtohost = true;
            break;
        case 30:
            messagestring = "MUR: " + data1;
            sendtohost = true;
            break;
        case 31:
            messagestring = "TES: " + data1;
            sendtohost = true;
            break;
        case 32:
            messagestring = "FTE: " + data1;
            sendtohost = true;
            break;
        case 33:
            messagestring = "US2: " + data1;
            sendtohost = true;
            break;
        default:
            logcritical("NO VALID INT RECEIVED TO CONNECT TO MAIN!", true);
            logcritical("RECEIVED int=" + inttostring(packetinto) + " FOR:" + data1, true);
            sendtohost = false;
            break;
    }

    // SEND TO MAIN HOST
    int returnvalues = 0;
    if (sendtohost == true) {
        returnvalues = packettohost(messagestring);
    } else {
        returnvalues = 100;
    }
    return returnvalues;
}



///////////////
// PING HOST //
///////////////
int pinghost() {
    int returnvalues = packettohost("heartbeatSSH");
    return returnvalues;
}




/////////////////////////
// SEND PACKET TO HOST //
/////////////////////////
int packettohost(std::string packettosend) {
    if (packettosend == "") {
        return 1;
    }

    // DEFAULT UTIL
    std::string host = "HoneyPiMain";
    std::string port = "63599";
    struct addrinfo hints, *res, *p;
    int status;
    int socket_fd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    
    if ((status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return 1;
    }

    // Loop through the results and connect to the first valid one
    for (p = res; p != nullptr; p = p->ai_next) {
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_fd);
            continue;
        }
        break; // Connection successful
    }

    if (p == nullptr) {
        std::cerr << "Failed to connect" << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    // Print the IP address of the connected server
    void *addr;
    char ipstr[INET6_ADDRSTRLEN];

    if (p->ai_family == AF_INET) { // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);
    } else { // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
        addr = &(ipv6->sin6_addr);
    }
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    std::cout << "Connected to " << host << " address: " << ipstr << std::endl;

    // SEND PACKET TO HOST
    send(socket_fd, packettosend.c_str(), packettosend.size(), 0);

    freeaddrinfo(res); // Free the linked list

    close(socket_fd);
    return 0;
}