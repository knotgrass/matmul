#include <iostream>
/**
 * @brief C(m, n) = A(m, p) * B(p, n)
 *
 * @return int
 */
template <typename T>
T** naive_implementation(T** A, T** B, int m, int n, int p){
    // T C[m][n] = {};
    T** C = new T*[m];
    for (int i = 0; i < m; i++){
        C[i] = new T[n];
    }

    // Fill C with value 0
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = 0;
        }
    }

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; i++){
            for (int k = 0; k < p; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}
