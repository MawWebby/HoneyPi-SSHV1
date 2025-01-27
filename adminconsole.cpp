#include "adminconsole.h"
#include "globalvariables.h"

int useraccesslevel = 0;

// BACKUP OF DATABASES

// FIX THIS - ADD CLEAR COMMAND


// USER LEVELS
// USER 1 - ACTIVATE 
// USER 0 - TEST COMMANDS ON SERVER

int currentimunte;
int currenthour;
int currentdays;
int currentyear;

// PREVIOUS COMMAND
std::string previouscommand;


/////////////////////////////////
//// WAIT FOR TERMINAL INPUT ////
/////////////////////////////////
std::string terminalinput(bool sensitive) {
    std::string command;
    std::getline(std::cin, command);
    if (sensitive == false) {
        if (command == "u") {
            switch (useraccesslevel) {
                case 0:
                    std::cout << ">> ";
                    break;
                case 1:
                    std::cout << "user >> ";
                    break;
                case 2:
                    std::cout << "admin >> ";
                    break;
                case 3:
                    std::cout << "sudo >> ";
                    break;
            }
            std::cout << previouscommand << std::endl;
            return previouscommand;
        } else {
            previouscommand = command;
        }
    }
    return command;
}



////////////////////////////////////
//// USER ACCESS LEVEL COMMANDS ////
////////////////////////////////////
void level1access() {
    std::cout << std::endl;
    std::cout << "Level 1 Access:" << std::endl;
    std::cout << "shutdown    | (NO ARGS) | Shutdown the Server" << std::endl;
    std::cout << "ping        | (NO ARGS) | Ping Internet for Connectivity" << std::endl;
}

void level0access() {
    std::cout << std::endl;
    std::cout << "Level 0 Access:" << std::endl;
    std::cout << "commands    | (NO ARGS) | Displays this list of commands" << std::endl;
    std::cout << "status      | (NO ARGS) | Status of Server Command" << std::endl;
    std::cout << "login       | (NO ARGS) | Login with Higher User" << std::endl;
    std::cout << "logout      | (NO ARGS) | Log Out of Console" << std::endl;
    std::cout << "clear       | (NO ARGS) | Clear the Terminal" << std::endl;
}




/////////////////////////////////
//// SEND TO TERMINAL SCRIPT ////
/////////////////////////////////
void sendtoterminal(std::string data) {
    std::cout << data << std::endl;
}

void processCommand(const std::string& command) {
    bool foundcommand = false;

    // SHOW GENERAL HELP IN COMMANDS
    if (command == "commands") {
        switch (useraccesslevel) {
            case 0:
                level0access();
                break;
            case 1:
                level1access();
                level0access();
                break;
        }
        foundcommand = true;
    }

    // SERVER STATUS COMMAND
    if (command == "status") {
        // FIX THIS
        std::cout << "Server Status: " << std::endl;

        foundcommand = true;
    }

    // LOGIN COMMAND
    if (command == "login") {
        system("clear");
        std::cout << "Username: ";
        std::string username = terminalinput(true);
        system("clear");
        std::cout << "Password: ";
        std::string password = terminalinput(true);
        system("clear");
        sleep(1);
        logwarning("Attempted Login @ " + username + "; Pass: " + password, true);
        int result = 0;

        // FIX THIS
        //result = logincredentials(username, password); 
        // FIX THIS

        if (result != 0) {
            useraccesslevel = result;
            std::cout << "Login was Successful" << std::endl;
            loginfo("Login Successful", true);
        } else {
            useraccesslevel = 0;
            std::cout << "Login DENIED!" << std::endl;
            logcritical("Login DENIED @ " + username + "; Pass: " + password, true);
        }
        sleep(3);
        system("clear");
        foundcommand = true;
    }

    // PING THE NETWORK
    if (command == "ping") {
        if (useraccesslevel >= 1) {
            std::cout << "Pinging Internet..." << std::endl;
            int pinger = pingnetwork();
            if (pinger == 0) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "ERROR" << std::endl;
            }
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // SHUTDOWN COMMAND
    if (command == "shutdown") {
        if (useraccesslevel >= 1) {
            stopSIGNAL.store(1);
            std::cout << "THE SERVER IS SHUTTING DOWN NOW!" << std::endl;
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // OPEN AND READ LOG FILES
    if (command == "logs") {
        if (useraccesslevel >= 2) {
            readfromlogger();
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // LOGOUT OF CONSOLE
    if (command == "logout") {
        std::cout << "Logging out..." << std::endl;
        sleep(1);
        system("clear");
        sleep(1);
        foundcommand = true;
        useraccesslevel = 0;
    }



    // START ANALYZING FIRST WORD IF NOT FOUND
    std::string firstseveral = "";
    std::string firstfour = "";
    if (command.length() >= 8 && foundcommand == false) {
        firstseveral = command.substr(0,8);
        firstfour = command.substr(0,4);
    } else if (command.length() >= 4) {
        firstfour = command.substr(0,4);
    } else {
        firstseveral = command;
        firstfour = command;
    }

    // LOCK PORTS
    if (firstfour == "lock") {
        if (useraccesslevel >= 2) {
            std::string port = "";
            std::cout << "FIX THIS" << std::endl;
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // UNLOCK PORTS
    if (firstfour == "unlo") {
        if (useraccesslevel >= 2) {
            std::string port = "";
            std::cout << "FIX THIS" << std::endl;
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }



    // MAKE SURE THE COMMAND IS FOUND
    if (foundcommand == false) {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Try 'commands' instead" << std::endl;
    }
}



// MAIN INTERACTIVE TERMINAL COMMAND
void interactiveTerminal() {
    sleep(10);
    system("clear");
    std::cout << "HoneyPi Terminal" << std::endl;
    std::cout << "HoneyPi Server Version: " << honeyversion << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    while (true) {
        std::string command;
        switch (useraccesslevel) {
            case 0:
                std::cout << ">> ";
                break;
            case 1:
                std::cout << "user >> ";
                break;
        }
        command = terminalinput(false);
        sendtolog("[CONSOLE] - Received Command: " + command);

        if (command.empty() != true) {
            processCommand(command);
        }
    }
}