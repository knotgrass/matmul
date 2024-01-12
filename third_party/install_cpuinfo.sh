#! /bin/bash

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

mkdir -p third_party

# git submodule add https://github.com/pytorch/cpuinfo third_party/cpuinfo
cd third_party
# git clone https://github.com/pytorch/cpuinfo
cd cpuinfo
git checkout 76cc10d627add77922dc24521b332a055a4d6d77

mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$workspaceFolder/Libs/cpuinfo
make all
make install

cd "$workspaceFolder"
