#include <cstdlib>
#include <string>
#include <cstring>

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

    char buffer4[512]; // <-- Enough room for both strings and a NULL character
    strcpy(buffer4, starter2);  // Copy in the first string
    char* rainbow = "";
    loginfo("Called to insert random packages...");    
    sleep(3);

    int process = system("echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections");

    int i = 0;
    while (i <= 30) {
        //packages[i] = rand() % 127;
        packages[i] = int(rand() % 6);
        i = i + 1;

        switch (packages[i]) {
            case 0:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
                
            case 1:
                // 0
                rainbow = (" htop ");
                strcat(buffer4, rainbow);            
                break;
            case 2:
                // 0
                rainbow = (" build-essential ");
                strcat(buffer4, rainbow);
                break;
            case 3:
                // 0
                rainbow = (" nodejs ");
                strcat(buffer4, rainbow);
                break;
            case 4:
                // 0
                rainbow = (" libreoffice ");
                strcat(buffer4, rainbow);
                break;
            case 5:
                // 0
                rainbow = (" firefox ");
                strcat(buffer4, rainbow);
                break;
            /*
            case 6:
                // 0
                sentence = sentence + char(" chromium-browser ");
                break;
            case 7:
                // 0
                sentence = sentence + char(" docker.io ");
                break;
            case 8:
                // 0
                sentence = sentence + char(" vim ");
                break;
            case 9:
                // 0
                sentence = sentence + char(" git ");
                break;
            case 10:
                // 0
                sentence = sentence + char(" nginx ");
                break;
            case 11:
                // 0
                sentence = sentence + char(" apache2 ");
                break;
            case 12:
                // 0
                sentence = sentence + char(" curl ");
                break;
            case 13:
                // 0
                sentence = sentence + char(" kubernetes ");
                break;
            case 14:
                // 0
                sentence = sentence + char(" mysql-server ");
                break;
            case 15:
                // 0
                sentence = sentence + char(" mysql-client ");
                break;
            case 16:
                // 0
                sentence = sentence + char(" xwayland ");
                break;
            case 17:
                // 0
                sentence = sentence + char(" xserver ");
                break;
            case 18:
                // 0
                sentence = sentence + char(" xz-utils ");
                break;
            case 19:
                // 0
                sentence = sentence + char(" pip ");
                break;
            case 20:
                // 0
                sentence = sentence + char(" synaptic ");
                break;
            case 21:
                // 0
                sentence = sentence + char(" gedit ");
                break;
            case 22:
                // 0
                sentence = sentence + char(" filezilla ");
                break;
            case 23:
                // 0
                sentence = sentence + char(" remmina ");
                break;
            case 24:
                // 0
                sentence = sentence + char(" steam ");
                break;
            case 25:
                // 0
                sentence = sentence + char(" flameshot ");
                break;
            case 26:
                // 0
                sentence = sentence + char(" calibre ");
                break;
            case 27:
                // 0
                sentence = sentence + char(" vim-gtk3 ");
                break;
            case 28:
                // 0
                rainbow = (" cron ");
                strcat(buffer4, rainbow);
                break;
            case 29:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 30:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 31:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 32:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 33:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 34:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 35:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 36:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 37:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 38:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 39:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 40:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 41:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 42:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 43:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 44:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 45:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 46:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 47:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 48:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 49:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 50:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 51:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 52:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 53:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 54:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 55:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 56:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 57:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 58:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 59:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 60:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 61:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 62:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 63:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 64:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 65:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 66:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 67:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 68:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 69:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 70:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 71:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 72:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 73:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 74:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 75:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 76:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 77:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 78:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 79:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 80:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 81:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 82:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 83:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 84:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 85:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 86:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 87:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 88:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 89:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 90:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 91:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 92:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 93:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 94:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 95:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 96:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 97:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 98:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 99:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 100:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 101:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 102:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 103:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 104:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 105:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 106:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 107:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 108:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 109:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 110:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 111:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 112:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 113:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 114:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 115:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 116:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 117:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 118:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 119:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 120:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 121:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 122:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 123:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 124:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 125:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            case 126:
                // 0
                rainbow = (" atop ");
                strcat(buffer4, rainbow);
                break;
            */
        }
    }

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