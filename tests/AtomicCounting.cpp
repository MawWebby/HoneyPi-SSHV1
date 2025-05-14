// BY MATTHEW WHITWORTH

#include <atomic>
#include <string>
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <iostream>


std::atomic<int> counter;



void thread1function() {
    int timer = 0;
    while (timer < 20) {
        std::cout << timer << "-" << counter.load() << std::endl;
        sleep(1);
        timer = timer + 1;
    }

    while (true) {
        counter.store(counter.load() - 1);
        std::cout << counter.load() << std::endl;
        sleep(1);
    }
}

void thread2function() {
    while (true) {
        counter.fetch_add(1);
        sleep(1);
    }
}



int main() {
    std::thread thread1(thread1function);
    thread1.detach();

    std::thread thread2(thread2function);
    thread2.detach();

    while (true) {
        sleep(3);
    }
}