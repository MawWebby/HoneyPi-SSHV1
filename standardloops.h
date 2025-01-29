// STANDARD LOOP DECLARATION AND FUNCTIONS
#include <string>


extern int currentminute;
extern int currenthour;
extern int currentdays;
extern int currentyear;


#ifndef STANDARDLOOPS_H
#define STANDARDLOOPS_H

// CURRENT TIME/DATE
std::string timedetector();

// LOG OUTPUT COMMANDS
void sendtolog(std::string);
void sendtologopen(std::string);
void logdebug(std::string, bool);
void loginfo(std::string, bool);
void logwarning(std::string, bool);
void logcritical(std::string, bool);
void logerror(std::string, std::string);

// LOG INPUT COMMANDS
void readfromlogger();

// CONVERSIONS
int stringtoint(std::string);
std::string inttostring(int);

// NETWORK PING
int pingnetwork();

// NETWORK HOST
int pinghost();

#endif


// FIXTHIS
// ADD CURRENT TIME HOUR/DAY/ETC.