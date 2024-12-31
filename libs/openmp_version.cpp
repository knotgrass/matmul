#include <iostream>
#include <omp.h>

int main() {
    std::cout << "OpenMP Version: " << _OPENMP << std::endl;
    return 0;
}

/**
g++ -fopenmp libs/openmp_version.cpp -o check_openmp_version \
&& ./check_openmp_version
 */
