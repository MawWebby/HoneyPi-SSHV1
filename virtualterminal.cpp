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

    std::string userprompt = "\n" + userlogintype + "@server:~/ $ ";
    return userprompt;
}







//////////////////////////////////////////////////
// VIRTUAL TERMINL EMULATOR TO EXECUTE COMMANDS //
//////////////////////////////////////////////////
void virtualterminal(std::string command, int method) {
    std::cout << "RECEIVED FINISHED COMMAND - " << command << " - FROM: " << method << std::endl;
    std::cout << "LENGTH WAS" << command.length() << std::endl;
    int writeback = open(sshfifo.c_str(), O_WRONLY);
    std::string commandpost = "";
    bool foundcommand = false;
    bool validflags = true;
    bool sendtobash = false;


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
            }

        // ***** m
            // mkdir

        // ***** n
            // nano

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
            std::string sendcommandtohost = command + "> /tmp/sshfifo";
            int returnvalue = system(sendcommandtohost.c_str());
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
void readback() {
    // FIFO PIPE MUST BE RD/WR IN ORDER TO CLEAR THE BUFFER IN THE PIPE!
    int fd = open(cmdfifo.c_str(), O_RDWR);
    int writeback = open(sshfifo.c_str(), O_WRONLY);
    showinput = true;

    // FOREVER LOOP TO HAVE CONSTANT READER
    // NO CONSTANT READER WILL CAUSE WRITE FUNCTION BELOW TO BLOCK INDEFINITELY
    while (true) {
        sleep(0.1);

        char whyyy[100];
        //int bytes = 0;
        int bytes = read(fd, whyyy, 100);

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
                        prevcommand[prevcommandint] = entirecommandstring;
                        prevcommandint = prevcommandint + 1;
                        if (prevcommandint == 101) {
                            prevcommandint = 0;
                        }
                    }
                    showingcommand = prevcommandint;


                    // EXECUTE THE COMMAND ON BEHALF
                    virtualterminal(entirecommandstring, 1);
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
                                // FIX THIS ADD CURSOR POSITION
                                std::string tosendssh = "\r" + userfunction() + entirecommandstring;
                                write(fd, tosendssh.c_str(), tosendssh.length());
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
    return;
}


// OPENING SSH WRITER INTO FAKE TERMINAL
void sshwriter() {
    // OPEN FIFO WRITER
    char arr1[80], arr2[80];
    std::map<int, std::string> mappymap;
    int fd = open(sshfifo.c_str(), O_WRONLY);
    time_t time10 = time(0); 
    std::string dateandtime = ctime(&time10);
    std::string hellomessage = "Linux Server 6.6.74+rpt-rpi-v7 #1 SMP Raspbian 1:6.6.74-1+rpt1 (2025-01-27) armv7l\r\n\r\nThe programs included with the Debian GNU/Linux system are free software;\r\nthe exact distribution terms for each program are described in the\r\nindividual files in /usr/share/doc/*/copyright.\r\n\r\nDebian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent\r\npermitted by applicable law. Last login: " + dateandtime.substr(0, dateandtime.length() - 1) + " from " + "FIX THIS\r\n";
    int cached = 0;

    // userprompt fix this
    //char usertype = (*currentuserssh).load();
    //std::string userlogintype(1, usertype);
    sleep(2);
    //std::string userlogintype = *(usernamessh.load());
    // FIX THIS - userprompt
    std::string userlogintype = "pi";
                    
    // READ FOR FIFOS
    int usernames = open(usefifo.c_str(), O_RDONLY, O_NONBLOCK);
    char bufuser[1000];
    int readable = read(usernames, bufuser, 1000);
    if (readable == 0) {
        std::string usernewbie = bufuser;
        userlogintype = "TEMP";
    } else {
        userlogintype = bufuser;
    }

    std::string userprompt = userfunction();
    
    write(fd, hellomessage.c_str(), hellomessage.length());
        
    sleep(1);
    
    write(fd, userprompt.c_str(), userprompt.length());
    
    return;
}



// CREATE NEW THREAD WHOSE SOLE PURPOSE IS TO FILTER THE CLI COMMAND AND CORRECTLY TERMINATE IT TO SSHFIFO
