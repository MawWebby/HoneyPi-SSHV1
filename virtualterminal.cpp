#include "globalvariables.h"


// AUTHORIZED KEYS
//char authorizedkeys[DEF_STR_SIZE] = {0};


// FIFOS
//std::string sshfifo = "/tmp/sshfifo";
//std::string cmdfifo = "/tmp/cmdfifo";


// CONSTANT STRINGS
//std::string endline = "\n";


// STRING TO STORE DATA IN COMMAND
std::string entirecommandstring = "";


// CACHE TO SEND THINGS OVER SSH
//std::atomic<std::map<int, std::string>>sshcache;
std::atomic<long long int>sshcachenum;
std::atomic<int> sshdatawaitingpipe;
//std::atomic<std::string> sshcache;

bool showinput = true;

std::string previousrootlessuser = "";

// MAP FOR PREVIOSU COMMANDS
std::map<int, std::string> prevcommand;
int prevcommandint = 0;
int showingcommand = 0;



// HELPER TO CALL FOR BASH USERNAME SCRIPT
std::string userfunction() {
    // READ FOR FIFOS
    std::string userlogintype = "pi";
    int usernames = open(usefifo.c_str(), O_RDONLY, O_NONBLOCK);
    char bufuser[1000];
    int readable = read(usernames, bufuser, 1000);
    if (readable == 0) {
        std::string usernewbie = bufuser;
        userlogintype = "TEMP";
    } else {
        userlogintype = bufuser;
    }

    std::string userprompt = "\n\r" + userlogintype + "@server:~/ $ ";
    return userprompt;
}







