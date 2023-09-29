#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

#define NUM_THREADS 10

using namespace std;

void worker () {
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Hello threads" << endl;
}

int main() {
    vector<thread> threads;
    threads.reserve(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.push_back(thread(worker));
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }
    return 0;
}
