#include <thread>
#include <iostream>
#include <unistd.h>

#define NUM_THREADS 10

using namespace std;

void worker () {
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Hello threads" << endl;
}

int main() {
    for (int i = 0; i < NUM_THREADS; i++) {
        new thread(worker);
    }
    this_thread::sleep_for(chrono::milliseconds(1000));
    return 0;
}
