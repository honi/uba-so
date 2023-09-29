#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>

#define NUM_THREADS 3

using namespace std;

mutex cout_mutex;

void worker (int i) {
    int r = 5;
    while (r--) {
        cout_mutex.lock();
        cout << "Hola! Soy el thread: " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << "Chau! Saludos desde: " << i << endl;
        cout_mutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
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
