#! /bin/bash

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

mkdir -p third_party

# git submodule add https://github.com/pytorch/cpuinfo third_party/cpuinfo
cd third_party/cpuinfo
git checkout 2f4c278f7aa3e9a451c14c3e9a02c3e091140d96

mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$workspaceFolder/third_party/cpuinfo/install
make all
make install

cd "$workspaceFolder"
