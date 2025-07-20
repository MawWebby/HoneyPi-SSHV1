#include <string>
#include <unistd.h>
#include "globalvariables.h"

int main() {
    std::cout << "Starting Cowrie" << std::endl;
    std::string cowriecommand = "su - cowrie -c \"/home/cowrie/cowrie/bin/cowrie start\"";
    system(cowriecommand.c_str());
    std::cout << "DONE" << std::endl;
    sleep(10000);
}