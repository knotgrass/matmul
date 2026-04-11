#include <iostream>
#include <chrono>

using namespace std;


int matmul1d(float* M, float* N, float* P, int rows, int cols, int inner) {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            float sum = 0.0f;  // register Optimization
            for (int k = 0; k < inner; k++){
                sum += M[i * inner + k] * N[k * cols + j];
            }
            P[i * cols + j] = sum;
        }
    }
    return 0;
}

/**
g++ src/naive/mat1d.cpp -o bin/mat1d && bin/mat1d

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
    auto start = chrono::high_resolution_clock::now();
    matmul1d(M, N, P, rows, cols, inner);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, nano> elapsed = end - start;
    cout << "matmul1d " << rows << " * " << inner << " * " << cols
         << " elapsed: " << elapsed.count() << " ns" << endl;

    // NOTE 4: release memory
    delete[] M;
    delete[] N;
    delete[] P;

    return 0;
}
