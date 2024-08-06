#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <unistd.h> // For sleep()
#include <cstdlib>
#include <csignal>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;
std::string shared_data[10]; // Shared data as a fixed-size array of 10 strings

void loginfo(const std::string &message) {
    std::cout << "[INFO] " << message << std::endl;
}

void logwarning(const std::string &message) {
    std::cout << "[WARNING] " << message << std::endl;
}

void thread1() {
    std::unique_lock<std::mutex> lock(mtx);
    loginfo("Thread 1: Waiting for data...");
    while (!ready) {
        cv.wait(lock);
    }
    loginfo("Thread 1: Received data:");
    for (const auto &data : shared_data) {
        std::cout << data << " ";
    }
    std::cout << std::endl;

    // Modify the shared data
    for (auto &data : shared_data) {
        data += " updated";
    }
    ready = false;
    cv.notify_all();
    loginfo("Thread 1: Data updated and notified.");
}


void shutdownApplication(int signal) {
    if (signal == SIGTERM) {
        std::cout << "Received SIGTERM. Application is shutting down gracefully..." << std::endl;
        // Perform cleanup tasks here
        sleep(1); // Simulate cleanup delay
        exit(0);
    }
}

// Set up signal handling
void setupSignalHandling() {
    struct sigaction sa;
    sa.sa_handler = shutdownApplication;
    sa.sa_flags = SA_RESTART | SA_NODEFER; // Restart interrupted syscalls
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, nullptr);
}



void thread2() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        // Set initial values in shared_data
        for (size_t i = 0; i < 10; ++i) {
            shared_data[i] = "String " + std::to_string(i + 1);
            
        }
        shared_data[9] = "HELLO";
        ready = true;
        loginfo("Thread 2: Data set:");
        for (const auto &data : shared_data) {
            std::cout << data << " ";
        }
        std::cout << std::endl;
        cv.notify_all();
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        while (ready) {
            cv.wait(lock);
        }
    }
    loginfo("Thread 2: Data after update:");
    for (const auto &data : shared_data) {
        std::cout << data << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::thread t1(thread1);
    std::thread t2(thread2);
    t1.detach();
    t2.detach();

    setupSignalHandling();

    std::cout << "Application is running. Waiting for SIGTERM signal..." << std::endl;

    // Main application loop
    while (true) {
        // Your application logic here
        sleep(1);
    }

    return 0;
}
