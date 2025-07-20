#include <iostream>
#include <termios.h> // For terminal control
#include <unistd.h>  // For STDIN_FILENO

int main() {
    struct termios oldt, newt;

    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &oldt); 
    newt = oldt;

    // Set terminal to cbreak mode (read characters immediately)
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch;
    std::cout << "Enter characters (Ctrl+C to exit):\n";
    while (std::cin.get(ch)) { // Read character by character
        std::cout << "You entered: " << ch << "(" << (int)ch << ")" << std::endl;
    }

    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}
