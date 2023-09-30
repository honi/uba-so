#include <atomic>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#define ROWS 300
#define COLS 300
#define THREAD_COUNT 6

using namespace std;
using namespace std::chrono;

typedef vector<vector<int>> matrix;

atomic<int> next_index(0);

void multiply_by_element(matrix &m1, matrix &m2, matrix &m3, vector<int> &counts, int tid) {
    int i;
    while ((i = next_index++) < ROWS * COLS) {
        counts[tid]++;
        int r = i / COLS;
        int c = i % COLS;
        for (int k = 0; k < ROWS; k++) {
            m3[r][c] += m1[r][k] * m2[k][c];
        }
    }
}

void multiply_by_row(matrix &m1, matrix &m2, matrix &m3, vector<int> &counts, int tid) {
    int r;
    while ((r = next_index++) < ROWS) {
        for (int c = 0; c < COLS; c++) {
            counts[tid]++;
            for (int k = 0; k < ROWS; k++) {
                m3[r][c] += m1[r][k] * m2[k][c];
            }
        }
    }
}

void multiply_by_column(matrix &m1, matrix &m2, matrix &m3, vector<int> &counts, int tid) {
    int c;
    while ((c = next_index++) < ROWS) {
        for (int r = 0; r < ROWS; r++) {
            counts[tid]++;
            for (int k = 0; k < ROWS; k++) {
                m3[r][c] += m1[r][k] * m2[k][c];
            }
        }
    }
}

void print_matrix(matrix &m){
    for (int i = 0; i < ROWS; i++) {
        cout << "[ ";
        for (int j = 0; j < COLS; j++) {
            cout << m[i][j];
            if (j < COLS-1) cout << ", ";
        }
        cout << " ]" << endl;
    }
}

int main() {
    matrix m1(ROWS, vector<int>(COLS));
    matrix m2(ROWS, vector<int>(COLS));
    matrix m3(ROWS, vector<int>(COLS));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            m1[i][j] = i * COLS + j + 1;
            m2[i][j] = (ROWS * COLS) - (i * COLS + j);
        }
    }

    vector<thread> threads;
    threads.reserve(THREAD_COUNT);
    vector<int> counts(THREAD_COUNT, 0);

    auto start_time = high_resolution_clock::now();
    for (int i = 0; i < THREAD_COUNT; i++) {
        threads.emplace_back(multiply_by_row, ref(m1), ref(m2), ref(m3), ref(counts), i);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i].join();
    }
    auto end_time = high_resolution_clock::now();

    if (ROWS * COLS < 100) {
        cout << "Matrix 1" << endl;
        print_matrix(m1);
        cout << "Matrix 2" << endl;
        print_matrix(m2);
        cout << "Matrix 3" << endl;
        print_matrix(m3);
    }

    cout << "Multiplications by thread" << endl;
    for (int i = 0; i < THREAD_COUNT; i++) {
        cout << "Thread #" << i << ": " << counts[i] << endl;
    }

    cout << "Execution time: " << duration_cast<milliseconds>(end_time - start_time).count() << " ms" << endl;

    return 0;
}
