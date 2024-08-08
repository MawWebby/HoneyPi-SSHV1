#include <cstdlib>
#include <string>
#include <cstring>
#include <map>

#ifdef _WIN32
#include <windows.h>
#define sleep Sleep
#else
#include <unistd.h>
#endif
#include <iostream>


const char* starter2 = "apt-get install -y -qq ";
const char* endstring = " > nul:";

int packages[30] = {};
const char* sentence = "";


std::map<int, char*> packagedictionary = {
    {0, " atop "},
    {1, " htop "},
    {2, " build-essential "},
    {3, " make "},
    {4, " nodejs "},
    {5, " libreoffice "},
    {6, " firefox "},
    {7, " chromium-browser "},
    {8, " docker.io "},
    {9, " vim "},
    {10, " git "},
    {11, " nginx "},
    {12, " apache2 "},
    {13, " curl "},
    {14, " bash "},
    {15, " kubernetes "},
    {16, " mysql-server "},
    {17, " mysql-client "},
    {18, " xwayland "},
    {19, " xserver "},
    {20, " xz-utils "},
    {21, " pip "},
    {22, " synaptic "},
    {23, " gedit "},
    {24, " filezilla "},
    {25, " gedit "},
    {26, " remmina "},
    {27, " steam "},
    {28, " flameshot "},
    {29, " calibre "},
    {30, " vim-gtk3 "},
    {31, " cron "},
    {32, " transmission "},
    {33, " emacs "},
    {34, " nano "},
    {35, " thunderbird "},
    {36, " geary "},
    {37, " evolution "},
    {38, " steam "},
    {39, " lutris "},
    {40, " nginx "},
    {41, " snap "},
    {42, " cinnamon "},
    {43, " ubuntu-mate-desktop "},
    {44, " xfce4 "},
    {45, " gnome-tweak-tool "},
    {46, " gnome-terminal "},
    {47, " konsole "},
    {48, " terminator "},
    {49, " ardour "},
    {50, " audacity "},
    {51, " blender "},
    {52, " gimp "},
    {53, " openshot "},
    {54, " kdenlive"},
    {55, " rhythmbox"},
    {56, " lollypop "},
    {57, " amarok "},
    {58, " sublime-text "},
    {59, " geany "},
    {60, " vlc "},
    {61, " vlc "},
    {62, " kodi "},
    {63, " virtualbox "},
    {64, " firefox "},
    {65, ""},
    {66, ""},
    {67, ""},
    {68, ""},
    {69, ""},
};



void sendtolog(std::string data2) {
    std::cout << data2 << std::endl;
}
void loginfo(std::string data2) {
    data2 = "[RANDOM] - " + data2;
    sendtolog(data2);
}
void logcritical(std::string data2) {
    data2 = "[CRITICAL] - " + data2;
    sendtolog(data2);
}


int main() {

    char buffer4[2056]; // <-- Enough room for both strings and a NULL character
    strcpy(buffer4, starter2);  // Copy in the first string
    char* rainbow = "";
    loginfo("Called to insert random packages...");    
    sleep(3);

    int process = system("echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections");

    int i = 0;

    while (i <= 64) {
        rainbow = packagedictionary[i];
        strcat(buffer4, rainbow);
        i = i + 1;
    }

    /*
    while (i <= 6) {
        i = i + 1;

        rainbow = packagedictionary[int(rand() % 65)];
        strcat(buffer4, rainbow);

    }
    */

    strcat(buffer4, endstring);
    loginfo("installing packages");

    // PROCESS AT THE END
    process = process + system(buffer4);

    if (process != 0) {
        logcritical("Error Occurred in Installing Packages!");
        logcritical("Could not continue");
        return 1;
        return 1;
        return 1;
    }

    sleep(3);
    loginfo("Installing Packages Complete...");
    sleep(1);
    int completed = system("./run");
}