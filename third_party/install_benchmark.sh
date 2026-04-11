#! /bin/bash

set -e  # Exit on error

version="v1.9.5"
workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

if [ ! -d "third_party/benchmark" ]; then
    echo "Initializing Benchmark submodule..."
    git submodule add -f https://github.com/google/benchmark.git third_party/benchmark
else
    echo "Updating Benchmark submodule..."
    git submodule update --init --recursive
fi

# Configure Benchmark version
cd third_party/benchmark
git fetch --all --tags
git checkout $version

# Clean previous build
rm -rf build

# Set compiler paths and flags
export CC=/home/tz/anaconda3/envs/cxx/bin/gcc
export CXX=/home/tz/anaconda3/envs/cxx/bin/g++

# Create build directory and configure CMake
mkdir -p build && cd build
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$workspaceFolder/libs/benchmark \
    -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on \
    -DBENCHMARK_ENABLE_THREADS=on

# Build
echo "Building Benchmark..."
cmake --build . --config Release -j$(nproc)

# Run tests
echo "Running Benchmark tests..."
ctest --build-config Release

# Install
echo "Installing Benchmark..."
cmake --build . --config Release --target install
