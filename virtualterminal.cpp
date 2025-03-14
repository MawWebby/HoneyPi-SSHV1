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
    bool validflags = true;


    // MAIN EXECUTION OF DIFFERENT FAKE COMMANDS
    if (command.length() > 0) {

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
                } else {

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
        if (foundcommand != true) {
            commandpost = "\r\n-bash: " + command + ": command not found\r";
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

    //char usertype = (*currentuserssh).load();
    //std::string userlogintype(100, usertype);
    std::string userlogintype = "myuser";
    std::string userprompt = "\n" + userlogintype + "@server:~/ $ ";
    // FIX THIS TO ACCURATELY PORTRAY INFORMATION ALONG WITH TERMINAL AND BRUTE FORCE SSH

    // FOREVER LOOP TO HAVE CONSTANT READER
    // NO CONSTANT READER WILL CAUSE WRITE FUNCTION BELOW TO BLOCK INDEFINITELY
    while (true) {
        sleep(0.1);

        char whyyy[100];
        //int bytes = 0;
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
    time_t time10 = time(0); 
    std::string dateandtime = ctime(&time10);
    std::string hellomessage = "Linux Server 6.6.74+rpt-rpi-v7 #1 SMP Raspbian 1:6.6.74-1+rpt1 (2025-01-27) armv7l\r\n\r\nThe programs included with the Debian GNU/Linux system are free software;\r\nthe exact distribution terms for each program are described in the\r\nindividual files in /usr/share/doc/*/copyright.\r\n\r\nDebian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent\r\npermitted by applicable law. Last login: " + dateandtime.substr(0, dateandtime.length() - 1) + " from " + "FIX THIS\r\n";
    int cached = 0;

    // userprompt fix this
    //char usertype = (*currentuserssh).load();
    //std::string userlogintype(1, usertype);
    std::string userlogintype = "myuser";
    std::string userprompt = userlogintype + "@server:~/ $ ";
    // FIX THIS TO ACCURATELY PORTRAY INFORMATION ALONG WITH TERMINAL AND BRUTE FORCE SSH

    
    // SEND TO FIFO PIPE
    //std::cout << "WRITEFD1" << std::endl;
    //write(fd, hellomessage.c_str(), hellomessage.length());
    //std::cout << "CLOSEFD1" << std::endl;

    sleep(2);
    
    write(fd, hellomessage.c_str(), hellomessage.length());
        
    sleep(2);
    
    write(fd, userprompt.c_str(), userprompt.length());
    
    return;
}
