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
    std::cout << "logs        | (NO ARGS) | View Logs of SSH Guets" << std::endl;
    std::cout << "pinghost    | (NO ARGS) | Ping the Host Device" << std::endl;
    std::cout << "sendtorep   | (NO ARGS) | Send All Report Flags to the Host and Verify Working" << std::endl;
}

void level0access() {
    std::cout << std::endl;
    std::cout << "Level 0 Access:" << std::endl;
    std::cout << "commands    | (NO ARGS) | Displays this list of commands" << std::endl;
    std::cout << "status      | (NO ARGS) | Status of Server Command" << std::endl;
    std::cout << "login       | (NO ARGS) | Login with Higher User" << std::endl;
    std::cout << "logout      | (NO ARGS) | Log Out of Console" << std::endl;
    std::cout << "clear       | (NO ARGS) | Clear the Terminal" << std::endl;
    std::cout << "testfifo    | (NO ARGS) | Test the Internal FIFO Communication of Docker" << std::endl;
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
        result = logincredentials(username, password); 
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

    // PING HOST COMMAND
    if (command == "pinghost") {
        if (useraccesslevel >= 1) {
            std::cout << "Pinging Host Device..." << std::endl;
            int pingresults = pinghost();
            if (pingresults == 0) {
                std::cout << "OK" << std::endl;
            } else if (pingresults == 1) {
                std::cout << "Returned Value of 1! (Socket Creation Error)" << std::endl;
            } else if (pingresults == 50) {
                std::cout << "Returned Value of 50! (Socket Fail/Kill Error)" << std::endl;
            } else {
                std::cout << "UNKNOWN RETURN VALUE!" << std::endl;
            }
        }
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

    // TEST INTERNAL
    if (command == "testfifo") {
        if (useraccesslevel >= 0) {
            std::cout << "RUNNING COMMAND" << std::endl;
            int filedescript = open(infifo.c_str(), O_WRONLY);
            std::cout << "OPENED" << std::endl;
            std::string controller = "INTERNAL: CONTROLLER: YAY!";
            write(filedescript, controller.c_str(), controller. length());
            std::cout << "WROTE" << std::endl;
            close(filedescript);
            std::cout << "SENT" << std::endl;
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
    }

    // OPEN AND READ LOG FILES
    if (command == "logs") {
        if (useraccesslevel >= 1) {
            readfromlogger();
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    if (command == "sendtorep") {
        if (useraccesslevel >= 0) {
            std::cout << "Testing Full Report to Main Host!" << std::endl;
            std::cout << "Verifying Main Host Connectivity...";
            int pingresults = pinghost();
            if (pingresults == 0) {
                std::cout << "OK" << std::endl;
            } else if (pingresults == 1) {
                std::cout << "Returned Value of 1! (Socket Creation Error)" << std::endl;
            } else if (pingresults == 50) {
                std::cout << "Returned Value of 50! (Socket Fail/Kill Error)" << std::endl;
            } else {
                std::cout << "UNKNOWN RETURN VALUE!" << std::endl;
            }

            // CONTINUE ON TO SENDING COMMANDS
            if (pingresults == 0) {
                std::cout << "Setting Test Flag..." << std::endl;
                pingresults = compilepacket("true", 31);
                if (pingresults == 0) {
                    std::cout << "OK" << std::endl;
                } else {
                    std::cout << "ERROR occurred while setting test flag to true. (" << pingresults << ")" << std::endl;
                }
            }

            // CONTINUE IF PINGRESULTS CONTINUES TO EQUAL 0
            if (pingresults == 0) {
                std::cout << "Sending All Conditions to Host!" << std::endl;
                int lessthan = 0;
                int target = 5;
                while (lessthan <= target) {
                    int testflagargs = 0;
                    int testflagargsmax = 34;
                    int returntype = 0;
                    while (testflagargs < testflagargsmax) {
                        std::cout << "Sending Flag #" << testflagargs << "..." << std::endl;
                        returntype = compilepacket("test", testflagargs);
                        if (returntype != 0) {
                            std::cout << "ERROR (" << returntype << ")" << std::endl;
                        }
                        testflagargs = testflagargs + 1;
                    }
                    lessthan = lessthan + 1;
                }
                std::cout << "Testing Fail Conditions..." << std::endl;
                int failcondition = 0;
                failcondition = compilepacket("test", 100);
                failcondition = failcondition + compilepacket("", 0);
                if (failcondition == 300) {
                    std::cout << "OK" << std::endl;
                } else if (failcondition == 200) {
                    std::cout << "ERROR (1)" << std::endl;
                } else if (failcondition == 100) {
                    std::cout << "ERROR (2)" << std::endl;
                } else {
                    std::cout << "ERROR (3)" << std::endl;
                }
                std::cout << "FINISHED" << std::endl;
                std::cout << "Ending Report...";
                int endreport = compilepacket("true", 6);
                int testreport = compilepacket("false", 31);
                int testsreport = compilepacket("true", 32);
                if (endreport == 0 && testreport == 0 && testsreport == 0) {
                    std::cout << "OK" << std::endl;
                } else {
                    std::cout << "ERROR" << std::endl;
                }
                std::cout << "On 'HoneyPiMain', type in the command 'compiletest' to verify connectivity." << std::endl;
            }
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
        if (useraccesslevel >= 1) {
            std::string port = "";
            std::cout << "FIX THIS" << std::endl;
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // UNLOCK PORTS
    if (firstfour == "unlo") {
        if (useraccesslevel >= 1) {
            std::string port = "";
            std::cout << "FIX THIS" << std::endl;
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    if (command == "bash") {
        if (useraccesslevel >= 1) {
            std::cout << "Sending to Bash..." << std::endl;
            system("bash");
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
    std::cout << "HoneyPi Version: " << honeyversion << std::endl;
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