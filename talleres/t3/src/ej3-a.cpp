#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
#include <thread>
#include <vector>

#define N 100
#define THREAD_COUNT 5
#define STEP N / THREAD_COUNT

using namespace std;

void multiply(vector<int> &v1, vector<int> &v2, vector<int> &v3, int start, int end) {
    for (int i = start; i < end; i++) {
        v3[i] = v1[i] * v2[i];
    }
}

void verify(vector<int> v1, vector<int> v2, vector<int> v3) {
    cout << "v1 = [" << v1[0] << ", " << v1[1] << ", " << "..., " << v1[N - 2] << ", " << v1[N - 1] << "]" << endl;
    cout << "v2 = [" << v2[0] << ", " << v2[1] << ", " << "..., " << v2[N - 2] << ", " << v2[N - 1] << "]" << endl;
    cout << "v3 = [" << v3[0] << ", " << v3[1] << ", " << "..., " << v3[N - 2] << ", " << v3[N - 1] << "]" << endl;

    for (int i = 0; i < N; ++i) {
        assert(v3[i] == v1[i] * v2[i]);
    }
}

int main() {
    vector<int> v1(N);
    vector<int> v2(N);
    iota(v1.begin(), v1.end(), 1);
    iota(v2.begin(), v2.end(), N + 1);

    vector<int> v3(N);

    vector<thread> threads;
    threads.reserve(THREAD_COUNT);

    for (int i = 0; i < THREAD_COUNT; i++) {
        int start = i * STEP;
        int end = (i+1) * STEP;
        threads.emplace_back(multiply, ref(v1), ref(v2), ref(v3), start, end);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i].join();
    }

    verify(v1, v2, v3);
    return 0;
}
