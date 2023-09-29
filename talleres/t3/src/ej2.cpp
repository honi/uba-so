#include <iostream>
#include <iostream>
#include <semaphore.h>
#include <string>
#include <thread>

using namespace std;

#define MSG_COUNT 3

sem_t s1;
sem_t s2;

void fmsg(string name, chrono::milliseconds sleep) {
    for (int i = 0; i < MSG_COUNT; i++) {
        cout << "Ejecutando " << name << endl;
        this_thread::sleep_for(sleep);
    }
}

void f1() {
    fmsg("F1 (A)", 100ms);
    sem_post(&s2);
    sem_wait(&s1);
    fmsg("F1 (B)", 200ms);
}

void f2() {
    fmsg("F2 (A)", 500ms);
    sem_post(&s1);
    sem_wait(&s2);
    fmsg("F2 (B)", 10ms);
}

int main() {
    sem_init(&s1, 0, 0);
    sem_init(&s2, 0, 0);
    thread t1 = thread(f1);
    thread t2 = thread(f2);
    t1.join();
    t2.join();
    sem_destroy(&s1);
    sem_destroy(&s2);
    return 0;
}
