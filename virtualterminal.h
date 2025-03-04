// VIRTUAL TERMINAL EMULATOR DECLARATION H FILE

#include <string>

#ifndef VIRTUALTERMINAL_H
#define VIRTUALTERMINAL_H

// MAIN VIRTUAL TERMINAL THREAD
void virtualterminal(std::string, int);

// MAIN READBACK FUNCTION FROM SSH
void readback();

// MAIN SENDING FUNCTION TO SSH
void sshwriter();

#endif