//////////////////////////////////////////////////
// VIRTUAL TERMINL EMULATOR TO EXECUTE COMMANDS //
//////////////////////////////////////////////////
void virtualterminal(std::string command, int method, std::string inputfifo, std::string outputfifo) {
    std::cout << "RECEIVED FINISHED COMMAND - " << command << " - FROM: " << method << std::endl;
    std::cout << "LENGTH WAS" << command.length() << std::endl;
    int writeback = open(outputfifo.c_str(), O_WRONLY);
    std::string commandpost = "";
    bool foundcommand = false;
    bool validflags = true;
    bool sendtobash = true;
    bool interactivecommand = false;


    // MAIN EXECUTION OF DIFFERENT FAKE COMMANDS
    if (command.length() > 0) {

        // FIRST THING IS TO FILTER OUT BAD COMMANDS THAT I DON'T EVEN WANT TO FAKE HERE!!!

        // DIFFERENT VARIABLES FOR FASTER EXECUTING
        std::string firstthree = "";
        std::string firstfour = "";
        std::string firstfive = "";
        std::string firstsix = "";
        std::string firsteight = "";
        compilepacket(command, 7);
        if (command.length() >= 8) {
            firsteight = command.substr(0,8);
            firstsix = command.substr(0,6);
            firstfive = command.substr(0,5);
            firstfour = command.substr(0,4);
            firstthree = command.substr(0,3);
        } else if (command.length() >= 6) {
            firstsix = command.substr(0,6);
            firstfive = command.substr(0,5);
            firstfour = command.substr(0,4);
            firstthree = command.substr(0,3);
        } else if (command.length() >= 5) {
            firstfive = command.substr(0,5);
            firstfour = command.substr(0,4);
            firstthree = command.substr(0,3);
        } else if (command.length() >= 4) {
            firstfour = command.substr(0,4);
            firstthree = command.substr(0,3);
        } else if (command.length() >= 3) {
            firstthree = command.substr(0,3);
        }


        // START GOING THROUGH ALL DIFFERENT CASES TO FIND OUT WHICH COMMAND IT REALLY IS
        // ***** a
        

        // ***** b


        // ***** c
            // cat
            if (firstthree == "cat") {

            }


        // ***** d


        // ***** e
            // echo

        // ***** f


        // ***** g


        // ***** h


        // ***** i


        // ***** j


        // ***** k


        // ***** l
            // ls
            
            // logout
            if (command == "logout") {
                std::cout << "CALLED TO EXIT" << std::endl;
                commandpost = "INTERNAL: LOGOUT";
                foundcommand = true;
                sendtobash = false;
            }

        // ***** m
            // mkdir

        // ***** n
            // nano
            if (command == "nano") {
                std::cout << "NANO CALLED (INTERACTIVE)" << std::endl;
                sendtobash = true;
                interactivecommand = true;
            }

        // ***** o


        // ***** p
            // ping

        // ***** q


        // ***** r


        // ***** s
            // ssh
            if (firstthree == "ssh") {
                if (firstfive != "") {
                    // INITIATE COMMAND AS IF SOMETHING IS ACTUALLY SUPPOSED TO HAPPEN
                }
                sleep(2);

            }
            // sudo
            if (firstfour == "sudo") {
                foundcommand = true;
                sendtobash = false;
                std::cout << "SUDO RECEIVED" << std::endl;
                if (firstsix == "sudo -s") {
                    
                }
            }

        // ***** t


        // ***** u
            // uname
            if (firstfive == "uname") {
                if (command.length() == 5) {
                    commandpost = "Linux";
                    foundcommand = true;
                    sendtobash = false;
                } else {
                    sendtobash = true;
                }
            }

        // ***** v


        // ***** w
            // wget

        // ***** x


        // ***** y


        // ***** z




        ///////////////////
        // TEMP NO FOUND //
        ///////////////////
        if (foundcommand != true && sendtobash == false) {
            commandpost = "\r\n-bash: " + command + ": command not found\r";
        } else if (sendtobash == true && foundcommand != true) {
            std::string sendcommandtohost = command + " > /tmp/sshfifo";
            int returnvalue;
            if (interactivecommand == false) {
                returnvalue = system(sendcommandtohost.c_str());
            } else {
                // MAIN PID AND DUP PIPES FOR SYSTEM CALLS
                pid_t pid = 0;
                int inpipefd[2];
                int outpipefd[2];
                char buf[1024];
                char msg[1024];
                int status;

                // SUCCESS
                int successful = pipe(inpipefd);
                successful = successful + pipe(outpipefd);
                pid = fork();

                if (pid == 0 && successful == 0) {
                    int inputs = open(inputfifo.c_str(), O_RDWR);

                    int duplicator2 = dup2(writeback, STDIN_FILENO);
                    duplicator2 = duplicator2 + dup2(inputs, STDOUT_FILENO);

                    close(outpipefd[1]);
                    close(inpipefd[0]);

                    execl("/home/pi", command.c_str(), (char*) NULL);


                    exit(1);
                } else {
                    std::cout << "AN ERROR OCCURRED" << std::endl;
                }

            }
            
        }

        // WRITE STRING TO SSH
        if (commandpost != "" && sendtobash != true) {
            write(writeback, commandpost.c_str(), commandpost.length());
        }        
    }
    return;
}




