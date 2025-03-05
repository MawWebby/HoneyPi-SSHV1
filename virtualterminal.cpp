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






//////////////////////////////////////////////////
// VIRTUAL TERMINL EMULATOR TO EXECUTE COMMANDS //
//////////////////////////////////////////////////
void virtualterminal(std::string command, int method) {
    std::cout << "RECEIVED FINISHED COMMAND - " << command << " - FROM: " << method << std::endl;
    std::cout << "LENGTH WAS" << command.length() << std::endl;
    int writeback = open(sshfifo.c_str(), O_WRONLY);
    std::string commandpost = "";
    bool foundcommand = false;


    // MAIN EXECUTION OF DIFFERENT FAKE COMMANDS
    if (command.length() > 0) {

        // DIFFERENT VARIABLES FOR FASTER EXECUTING
        std::string firstthree = "";
        std::string firstfour = "";
        std::string firstfive = "";
        std::string firstsix = "";
        std::string firsteight = "";
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

            }
            // sudo
            if (firstfour == "sudo") {
                foundcommand = true;
                std::cout << "SUDO RECEIVED" << std::endl;
            }

        // ***** t


        // ***** u
            // uname
            if (firstfive == "uname") {

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
        if (foundcommand != true) {
            commandpost = "bash: " + command + ": command not found";
        }

        write(writeback, commandpost.c_str(), commandpost.length());
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


    std::string userprompt = "\npi@server:~/Documents";
    // FIX THIS TO ACCURATELY PORTRAY INFORMATION ALONG WITH TERMINAL AND BRUTE FORCE SSH

    // FOREVER LOOP TO HAVE CONSTANT READER
    // NO CONSTANT READER WILL CAUSE WRITE FUNCTION BELOW TO BLOCK INDEFINITELY
    while (true) {
        sleep(0.1);

        char whyyy[100];
        int bytes = read(fd, whyyy, 100);

        std::cout << "BYTES READ: " << bytes << std::endl;

        std::string readfromfifo = whyyy;
        readfromfifo = readfromfifo.substr(0, bytes);
        if (bytes > 0) {
            if (readfromfifo.substr(0,bytes) != "") {
                //std::cout << "CMD PIPE: " << whyyy << std::endl;
                //std::cout << "ASCII VALUE: " << (int)whyyy[0] << std::endl;

                // SHOW INPUT ON MASTER DISPLAY
                if (showinput == true) {
                    write(writeback, readfromfifo.c_str(), readfromfifo.length());
                }

                // DEBUG
                //if (bytes == 3) {
                //    std::cout << "UNCOMPRESSED: " << readfromfifo.substr(0,1) << " " << readfromfifo.substr(1,1) << " " << readfromfifo.substr(2,1) << std::endl;
                //}

                // 
                if ((int)whyyy[0] == 13) {
                    std::cout << "I AM NOW IN TERMINAL COMMAND" << std::endl << std::endl << std::endl;
                    virtualterminal(entirecommandstring, 1);
                    entirecommandstring = "";
                    write(writeback, userprompt.c_str(), userprompt.length());
                } else {
                    entirecommandstring = entirecommandstring + readfromfifo.substr(0,bytes);
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
    //std::string hellomessage = "Linux Server 6.6.74+rpt-rpi-v7 #1 SMP Raspbian 1:6.6.74-1+rpt1 (2025-01-27) armv7l\n\nThe programs included with the Debian GNU/Linux system are free software; \nthe exact distribution terms for each program are described in the \nindividual files in /usr/share/doc/*/copyright. \n\n Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent permitted by applicable law. Last login: " + time(0);
    int cached = 0;

    // userprompt fix this
    std::string userprompt = "pi@server:~/Documents";
    // FIX THIS TO ACCURATELY PORTRAY INFORMATION ALONG WITH TERMINAL AND BRUTE FORCE SSH

    
    // SEND TO FIFO PIPE
    //std::cout << "WRITEFD1" << std::endl;
    //write(fd, hellomessage.c_str(), hellomessage.length());
    //std::cout << "CLOSEFD1" << std::endl;

    sleep(1);
    
    //write(fd, hellomessage.c_str(), hellomessage.length());
        
    sleep(3);
    
    write(fd, userprompt.c_str(), userprompt.length());
    
    return;
}
