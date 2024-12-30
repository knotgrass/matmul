#! /bin/bash

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

mkdir -p third_party

# git submodule add https://github.com/pytorch/cpuinfo third_party/cpuinfo
cd third_party
# git clone https://github.com/pytorch/cpuinfo
cd cpuinfo
git checkout ca156f7bc9109c552973414a63d310f76ef0cbf8

mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$workspaceFolder/libs/cpuinfo
make all -j$(nproc)
make install

cd "$workspaceFolder"
