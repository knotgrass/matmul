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
