#include <iostream>
#include <cstdlib>
#include <benchmark/benchmark.h>
#include "matrix_utils.h"


/**
 * @brief Performs matrix multiplication.
 *
 * Multiplies two matrices `A` and `B` and stores the result in matrix `C`.
 * Dimensions:
 * - Matrix `A`: m x n
 * - Matrix `B`: n x p
 * - Matrix `C`: m x p
 *
 * @tparam T The type of the matrix elements.
 * @param m The number of rows in matrix `A` and `C`.
 * @param n The number of columns in matrix `A` and rows in matrix `B`.
 * @param p The number of columns in matrix `B` and `C`.
 * @param A The first input matrix (m x n).
 * @param B The second input matrix (n x p).
 * @param C The result matrix (m x p). It must be initialized to zero.
 */
template <typename T>
void matmul(int m, int n, int p, T **A, T **B, T **C)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            for (int k = 0; k < n; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Benchmark function
static void BenchmarkMatmul(benchmark::State &state)
{
    int m = state.range(0);
    int n = state.range(1);
    int p = state.range(2);

    // Allocate matrix A memory m*n
    float **A = new float *[m];
    for (int i = 0; i < m; i++)
        A[i] = new float[n];

    // Allocate matrix B memory n*p
    float **B = new float *[n];
    for (int i = 0; i < n; i++)
        B[i] = new float[p];

    // Allocate matrix C memory m*p
    float **C = new float *[m];
    for (int i = 0; i < m; i++)
        C[i] = new float[p];

    // Initialize matrix A, B and C must be initialized to zero
    init_matrix_value<float>(m, n, A, 1.0);
    init_matrix_value<float>(n, p, B, 2.0);
    init_matrix_value<float>(m, p, C, 0.0);

    // Run the benchmark
    for (auto _ : state)
        matmul(m, n, p, A, B, C);

    // Clean up memory
    for (int i = 0; i < m; i++)
        delete[] A[i];
    for (int i = 0; i < n; i++)
        delete[] B[i];
    for (int i = 0; i < m; i++)
        delete[] C[i];
    delete[] A;
    delete[] B;
    delete[] C;
}

BENCHMARK(BenchmarkMatmul)
    ->Args({128, 128, 128})
    ->Args({256, 256, 256})
    ->Args({512, 512, 512})
    ->Args({1024, 1024, 1024});

// Main entry point for the benchmark
BENCHMARK_MAIN();

/*shell
/usr/bin/g++ src/Efficient_matmul/serial_mm.cc \
    -std=c++17 -O0 \
    -o ./bin/benchmark_matmul \
    -L/home/tz/Documents/myproject/matmul/Libs/benchmark/lib \
    -I/home/tz/Documents/myproject/matmul/Libs/benchmark/include \
    -lbenchmark -lpthread

./bin/benchmark_matmul
*/
