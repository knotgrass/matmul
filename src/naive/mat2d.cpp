#include <iostream>
#include <chrono>

using namespace std;


int matmul2d(float** M, float** N, float** P, int rows, int cols, int inner) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            P[i][j] = 0.0f;
            for (int k = 0; k < inner; k++) {
                P[i][j] += M[i][k] * N[k][j];
            }
        }
    }
    return 0;
}

/**
g++ src/naive/mat2d.cpp -o bin/mat2d && bin/mat2d
matmul2d 2048 * 2048 * 2048 elapsed: 2.52117e+10 ns
*/
int main(int argc, char* argv[]) {
    int rows = 2 << 10;
    int inner = 2 << 10;
    int cols = 2 << 10;

    // NOTE 1: memory allocation
    float** M = new float*[rows];
    float** N = new float*[inner];
    float** P = new float*[rows];

    for (int i = 0; i < rows; i++) {
        M[i] = new float[inner];
        P[i] = new float[cols];
    }
    for (int i = 0; i < inner; i++) {
        N[i] = new float[cols];
    }

    // NOTE 2: fill value
    srand(42);  // fixed seed for reproducibility

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < inner; j++) {
            M[i][j] = rand() / (float)RAND_MAX;
        }
    }
    for (int i = 0; i < inner; i++) {
        for (int j = 0; j < cols; j++) {
            N[i][j] = rand() / (float)RAND_MAX;
        }
    }

    // NOTE 3: perform matmul
    auto start = chrono::high_resolution_clock::now();
    matmul2d(M, N, P, rows, cols, inner);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, nano> elapsed = end - start;
    cout << "matmul2d " << rows << " * " << inner << " * " << cols
         << " elapsed: " << elapsed.count() << " ns" << endl;

    // NOTE 4: release memory
    for (int i = 0; i < rows; i++) {
        delete[] M[i];
        delete[] P[i];
    }

    for (int i = 0; i < inner; i++) {
        delete[] N[i];
    }
    delete[] M;
    delete[] N;
    delete[] P;

    return 0;
}
