#include <iostream>
#include <chrono>
#include <cstring> // memset

using namespace std;

int loop_reorder(float* M, float* N, float* P, int rows, int cols, int inner) {
    for (int i = 0; i < rows; i++){
        for (int k = 0; k < inner; k++){
            float m_ik = M[i * inner + k];  // hoist to register, constant in j-loop
            for (int j = 0; j < cols; j++){
                P[i * cols + j] += m_ik * N[k * cols + j];
            }
        }
    }
    return 0;
}

/**
g++ src/naive/cacheline.cpp -o bin/cacheline && bin/cacheline
loop_reorder 2048 * 2048 * 2048 elapsed: 1.56976e+10 ns
*/
int main(int argc, char* argv[]) {
    constexpr int rows = 2 << 10;
    constexpr int inner = 2 << 10;
    constexpr int cols = 2 << 10;

    // NOTE 1: memory allocation
    float* M = new float[rows * inner]; //rows major
    float* N = new float[inner * cols]; //rows major
    float* P = new float[rows * cols]; //rows major

    // NOTE 2: fill value
    srand(42);  // fixed seed for reproducibility

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < inner; j++) {
            M[i * inner + j] = rand() / (float)RAND_MAX;
        }
    }
    for (int i = 0; i < inner; i++) {
        for (int j = 0; j < cols; j++) {
            N[i * cols + j] = rand() / (float)RAND_MAX;
        }
    }

    // NOTE 3: perform matmul
    memset(P, 0, sizeof(float) * rows * cols);  // zero-init: ikj uses P[]+= across k iterations
    auto start = chrono::high_resolution_clock::now();
    loop_reorder(M, N, P, rows, cols, inner);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, nano> elapsed = end - start;
    cout << "loop_reorder " << rows << " * " << inner << " * " << cols
         << " elapsed: " << elapsed.count() << " ns" << endl;

    // NOTE 4: release memory
    delete[] M;
    delete[] N;
    delete[] P;

    return 0;
}