/////////////////
// FIFO READER //
/////////////////
void readback(std::string readfifo, std::string writefifo) {
    // FIFO PIPE MUST BE RD/WR IN ORDER TO CLEAR THE BUFFER IN THE PIPE!
    int fd = open(readfifo.c_str(), O_RDWR);
    int writeback = open(writefifo.c_str(), O_WRONLY);
    showinput = true;

    // FOREVER LOOP TO HAVE CONSTANT READER - NO CONSTANT READER WILL CAUSE WRITE FUNCTION BELOW TO BLOCK INDEFINITELY
    while (true) {
        sleep(0.1);

        char whyyy[200];
        //int bytes = 0;
        int bytes = read(fd, whyyy, 200);

        std::string readfromfifo = whyyy;
        readfromfifo = readfromfifo.substr(0, bytes);
        if (bytes > 0) {
            if (readfromfifo.substr(0,bytes) != "") {
                //std::cout << "CMD PIPE: " << whyyy << std::endl;
                //std::cout << "ASCII VALUE: " << (int)whyyy[0] << std::endl;
                
                // SHOW INPUT ON MASTER DISPLAY
                if (showinput == true && bytes != 3) {
                    write(writeback, readfromfifo.c_str(), readfromfifo.length());
                }

                // DEBUG
                if (bytes == 3) {
                    std::cout << "UNCOMPRESSED: " << (int)whyyy[0] << " " << (int)whyyy[1] << " " << (int)whyyy[2] << std::endl;
                } else if (bytes == 1) {
                    std::cout << "UNCOMPRESSED SINGLE: " << (int)whyyy[0] << std::endl;
                }

                // FIX THIS - ADD \ REQUEST FOR NEW LINE

                // ASCII CHARACTER ASSIGNMENTS
                // ASCII 13 - ENTER KEY TO COMMAND
                if ((int)whyyy[0] == 13 && bytes == 1) {
                    // CHECK CONDITION FOR PREVIOUS AND THEN ADD TO DB
                    std::string previousindb = "";
                    if (prevcommandint == 0) {
                        previousindb = prevcommand[100];
                    } else {
                        previousindb = prevcommand[prevcommandint - 1];
                    }
                    if (previousindb != entirecommandstring) {
                        std::cout << "ENTIRE" << entirecommandstring << std::endl;
                        prevcommand[prevcommandint] = entirecommandstring;
                        prevcommandint = prevcommandint + 1;
                        std::cout << "DB" << prevcommand[prevcommandint - 1] << std::endl;
                        if (prevcommandint == 101) {
                            prevcommandint = 0;
                        }
                    }
                    showingcommand = prevcommandint;


                    // EXECUTE THE COMMAND ON BEHALF
                    virtualterminal(entirecommandstring, 1, readfifo, writefifo);
                    entirecommandstring = "";

                    std::string userprompt = userfunction();
                    write(writeback, userprompt.c_str(), userprompt.length());

                } else if (bytes != 3) {
                    entirecommandstring = entirecommandstring + readfromfifo.substr(0,bytes);

                } else if (bytes == 3) {
                    // BYTES = 3; THEN PROCEED WITH SPECIAL CHARACTERS
                    // UP        - 27;91;65
                    // DOWN      - 27;91;66
                    // RIGHT     - 27;91;67
                    // LEFT      - 27;91;68
                    // END       - 27;91;70
                    // backslash - 92
                    // Cntr-C    - 03
                    // Tab       - 9
                    // ^         - 94
                    
                    // DETERMINE IF IT IS A LETTER KEY PRESSED
                    if ((int)whyyy[0] == 27 && (int)whyyy[1] == 91) {

                        // UP ARROW
                        if ((int)whyyy[2] == 65) {
                            showingcommand = showingcommand - 1;
                            if (showingcommand == -1) {
                                showingcommand = 100;
                            }
                            std::string previouscommand = prevcommand[showingcommand];
                            if (previouscommand != "") {
                                entirecommandstring = previouscommand;
                                std::cout << "PREV" << showingcommand << entirecommandstring << std::endl;
                                // FIX THIS ADD CURSOR POSITION
                                std::string tosendssh = "\r" + userfunction() + entirecommandstring;
                                write(fd, tosendssh.c_str(), tosendssh.length());
                                // fix this weird duplicating bug here?
                            }
                        
                        // DOWN ARROW
                        } else if ((int)whyyy[2] == 66) {

                        } else {
                            entirecommandstring = entirecommandstring + readfromfifo.substr(0,bytes);
                        }
                    } else {
                        entirecommandstring = entirecommandstring + readfromfifo.substr(0,bytes);
                    }
                    
                } else {
                    logcritical("AN INVALID CASE WAS RECEIVED! CONTINUE WITH REFRESHING SSH!", true);

                    // CODE TO REFRESH SSH
                }
            }
        }
    }
    close(fd);
    close(writeback);
    return;
}



// CREATE NEW THREAD WHOSE SOLE PURPOSE IS TO FILTER THE CLI COMMAND AND CORRECTLY TERMINATE IT TO SSHFIFO
