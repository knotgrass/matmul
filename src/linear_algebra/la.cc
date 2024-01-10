#include <iostream>
#include <vector>


/**
 * Calculates the dot product of two vectors.
 *
 * @tparam T The data type of the elements in the vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The dot product of v1 and v2.
 * @throws std::invalid_argument If vectors are empty or of different sizes.
 */
template <typename T>
T dot_product(const std::vector<T>& v1, const std::vector<T>& v2) {
    if (v1.empty() || v2.empty() || v1.size() != v2.size()) {
        throw std::invalid_argument("Invalid vector dimensions for dot product.");
    }

    T result = 0;
    for (size_t i = 0; i < v1.size(); ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}

/**
 * Retrieves a specified row from a 2D matrix.
 *
 * @tparam T The data type of the elements in the matrix.
 * @param matrix The matrix from which the row will be retrieved.
 * @param row_index The index of the row to retrieve.
 * @return The specified row from the matrix.
 */
template <typename T>
std::vector<T> get_row(
    const std::vector<std::vector<T>>& matrix, int row_index) {
    return matrix[row_index];
}

/**
 * Retrieves a specified column from a 2D matrix.
 *
 * @tparam T The data type of the elements in the matrix.
 * @param matrix The matrix from which the column will be retrieved.
 * @param col_index The index of the column to retrieve.
 * @return The specified column from the matrix.
 */
template <typename T>
std::vector<T> get_column(
    const std::vector<std::vector<T>>& matrix, int col_index) {
    std::vector<T> column;
    for (const auto& row : matrix) {
        column.push_back(row[col_index]);
    }
    return column;
}

/**
 * Transposes a given matrix.
 *
 * @tparam T The data type of the elements in the matrix.
 * @param matrix The matrix to be transposed.
 * @return The transposed matrix.
 */
template <typename T>
std::vector<std::vector<T>> transpose(
    const std::vector<std::vector<T>>& matrix) {
    std::vector<std::vector<T>> transposed(matrix[0].size(), std::vector<T>(matrix.size()));
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }
    return transposed;
}

/**
 * Performs matrix multiplication on two matrices.
 *
 * @tparam T The data type of the elements in the matrices.
 * @param mat1 The first matrix.
 * @param mat2 The second matrix.
 * @return The resultant matrix after multiplication.
 * @throws std::invalid_argument If matrices have incompatible dimensions for multiplication.
 */
template <typename T>
std::vector<std::vector<T>> matmul(
    const std::vector<std::vector<T>>& mat1,
    const std::vector<std::vector<T>>& mat2
) {
    if (mat1.empty() || mat2.empty() || mat1[0].size() != mat2.size()) {
        throw std::invalid_argument("Invalid matrix dimensions for multiplication.");
    }
    std::vector<std::vector<T>> result(mat1.size(), std::vector<T>(mat2[0].size(), 0));
    for (size_t i = 0; i < mat1.size(); ++i) {
        for (size_t j = 0; j < mat2[0].size(); ++j) {
            std::vector<T> row = get_row(mat1, i);
            std::vector<T> col = get_column(mat2, j);
            result[i][j] = dot_product(row, col);
        }
    }
    return result;
}

int test_dot_product(){
    std::vector<int> v1 = {1, 2, 3};
    std::vector<int> v2 = {4, 5, 6};
    int dot_product_result = dot_product(v1, v2);
    std::cout << "Dot product: " << dot_product_result << std::endl;
    return 0;
}

int test_get_row_and_column(const std::vector<std::vector<int>> &matrix){
    std::vector<int> row = get_row(matrix, 1);
    std::vector<int> column = get_column(matrix, 1);

    std::cout << "Row 1: ";
    for (int i : row) std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "Column 1: ";
    for (int i : column) std::cout << i << " ";
    std::cout << std::endl;

    return 0;
}

int test_transpose(const std::vector<std::vector<int>> &matrix){
    std::vector<std::vector<int>> transposed_matrix = transpose(matrix);
    std::cout << "Transposed Matrix:" << std::endl;
    for (const std::vector<int>& row : transposed_matrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}

int test_matmul() {
    // Create two matrices
    std::vector<std::vector<int>> mat1 = {
        {1, 2, 3},
        {4, 5, 6}
    };

    std::vector<std::vector<int>> mat2 = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    // Perform matrix multiplication
    try {
        std::vector<std::vector<int>> result = matmul(mat1, mat2);

        std::cout << "Result of matrix multiplication:" << std::endl;
        for (const auto& row : result) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}

int main(int argc, char** argv) {
    std::vector<std::vector<int>> matrix = {{1, 2, 3},
                                            {4, 5, 6},
                                            {7, 8, 9}};
    test_dot_product();
    test_get_row_and_column(matrix);
    test_transpose(matrix);
    test_matmul();

    return 0;
}
