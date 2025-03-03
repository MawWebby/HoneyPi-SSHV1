// HEADERS
#include "globalvariables.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



// FIFOPipeExample.cpp
// Last Updated 3/1/25
// This file was created to learn how FIFO Files and Pipes Work in order 
// to communicate effectively between SSH Terminals, Networking, and the 
// console output. 



// FIFO file path
std::string myfifo = "/tmp/myfifo";




// FIFO READER
void readback() {
    std::cout << "OPENFD2" << std::endl;

    // FIFO PIPE MUST BE RD/WR IN ORDER TO CLEAR THE BUFFER IN THE PIPE!
    int fd = open(myfifo.c_str(), O_RDWR);

    // FOREVER LOOP TO HAVE CONSTANT READER
    // NO CONSTANT READER WILL CAUSE WRITE FUNCTION BELOW TO BLOCK INDEFINITELY
    while (true) {

        char whyyy[100];
        read(fd, whyyy, 100);

        std::string readfromfifo = whyyy;
        if (readfromfifo != "") {
            std::cout << "PIPE: " << whyyy << std::endl;
        }
    }
    close(fd);
}


// STARTING LOOP
int main() {

    // CREATE NEW PIPE
    std::cout << "CREATING NEW PIPE" << std::endl;
    int fd;
    mkfifo(myfifo.c_str(), 0666);
    std::cout << "MADE PIPE1" << std::endl;


    // CREATE THREAD FOR NEW FIFO READER
    std::cout << "CREATE FIFO READER" << std::endl;
    std::thread hey(readback);
    hey.detach();


    // OPEN FIFO WRITER
    char arr1[80], arr2[80];
    std::cout << "OPENFD1" << std::endl;
    fd = open(myfifo.c_str(), O_WRONLY);
    std::string hellomessage = "HELLO EVERYONE FROM PIPE 1";

    
    // SEND TO FIFO PIPE
    std::cout << "WRITEFD1" << std::endl;
    write(fd, hellomessage.c_str(), hellomessage.length());
    std::cout << "CLOSEFD1" << std::endl;
    sleep(2);
    close(fd);


    // PAUSE TO VERIFY READER HAS HAD TIME TO POST
    sleep(5);
    return 0;
}