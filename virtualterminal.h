// VIRTUAL TERMINAL EMULATOR DECLARATION H FILE

#include <string>

#ifndef VIRTUALTERMINAL_H
#define VIRTUALTERMINAL_H

// USER SCRIPT
std::string userfunction();

// MAIN VIRTUAL TERMINAL THREAD
void virtualterminal(std::string, int);

// MAIN READBACK FUNCTION FROM SSH
void readback();

#endif