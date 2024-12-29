#! /bin/bash

version=1.9.1
workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd $workspaceFolder
mkdir -p third_party
# git submodule add https://github.com/google/benchmark.git third_party/benchmark
cd third_party

#1
git clone https://github.com/google/benchmark.git
cd benchmark
git checkout v$version

#2
cmake -E make_directory "build"
cmake -E chdir "build" cmake \
    -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on \
    -DCMAKE_BUILD_TYPE=Release ../ \
    -DCMAKE_INSTALL_PREFIX=$workspaceFolder/libs/benchmark

#3 Build the library.
cmake --build "build" --config Release
#4 run test to check build
cmake -E chdir "build" ctest --build-config Release

#5 install
cmake --build "build" --config Release --target install

cd $workspaceFolder
