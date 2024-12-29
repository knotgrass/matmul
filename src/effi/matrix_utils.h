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
inline void init_matrix_value(int row, int col, T **mat, T value)
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
 * @brief Allocates memory for a matrix.
 *
 * This function allocates memory for a 2D matrix of dimensions `row x col`.
 *
 * @tparam T The type of the matrix elements.
 * @param row The number of rows in the matrix.
 * @param col The number of columns in the matrix.
 * @return A pointer to the allocated matrix.
 */
template <typename T>
inline T **allocate_matrix_memory(int row, int col)
{
    T **mat = new T *[row];
    for (int i = 0; i < row; i++)
        mat[i] = new T[col];

    return mat;
}
/**
 * @brief Creates a matrix and initializes it with a given value.
 *
 * This function creates a 2D matrix of dimensions `row x col` and initializes it with the specified value.
 *
 * @tparam T The type of the matrix elements.
 * @param row The number of rows in the matrix.
 * @param col The number of columns in the matrix.
 * @param value The value to fill the matrix with.
 * @return A pointer to the created matrix.
 */
template <typename T>
inline T **create_matrix(int row, int col, T value)
{
    T **mat = new T *[row];
    for (int i = 0; i < row; i++)
    {
        mat[i] = new T[col];
        for (int j = 0; j < col; j++)
            mat[i][j] = value;
    }

    return mat;
}

/**
 * @brief Releases memory allocated for a matrix.
 *
 * This function releases the memory allocated for a 2D matrix.
 *
 * @tparam T The type of the matrix elements.
 * @param row The number of rows in the matrix.
 * @param mat A pointer to the matrix to release.
 */
template <typename T>
inline void release_matrix_memory(int row, T **mat)
{
    for (int i = 0; i < row; i++)
        delete[] mat[i];
    delete[] mat;
}
