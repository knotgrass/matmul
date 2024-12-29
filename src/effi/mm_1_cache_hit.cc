#include <iostream>
#include <cstdlib>
#include <benchmark/benchmark.h>
#include "matrix_utils.h"


/**
 * @brief Performs matrix multiplication.
 * This version of the function is optimized for cache hit in matrix C.
 * cache hit: C[i][j] and B[k][j] are in the same cache line.
 * https://chatgpt.com/share/6770ecac-1ea8-8005-804a-22e9ada9f69d
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
void matmul_cache_hit(int m, int n, int p, T **A, T **B, T **C)
{
    for (int i = 0; i < m; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < p; j++)
                C[i][j] += A[i][k] * B[k][j];
}

// Benchmark function
static void BenchmarkMatmul(benchmark::State &state)
{
    int m = state.range(0);
    int n = state.range(1);
    int p = state.range(2);

    float **A = create_matrix<float>(m, n, 1.0);
    float **B = create_matrix<float>(n, p, 2.0);
    float **C = create_matrix<float>(m, p, 0.0);

    // Run the benchmark
    for (auto _ : state)
        matmul_cache_hit(m, n, p, A, B, C);

    release_matrix_memory<float>(m, A);
    release_matrix_memory<float>(n, B);
    release_matrix_memory<float>(m, C);
}

BENCHMARK(BenchmarkMatmul)
    ->Args({128, 128, 128})
    ->Args({256, 256, 256})
    ->Args({512, 512, 512})
    ->Args({1024, 1024, 1024});

// Main entry point for the benchmark
BENCHMARK_MAIN();

/*shell
/usr/bin/g++ src/effi/cache_hit.cc \
    -std=c++17 -O0 \
    -o ./bin/benchmark_matmul \
    -Llibs/benchmark/lib \
    -Ilibs/benchmark/include \
    -lbenchmark -lpthread

./bin/benchmark_matmul
*/
