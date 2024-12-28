#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std::chrono;

/**
 * @brief Initializes a matrix with a given value.
 *
 * This function fills a 2D matrix of dimensions `row x col` with the specified value.
 *
 * @tparam T The type of the matrix elements.
 * @param row The number of rows in the matrix.
 * @param col The number of columns in the matrix.
 * @param mat A pointer to the matrix to initialize.
 * @param value The value to fill the matrix with.
 */
template <typename T>
void init_matrix_value(int row, int col, T **mat, T value)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            mat[i][j] = value;
        }
    }
}

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

/**
 * @brief The main function of the program.
 *
 * This function performs the following steps:
 * 1. Parses command-line arguments to get matrix dimensions.
 * 2. Allocates memory for matrices `A`, `B`, and `C`.
 * 3. Initializes matrices `A` and `B` with specific values.
 * 4. Performs matrix multiplication and measures execution time.
 * 5. Outputs the time taken for the multiplication.
 * 6. Releases the allocated memory.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 *             - argv[1]: Number of rows in matrix `A` and `C` (m).
 *             - argv[2]: Number of columns in matrix `A` and rows in matrix `B` (n).
 *             - argv[3]: Number of columns in matrix `B` and `C` (p).
 * @return 0 if the program executes successfully, 1 if there is an error.
 */
int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <m> <n> <p>" << std::endl;
        return 1;
    }

    int m = std::atoi(argv[1]);
    int n = std::atoi(argv[2]);
    int p = std::atoi(argv[3]);

    // step 1: Allocate matrix A memory m*n
    float **A = new float *[m];
    for (int i = 0; i < m; i++)
    {
        A[i] = new float[n];
    }

    // step 2: Allocate matrix B memory n*p
    float **B = new float *[n];
    for (int i = 0; i < n; i++)
    {
        B[i] = new float[p];
    }

    // step 3: Allocate matrix C memory m*p
    float **C = new float *[m];
    for (int i = 0; i < m; i++)
    {
        C[i] = new float[p];
    }

    // step 4: Initialize matrix A
    init_matrix_value<float>(m, n, A, 1.0);
    init_matrix_value<float>(n, p, B, 2.0);
    init_matrix_value<float>(m, p, C, 0.0);

    // step 5: Matrix multiplication
    auto t = std::chrono::high_resolution_clock::now();
    matmul(m, n, p, A, B, C);
    auto tt = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<nanoseconds>(tt - t);
    std::cout << "Time: " << duration.count() << " nanoseconds" << std::endl;

    // step 6: Release memory
    for (int i = 0; i < m; i++)
        delete[] A[i];

    for (int i = 0; i < n; i++)
        delete[] B[i];

    for (int i = 0; i < m; i++)
        delete[] C[i];

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}

/*shell
/usr/bin/g++ -fdiagnostics-color=always -std=c++17 -g -O0 \
    src/Efficient_matmul/naive.cc \
    -o bin/matmul

./bin/matmul 1000 1000 1000
*/
