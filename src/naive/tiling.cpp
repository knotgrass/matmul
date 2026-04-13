#include <iostream>
#include <chrono>
#include <cstring> // memset
#include <algorithm> // std::min

using namespace std;

int tilling(float* M, float* N, float* P,
            int rows, int inner, int cols,
            int strideM, int strideN, int strideP)
{
    constexpr int TILE_SIZE = 32; // 32*32*4B = 4KB, fits in L1 cache

    for (int i = 0; i < rows; i += TILE_SIZE) {
        for (int k = 0; k < inner; k += TILE_SIZE) {
            for (int j = 0; j < cols; j += TILE_SIZE) {

                int i_stop = std::min(i + TILE_SIZE, rows);
                int k_stop = std::min(k + TILE_SIZE, inner);
                int j_stop = std::min(j + TILE_SIZE, cols);

                for (int ii = i; ii < i_stop; ii++){
                    for (int kk = k; kk < k_stop; kk++){
                        float m_ik = M[ii * strideM + kk];  // hoist to register, constant in j-loop
                        for (int jj = j; jj < j_stop; jj++){
                            P[ii * strideP + jj] += m_ik * N[kk * strideN + jj];
                        }
                    }
                }
            }
        }
    }

    return 0;
}

/**
g++ src/naive/tiling.cpp -o bin/tiling && bin/tiling
tilling 2048 * 2048 * 2048 elapsed: 1.63269e+10 ns
*/

int main(int argc, char* argv[]) {
    constexpr int rows = 2 << 10;
    constexpr int inner = 2 << 10;
    constexpr int cols = 2 << 10;

    constexpr int ALIGN = 16;
    int strideM = ((inner + ALIGN - 1) / ALIGN) * ALIGN;
    int strideN = ((cols  + ALIGN - 1) / ALIGN) * ALIGN;
    int strideP = ((cols  + ALIGN - 1) / ALIGN) * ALIGN;

    // NOTE 1: memory allocation with stride
    float* M = new float[rows * strideM]; //rows major
    float* N = new float[inner * strideN]; //rows major
    float* P = new float[rows * strideP]; //rows major

    // NOTE 2: fill value with stride
    srand(42);  // fixed seed for reproducibility

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < inner; j++) {
            M[i * strideM + j] = rand() / (float)RAND_MAX;
        }
    }
    for (int i = 0; i < inner; i++) {
        for (int j = 0; j < cols; j++) {
            N[i * strideN + j] = rand() / (float)RAND_MAX;
        }
    }
    memset(P, 0, sizeof(float) * rows * strideP);  // zero-init: ikj uses P[]+= across k iterations

    // NOTE 3: perform matmul
    auto start = chrono::high_resolution_clock::now();
    tilling(M, N, P, rows, inner, cols, strideM, strideN, strideP);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, nano> elapsed = end - start;
    cout << "tilling " << rows << " * " << inner << " * " << cols
         << " elapsed: " << elapsed.count() << " ns" << endl;

    // NOTE 4: release memory
    delete[] M;
    delete[] N;
    delete[] P;

    return 0;
}
