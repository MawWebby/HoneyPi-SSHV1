#include <cstdlib>
#include <string>
#include <cstring>
#include <map>
#include <unistd.h>
#include <iostream>


std::string starter2 = "apt-get install -y -qq ";
std::string endstring = " > nul:";

int packages[30] = {};
std::string sentence = "";


std::map<int, std::string> packagedictionary = {
    {0, " atop "},
    {1, " htop "},
    {2, " build-essential "},
    {3, " make "},
    {4, " nodejs "},
    {5, " libreoffice "},
    {6, " firefox-esr "},
    {7, " chromium "},
    {8, " docker.io "},
    {9, " vim "},
    {10, " git "},
    {11, " nginx "},
    {12, " apache2 "},
    {13, " curl "},
    {14, " bash "},
    {15, " nginx "},
    {16, " mariadb-server "},
    {17, " mariadb-client "},
    {18, " xwayland "},
    {19, " xserver-xorg-core "},
    {20, " xz-utils "},
    {21, " pip "},
    {22, " synaptic "},
    {23, " gedit "},
    {24, " filezilla "},
    {25, " gedit "},
    {26, " remmina "},
    {27, " apache2 "},
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
    {38, " apache2 "},
    {39, " apache2 "},
    {40, " nginx "},
    {41, " snap "},
    {42, " cinnamon "},
    {43, " nginx "},
    {44, " xfce4 "},
    {45, " nginx "},
    {46, " gnome-terminal "},
    {47, " konsole "},
    {48, " terminator "},
    {49, " ardour "},
    {50, " audacity "},
    {51, " blender "},
    {52, " gimp "},
    {53, " apache2 "},
    {54, " kdenlive"},
    {55, " rhythmbox"},
    {56, " lollypop "},
    {57, " nginx "},
    {58, " sublime-text "},
    {59, " geany "},
    {60, " vlc "},
    {61, " vlc "},
    {62, " kodi "},
    {63, " nginx "},
    {64, " firefox-esr "},
    {65, " apache2 "},
    {66, " apache2 "},
    {67, " nginx "},
    {68, " apache2 "},
    {69, " apache2 "},
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

    std::string messagetosend = "";
    messagetosend = messagetosend + starter2;
    loginfo("Called to insert random packages...");    
    sleep(3);

    int process = system("echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections");

    int i = 0;
    
    while (i <= 6) {
        i = i + 1;
        messagetosend = messagetosend + packagedictionary[int(rand() % 65)];
    }

    messagetosend = messagetosend + endstring;
    loginfo("installing packages");

    // PROCESS AT THE END
    process = process + system(messagetosend.c_str());

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