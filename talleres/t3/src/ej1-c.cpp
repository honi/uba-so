#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

#define NUM_THREADS 10

using namespace std;

void worker (int i) {
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Hello threads: " << i << endl;
}

int main() {
    vector<thread> threads;
    threads.reserve(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.push_back(thread(worker, i));
        // threads.emplace_back(worker, i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }
    this_thread::sleep_for(chrono::milliseconds(501));
    return 0;
}
