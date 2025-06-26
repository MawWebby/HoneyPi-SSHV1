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
void sendtolog(std::string, bool);
void sendtologopen(std::string, bool);
void logdebug(std::string, bool, bool);
void loginfo(std::string, bool, bool);
void logwarning(std::string, bool, bool);
void logcritical(std::string, bool, bool);
void logerror(std::string, std::string, bool);

// LOG INPUT COMMANDS
void readfromlogger();

// CONVERSIONS
long long int stringtoint(std::string);
std::string inttostring(long long int);
float stringtofloat(std::string);
std::string floattostring(float);

// NETWORK PING
int pingnetwork();

// NETWORK HOST
int compilepacket(std::string, int);
int pinghost();
int packettohost(std::string);


#endif


// FIXTHIS
// ADD CURRENT TIME HOUR/DAY/ETC.