#include <iostream>
#include <gemmlowp/gemmlowp.h>
/*
"-I${workspaceFolder}/third_party/gemmlowp/eight_bit_int_gemm",
"-I${workspaceFolder}/third_party/gemmlowp/fixedpoint",
"-I${workspaceFolder}/third_party/gemmlowp/internal",
"-I${workspaceFolder}/third_party/gemmlowp/meta",
"-I${workspaceFolder}/third_party/gemmlowp/profiling",
"-I${workspaceFolder}/third_party/gemmlowp/public",
*/
int main() {
    // Define the dimensions of the matrices
    const int m = 2;  // Number of rows in A
    const int k = 3;  // Number of columns in A and rows in B
    const int n = 2;  // Number of columns in B

    // Create input matrices A and B
    std::vector<int8_t> matrix_a(m * k);
    std::vector<int8_t> matrix_b(k * n);

    // Initialize input matrices A and B (you should set your own values here)

    // Create an output matrix C with appropriate dimensions
    std::vector<int32_t> matrix_c(m * n);

    // Define Gemmlowp parameters
    gemmlowp::OutputStageType output_type = gemmlowp::OutputStageType::kQuantizeDownInt32ByFixedPoint;
    gemmlowp::OutputPipelineType output_pipeline = gemmlowp::OutputPipelineType::kNoPipeline;

    // Create a Gemmlowp GEMMContext with the specified parameters
    gemmlowp::GEMMContext gemm_context;
    gemm_context.set_max_num_threads(1);  // Set the number of threads to use

    // Perform matrix multiplication
    gemmlowp::GEMM(
        gemm_context,
        matrix_a.data(), matrix_b.data(),
        matrix_c.data(),
        matrix_a.data(), matrix_b.data(),
        m, n, k,
        128,  // Input offset for A
        128,  // Input offset for B
        0,    // Multiplication offset
        0,    // Output offset
        output_type,
        output_pipeline);

    // Print the result matrix C
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << matrix_c[i * n + j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
