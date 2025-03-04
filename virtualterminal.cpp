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



//////////////////////////////////////////////////
// VIRTUAL TERMINL EMULATOR TO EXECUTE COMMANDS //
//////////////////////////////////////////////////
void virtualterminal(std::string command, int method) {
    std::cout << "RECEIVED FINISHED COMMAND - " << command << " - FROM: " << method << std::endl;

    return;
}


/////////////////
// FIFO READER //
/////////////////
void readback() {
    std::cout << "OPENFD2" << std::endl;

    // FIFO PIPE MUST BE RD/WR IN ORDER TO CLEAR THE BUFFER IN THE PIPE!
    int fd = open(cmdfifo.c_str(), O_RDWR);

    // FOREVER LOOP TO HAVE CONSTANT READER
    // NO CONSTANT READER WILL CAUSE WRITE FUNCTION BELOW TO BLOCK INDEFINITELY
    while (true) {

        char whyyy[100];
        int bytes = read(fd, whyyy, 100);

        std::cout << "BYTES READ: " << bytes << std::endl;

        std::string readfromfifo = whyyy;
        readfromfifo = readfromfifo.substr(0, bytes);
        if (readfromfifo != "") {
            std::cout << "CMD PIPE: " << whyyy << std::endl;
            entirecommandstring = entirecommandstring + readfromfifo;
            if (readfromfifo == "\n" && entirecommandstring.substr(entirecommandstring.length() - 3, 1) != "") {
                virtualterminal(entirecommandstring, 1);
            }
        }
    }
    close(fd);
    return;
}


void sshwriter() {
    // OPEN FIFO WRITER
    char arr1[80], arr2[80];
    std::map<int, std::string> mappymap;
    std::cout << "OPENFDSSH" << std::endl;
    int fd = open(sshfifo.c_str(), O_WRONLY);
    std::string hellomessage = "HELLO EVERYONE FROM SSH WRITER";

    int cached = 0;
    
    // SEND TO FIFO PIPE
    //std::cout << "WRITEFD1" << std::endl;
    //write(fd, hellomessage.c_str(), hellomessage.length());
    //std::cout << "CLOSEFD1" << std::endl;

    sleep(2);


    std::string ones = "one";
    std::string tw = "two";
    std::string th = "three";
    std::string fo = "four";
    
    mappymap[sshcachenum] = hellomessage;
    sshcachenum = sshcachenum + 1;
    mappymap[sshcachenum] = ones;
    sshcachenum = sshcachenum + 1;
    mappymap[sshcachenum] = tw;
    sshcachenum = sshcachenum + 1;
    mappymap[sshcachenum] = th;
    sshcachenum = sshcachenum + 1;
    mappymap[sshcachenum] = fo;
    sshcachenum = sshcachenum + 1;


    
    sleep(2);
    sshcachenum = sshcachenum + 3;



    while (true) {
        //sleep(1);

        std::string tosendout = mappymap[cached];
        write(fd, tosendout.c_str(), tosendout.length());
        cached = cached + 1;
        sshdatawaitingpipe.store(1);
        
    }
    close(fd);
}
