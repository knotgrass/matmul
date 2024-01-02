#include <iostream>
#include <vector>
#include <assert.h>

/**
 * Tính toán dot product giữa hai vector.
 *
 * @param iRow Vector đầu tiên, có thể là mảng thông thường hoặc vector.
 * @param iCol Vector thứ hai, có thể là mảng thông thường hoặc vector.
 * @param size Độ dài của vector (hoặc mảng) đầu vào.
 * @param oDot Kết quả của dot product sẽ được lưu vào biến này.
 *
 * @throws std::invalid_argument Nếu độ dài của hai vector không bằng nhau hoặc độ dài không lớn hơn 0.
 */
template <class Tnum>
void dot_product_of_two_vector(
    const Tnum* iRow,
    const Tnum* iCol,
    std::size_t iSize,
    Tnum &oDot
) {
    assert(iSize > 0);
    assert(iRow != nullptr && iCol != nullptr);
    // assert(iRow.size() == size);
    // assert(iCol.size() == size);

    // assert(iRow.size() == iCol.size());
    for (std::size_t i=0; i < iSize; ++i){
        oDot += iRow[i] * iCol[i];
    }
}

int main(int argc, char** argv){
    std::vector<float> row {1, 2, 3, 4, 5};
    float col[5] = {5, 4, 3, 2, 1};
    float dot = 0.0;

    std::cout << "dot = " << dot << '\n';
    dot_product_of_two_vector<float>(row.data(), col, row.size(), dot);
    std::cout << "dot = " << dot << '\n';

    return 0;
}
